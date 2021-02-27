/* -*- c++ -*- */
/*
 * Copyright 2021 gr-ampkey author.
 *
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "samp_delay_impl.h"
// allows amp_tx (time it will take data to be sent, to be added to target time) to be used here
#include "keyer_impl.h"
#include <vector>
#include <iostream>
#include <string.h>
#include <cstring>
#include <chrono>
#include <sys/time.h>
#include <ctime>
#include <math.h>

namespace gr {
  namespace ampkey {

    samp_delay::sptr samp_delay::make(size_t itemsize, int pre_tx, int samp_rate)
    {
      return gnuradio::get_initial_sptr (new samp_delay_impl(itemsize, pre_tx, samp_rate));
    }


    /*
     * The private constructor
     */
    samp_delay_impl::samp_delay_impl(size_t itemsize, int pre_tx, int samp_rate)
      : gr::block("samp_delay",
              gr::io_signature::make(1, -1, itemsize),	// 1 min, -1 (infinite) max, itemsize is # ports
              gr::io_signature::make(1, -1, itemsize)),	// 1 min, -1 (infinite) max, itemsize is # ports
              // must have all arguments here
              d_pre_tx(pre_tx),
              d_samp_rate(samp_rate),
              d_itemsize(itemsize)
    {
    	// for setting of delay initially
    	if (pre_tx_samp < 0) {
    		throw std::runtime_error("delay: Cannot initialize block with a Pre Tx < 0.");
    	}
    	set_dly(pre_tx_samp);
    	d_delta = 0;
    }

    /*
     * Our virtual destructor.
     */
    samp_delay_impl::~samp_delay_impl()
    {
    }

    // starts continuously running function that is used for clock
    bool samp_delay_impl::start()
    {
    	d_finished = false;
    	d_thread = gr::thread::thread(std::bind(&samp_delay_impl::run, this));
	
    	return block::start();
    }
    
    // stops continuously running function
    bool samp_delay_impl::stop()
    {
    	// Shut down the thread
    	d_finished = true;
    	d_thread.interrupt();
    	d_thread.join();

    	return block::stop();
    }
    
    // this is the clock
    // a continuously running function that runs until current_millis reaches target_millis
    void samp_delay_impl::run()
    {
    	while (!d_finished) {
    		// static_cast<long>(#) sets how often the clock updates in milliseconds
    		// ie #=1 the function runs every millisecond
    		boost::this_thread::sleep(
            	boost::posix_time::milliseconds(static_cast<long>(1)));
        	if (d_finished) {
    			return;
        	}
        	// main clock loop
        	// starts clock when target_millis > current_millis
        	if(target_millis > current_millis){
        		// update current_millis
    	    		current_millis = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    	    		// clock ends when current_millis reaches target_millis
    	    		if(target_millis <= current_millis){
    				// time block (and USRP) takes to pass actual data samples (not delay samples)
    				int time_taken = (current_millis - ending_millis) - transmit_millis;
    				// block receives some samples upon startup of the flowgraph
    				// this prevents those samples from setting amp_tx
    				if(init){
    					// sets time it will take data to be sent by USRP
    					// will be added to a target_millis of ampl_keyer_impl.cc
    					amp_tx = std::max(time_taken, count_actual_samples) + 5;
    				}
    				// resets count_actual_samples/transmit_state/init/d_delta
    				// d_delta reset so delay samples will be inserted at beginning of next set of samples
    				// (which is a pdu converted to tagged stream)
    				count_actual_samples = 0;
    				transmit_state = true;
    				init = true;
    				d_delta = pre_tx_samp;
    			}
		}
    	}
    }
    
    // runs on flowgraph startup
    // for setting of delay initially
    void samp_delay_impl::set_dly(int d)
    {
    	// only set a new delta if there is a change in the delay; this
    	// protects from quickly-repeated calls to this function that
    	// would end with d_delta=0.
    	// runs when flowgraph starts
    	if (d != dly()) {
    		gr::thread::scoped_lock l(d_setlock);
    		int old = dly();
    		set_history(d + 1);	//changes dly() from 0 to d (pre_tx)
    		declare_sample_delay(history() - 1);
    		d_delta += dly() - old;
    	}
    }
    
    // runs when samples are received
    // makes sure # outputted samples matches # inputted samples
    void samp_delay_impl::forecast(int noutput_items, gr_vector_int& ninput_items_required)
    {
    	// make sure all inputs have noutput_items available
    	unsigned ninputs = ninput_items_required.size();
    	for (unsigned i = 0; i < ninputs; i++)
        	ninput_items_required[i] = noutput_items;
    }
    
    // runs when samples are received
    // adds delay samples (inserts 0's) to data stream before each set of samples (which is a pdu converted to tagged stream)
    // that are passed through this block
    int samp_delay_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
    	gr::thread::scoped_lock l(d_setlock);
    	assert(input_items.size() == output_items.size());
    	const char* iptr;
    	char* optr;
    	int cons, ret;
    	// update current_millis when sample is received
    	current_millis = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    	// set target_millis when sample is received
    	// target_millis is ending_millis added to current_millis
    	// and is the time when it can be assumed that no more samples will be received
    	target_millis = current_millis + ending_millis;
    	
    	// No change in delay; just pass actual samples
    	if (d_delta == 0) {//runs a few times at start to pad
    		// counts actual # samples received (not delay samples)
    		count_actual_samples += 1;
    		// makes it so transmit is only set once at proper time stated above
    		if(transmit_state){
    			// time when block has finished adding delay samples
    			// and has begun passing actual data samples (not delay samples)
            		transmit_millis = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
            		transmit_state = false;
        	}
        	// pass actual samples to next block downstream
        	for (size_t i = 0; i < input_items.size(); i++) {
            		iptr = (const char*)input_items[i];
            		optr = (char*)output_items[i];
            		std::memcpy(optr, iptr, noutput_items * d_itemsize);
        	}
        	cons = noutput_items;
        	ret = noutput_items;
    	}

    	// produce delay samples (insert 0's)
    	else if (d_delta > 0){ // d_delta > 0
        	int n_from_input, n_padding;
        	n_from_input = std::max(0, noutput_items - d_delta);
        	n_padding = std::min(d_delta, noutput_items);
        	// pass delay and then actual samples to next block downstream
        	for (size_t i = 0; i < input_items.size(); i++) {
            		iptr = (const char*)input_items[i];
            		optr = (char*)output_items[i];
            		std::memset(optr, 0, n_padding * d_itemsize);
            		std::memcpy(optr, iptr, n_from_input * d_itemsize);
        	}
        	cons = n_from_input;
        	ret = noutput_items;
        	d_delta -= n_padding;
    	}
    	
    	// standard for any stream block
    	// tells scheduler how many samples have been consumed
    	consume_each(cons);
    	return ret;
    }

  } /* namespace ampkey */
} /* namespace gr */

