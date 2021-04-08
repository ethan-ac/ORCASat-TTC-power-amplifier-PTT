/* -*- c++ -*- */
/*
 * Copyright 2021 ethan.
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
#include "amp_ptt_impl.h"

namespace gr {
  namespace ampkey {

    amp_ptt::sptr amp_ptt::make(size_t itemsize, int pre_tx, int post_tx, std::string file)
    {
      return gnuradio::get_initial_sptr (new amp_ptt_impl(itemsize, pre_tx, post_tx, file));
    }


    /*
     * The private constructor
     */
    amp_ptt_impl::amp_ptt_impl(size_t itemsize, int pre_tx, int post_tx, std::string file)
      : gr::sync_block("amp_ptt",
              gr::io_signature::make(1, -1, itemsize),	// 1 min, -1 (infinite) max, itemsize is # ports
              gr::io_signature::make(0, 0, 0)),		// no outputs
              d_finished(false),
              d_pre_tx(pre_tx),
              d_post_tx(post_tx),
              d_itemsize(itemsize),
              d_file(file)
    {}

    /*
     * Our virtual destructor.
     */
    amp_ptt_impl::~amp_ptt_impl()
    {
    }
    
    // starts continuously running function that is used for clock
    bool amp_ptt_impl::start()
    {
    	d_finished = false;
    	d_thread = gr::thread::thread(std::bind(&amp_ptt_impl::run, this));
	
    	return block::start();
    }
    
    // stops continuously running function
    bool amp_ptt_impl::stop()
    {
    	// Shut down the thread
    	d_finished = true;
    	d_thread.interrupt();
    	d_thread.join();

    	return block::stop();
    }
    
    // this is the clock
    // continuously running function that keeps rts# pin of usb/serial cable toggled for a length of time
    void amp_ptt_impl::run()
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
        	// starts when pre_target_millis is set larger than pre_current_millis and when enabled by the block receiving a sample
        	// runs for duration of d_pre_tx and amp_tx milliseconds
        	// then starts d_post_tx clock
        	if(pre_target_millis > pre_current_millis && d_pre_tx_state){
      			// update pre_current_millis
    	    		pre_current_millis = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    	    		
    	    		// to prevent open() from being called continuously when clock is running
    	    		if(USB_state){
    	    			// convert from std::string to char
    	    			char d_file_char[d_file.size()+1];
    	    			strcpy(d_file_char, d_file.c_str());
    	    			
    	    			// toggles rts# pin on of usb/serial cable by opening file location for that usb/serial cable
    	        		USB = open(d_file_char, O_RDWR| O_NOCTTY);
    	        		USB_state = false;
    	        		
    	        		// error check if the USB/serial cable file is a valid location
    	        		if (USB < 0) {
    	        			std::cout << "===================================" << std::endl;
    					std::cout << "ERROR Power Amplifier PTT: USB/serial cable file is not a valid file path" << std::endl;
    					std::cout << "===================================" << std::endl;
    	        		}
    	    		}
    	    		
    	    		// when data is received from a unmodulated carrier and samples have stopped coming in to samp_delay
    	    		if (!d_is_packet && !samples_incoming) {
    	    			// exits the d_pre_tx clock
      				pre_target_millis = pre_current_millis;
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
        	
        	// d_post_tx clock for next time a set of samples is received
        	// starts when post_target_millis is set larger than post_current_millis and when enabled by d_pre_tx clock ends
        	// runs for duration of d_post_tx milliseconds
        	// then resets clocks
        	if(post_target_millis > post_current_millis && d_post_tx_state){
      			// update post_current_millis
    	    		post_current_millis = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    	    		
    	    		// sometimes samples_incoming will flip to false while samples are still incoming for samp_delay
    	    		// this is a check to make sure samples are actually not incoming for samp_delay
    	    		if (samples_incoming) {
    	    			// turn of d_post_tx clock and turn on d_pre_tx clock
    	    			d_pre_tx_state = true;
    	    			d_post_tx_state = false;
    	    			
    	    			// update pre_current_millis
    	    			pre_current_millis = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    	    			// reset pre_target_millis to be massive
    	    			pre_target_millis = pre_current_millis + amp_tx;
    	    		}
    	    		// d_post_tx clock ends when post_current_millis reaches post_target_millis
    	    		else if (post_target_millis <= post_current_millis) {
    	    			// toggles rts# pin off of usb/serial cable by closing file location for that usb/serial cable
    	        		close(USB);
    	        		
    	        		// reset USB_state/state/d_pre_tx_state/d_post_tx_state for next time a set of samples is received
    	        		USB_state = true;
    	        		state = true;
    	        		d_post_tx_state = false;
    	    		}
        	}
    	}
    }
    
    // run whenever sample is received
    // d_pre_tx clock starts when first sample is received
    int
    amp_ptt_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
    	// get value of "packet_len" tag which is total length of a packet
    	// "packet_len" is default tag name of "PDU to Tagged Stream" block
    	// used to determine duration of amp_tx time by putting value into line equation
    	uint64_t abs_N, end_N;
        for (size_t i = 0; i < input_items.size(); i++) {
        	abs_N = nitems_read(i);
        	end_N = abs_N + (uint64_t)(noutput_items);
        	get_tags_in_range(d_work_tags, i, abs_N, end_N, pmt::intern("packet_len"));
        	for (const auto& tag : d_work_tags) {
			d_tag_value = pmt::to_long(tag.value);
		}
        }
    	
    	// make it so pre_target_millis is only set by first sample
    	if(state){
    		// updates pre_current_millis to current time
    		pre_current_millis = 		std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    		
    		// calculated by putting total length of a packet into line equation
    		amp_tx = ceil((d_tag_value * 0.83) + 4);
    		
    		// whether the samples received are from a packet or a unmodulated carrier
    		if (d_tag_value > 999) {
    			d_is_packet = false;
    		} else {
    			d_is_packet = true;
    		}
    		
    		// set pre_target_millis when a sample is received to allow d_pre_tx clock to start
    		// pre_current_millis is current time
    		// d_pre_tx is ptt amp toggle time before tx
    		// amp_tx is ptt toggle time during tx
    		// PTT_OFFSET is to compensate for inherent shortening of ptt period by usb/serial cable
    		pre_target_millis = pre_current_millis + d_pre_tx + amp_tx + PTT_OFFSET;
    		d_pre_tx_state = true;
    		state = false;
    	}

    	// standard for any stream block
    	// tells scheduler how many samples have passed
    	return noutput_items;
    }

  } /* namespace ampkey */
} /* namespace gr */

