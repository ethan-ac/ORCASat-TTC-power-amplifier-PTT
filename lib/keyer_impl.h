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

#ifndef INCLUDED_AMPKEY_KEYER_IMPL_H
#define INCLUDED_AMPKEY_KEYER_IMPL_H

#include <ampkey/keyer.h>

namespace gr {
  namespace ampkey {
  
  // variable declared here so it can be shared with que_delay_impl.cc\
  // time it will take data to be sent
  // will be added to target time
  extern int amp_tx;
  
    class keyer_impl : public keyer
    {
     private:
      // for continuously running function
      gr::thread::thread d_thread;
      std::atomic<bool> d_finished;
      
      // needed for configurable data type on ports
      const size_t d_itemsize;
      
      // time (ms) to add to clock to create target time
      // time before data is sent
      int d_pre_tx;
      //time after data is sent
      int d_post_tx;
      
      // will be used to toggle rts# pin on usb/serial
      int USB;
      
      // clock current and target times, set to 0 initially so clock wont start on flowgraph startup
      // for d_pre_tx time
      long pre_current_millis = 0;	// current time in milliseconds when d_pre_tx clock is running
      long pre_target_millis = 0;	// target time for d_pre_tx clock to reach in milliseconds
      // for amp_tx time
      long mid_current_millis = 0;	// current time in milliseconds when amp_tx clock is running
      long mid_target_millis = 0;	// target time for d_pre_tx clock to reach in milliseconds
      // for d_post_tx time
      long post_current_millis = 0;	// current time in milliseconds when d_post_tx clock is running
      long post_target_millis = 0;	// target time for d_post_tx clock to reach in milliseconds
      
      // prevents continuous toggling of rts# pin
      bool USB_state = true;
      // makes it so pre_target_millis is only set by first sample
      bool state = true;
      // starts/stops d_pre_tx clock
      bool d_pre_tx_state = true;
      // starts/stops amp_tx clock
      bool amp_tx_state = false;
      // starts/stops d_post_tx clock
      bool d_post_tx_state = false;


     public:
      keyer_impl(size_t itemsize, int pre_tx, int post_tx);
      ~keyer_impl();

      // continuously running functions where clock runs
      void run();
      bool start() override;
      bool stop() override;
      
      // runs whenever sample is received
      // pre_current_millis is updated and pre_target_millis is set here when first sample is received
      int work(
              int noutput_items,
              gr_vector_const_void_star &input_items,
              gr_vector_void_star &output_items
      );
    };

  } // namespace ampkey
} // namespace gr


#endif /* INCLUDED_AMPKEY_KEYER_IMPL_H */

