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
#include "keyer_impl.h"
// includes for clock
#include <chrono>
#include <iostream>
#include <sys/time.h>
#include <ctime>
#include <vector>
// includes for usb/serial converter
#include <stdio.h>      // standard input / output functions
#include <stdlib.h>
#include <string.h>     // string function definitions
#include <unistd.h>     // UNIX standard function definitions
#include <fcntl.h>      // File control definitions
#include <errno.h>      // Error number definitions
#include <termios.h>    // POSIX terminal control definitions
#include <math.h>

namespace gr {
  namespace ampkey {

    keyer::sptr keyer::make(size_t itemsize, int pre_tx, int post_tx)
    {
      return gnuradio::get_initial_sptr (new keyer_impl(itemsize, pre_tx, post_tx));
    }


    /*
     * The private constructor
     */
    keyer_impl::keyer_impl(size_t itemsize, int pre_tx, int post_tx)
      : gr::sync_block("keyer",
              gr::io_signature::make(1, -1, itemsize),	// 1 min, -1 (infinite) max, itemsize is # ports
              gr::io_signature::make(0, 0, 0)),	// no outputs
              d_finished(false),
              // must have all arguments here
              d_pre_tx(pre_tx),
              d_post_tx(post_tx),
              d_itemsize(itemsize)
    {
    }

    /*
     * Our virtual destructor.
     */
    keyer_impl::~keyer_impl()
    {
    }

    // starts continuously running function that is used for clock
    bool keyer_impl::start()
    {
    	d_finished = false;
    	d_thread = gr::thread::thread(std::bind(&keyer_impl::run, this));
	
    	return block::start();
    }
    
    // stops continuously running function
    bool keyer_impl::stop()
    {
    	// Shut down the thread
    	d_finished = true;
    	d_thread.interrupt();
    	d_thread.join();

    	return block::stop();
    }
    
    // this is the clock
    // a continuously running function that runs until all current_millis have reached their target_millis
    void keyer_impl::run()
    {
    	// static_cast<long>(#) sets how often the clock updates in milliseconds
    	// ie #=1 the function runs every millisecond
    	while (!d_finished) {
    		boost::this_thread::sleep(
            	boost::posix_time::milliseconds(static_cast<long>(1)));
        	if (d_finished) {
            		return;
        	}
        	
        	// d_pre_tx clock
        	// runs for duration of d_pre_tx and amp_tx milliseconds
        	// then starts d_post_tx clock
        	if(pre_target_millis > pre_current_millis && d_pre_tx_state){
      			// update pre_current_millis
    	    		pre_current_millis = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    	    		// to prevent open() from being called continuously when clock is running
    	    		if(USB_state){
    	    			// toggles rts# pin on of usb/serial by opening file for that usb/serial device
    	        		USB = open("/dev/ttyUSB0", O_RDWR| O_NOCTTY);
    	        		USB_state = false;
    	    		}
    	    		// d_pre_tx clock ends when pre_current_millis reaches pre_target_millis
    	    		if(pre_target_millis <= pre_current_millis){
    	    			// updates post_current_millis to current time
    	    			post_current_millis = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    	    			// set post_target_millis to be time when ptt can untoggle
    	    			post_target_millis = post_current_millis + d_post_tx;
    	    			// stops d_pre_tx clock and starts d_post_tx clock
    	    			d_pre_tx_state = false;
    	    			d_post_tx_state = true;
    	    			
    	    		}
        	}
        	
        	// d_post_tx clock
        	// runs for duration of d_post_tx milliseconds
        	// then resets all clocks
        	if(post_target_millis > post_current_millis && d_post_tx_state){
      			// update post_current_millis
    	    		post_current_millis = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    	    		// d_post_tx clock ends when post_current_millis reaches post_target_millis
    	    		if(post_target_millis <= post_current_millis){
    	    			// toggles rts# pin off of usb/serial by closing file for that usb/serial device
    	        		close(USB);
    	        		// reset USB_state/state/d_pre_tx_state/d_post_tx_state
    	        		// for next time a set of samples (which is a pdu converted to a tagged stream) is received
    	        		USB_state = true;
    	        		state = true;
    	        		d_pre_tx_state = true;
    	        		d_post_tx_state = false;
    	    		}
        	}
    	}
    }
    
    // runs whenever sample is received
    // pre_current_millis is updated and pre_target_millis is set here when first sample is received
    int keyer_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
    	uint64_t abs_N, end_N;
        for (size_t i = 0; i < input_items.size(); i++) {
        	abs_N = nitems_read(i);
        	end_N = abs_N + (uint64_t)(noutput_items);
        	get_tags_in_range(d_work_tags, i, abs_N, end_N);
        	for (const auto& tag : d_work_tags) {
			d_tag_value = pmt::to_long(tag.value);
		}
        }
    	
    	// makes it so pre_target_millis is only set by first sample
    	if(state){
    		// updates pre_current_millis to current time
    		pre_current_millis = 		std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    		// calculated by putting total packet length into line equation
    		int amp_tx = ceil((d_tag_value * 0.83) + 4);
    		// set pre_target_millis when a sample is received to allow d_pre_tx clock to start
    		// pre_current_millis is current time
    		// d_pre_tx is ptt toggle time before tx
    		// amp_tx is ptt toggle time during tx
    		// OFFSET is to compensate for inherent shortening of ptt period by usb/serial cable
    		pre_target_millis = pre_current_millis + d_pre_tx + amp_tx + OFFSET;
    		state = false;
    	}
    	
    	// standard for any stream block
    	// can be any value since this block is a sink and doesn't output any samples
    	return noutput_items;
    }

  } /* namespace ampkey */
} /* namespace gr */

