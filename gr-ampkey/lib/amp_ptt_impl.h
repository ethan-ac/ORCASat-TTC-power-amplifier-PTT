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

#ifndef INCLUDED_AMPKEY_AMP_PTT_IMPL_H
#define INCLUDED_AMPKEY_AMP_PTT_IMPL_H

#include <ampkey/amp_ptt.h>
/*
// includes for clock
#include <chrono>
#include <iostream>
#include <sys/time.h>
#include <ctime>
#include <vector>
*/
/*
// includes for usb/serial converter
#include <stdio.h>      // standard input / output functions
#include <stdlib.h>
#include <string.h>     // string function definitions
#include <unistd.h>     // UNIX standard function definitions
#include <fcntl.h>      // File control definitions
#include <errno.h>      // Error number definitions
#include <termios.h>    // POSIX terminal control definitions
#include <math.h>
*/
// OFFSET is to compensate for inherent shortening of ptt period by usb/serial cable
#define PTT_OFFSET 5

namespace gr {
  namespace ampkey {
  
  bool samples_incoming;
  
    class amp_ptt_impl : public amp_ptt
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
      // time after data is sent
      int d_post_tx;
      // time during data sending
      int amp_tx;
      
      // will be used to toggle rts# pin on usb/serial cable
      int USB;
      
      // clock current and target times, set to 0 initially so clock wont start on flowgraph startup
      // for d_pre_tx time
      long pre_current_millis = 0;	// current time in milliseconds when d_pre_tx clock is running
      long pre_target_millis = 0;	// target time for d_pre_tx clock to reach in milliseconds
      // for d_post_tx time
      long post_current_millis = 0;	// current time in milliseconds when d_post_tx clock is running
      long post_target_millis = 0;	// target time for d_post_tx clock to reach in milliseconds
      
      // prevents continuous toggling of rts# pin
      bool USB_state = true;
      // makes it so pre_target_millis is only set by first sample
      bool state = true;
      // starts/stops d_pre_tx clock
      bool d_pre_tx_state = true;
      // starts/stops d_post_tx clock
      bool d_post_tx_state = false;
      
      // tags from last work
      std::vector<tag_t> d_work_tags;
      // saves value of tag from last work
      int d_tag_value;
      
      // location of file that controls USB/serial cable
      std::string d_file;
      
      // whether the samples received are from a packet or a unmodulated carrier
      bool d_is_packet;

     public:
      amp_ptt_impl(size_t itemsize, int pre_tx, int post_tx, std::string file);
      ~amp_ptt_impl();

      // continuously running functions where clock runs
      void run();
      bool start() override;
      bool stop() override;
      
      // runs whenever sample is received
      // starts clock when sample is received
      int work(
              int noutput_items,
              gr_vector_const_void_star &input_items,
              gr_vector_void_star &output_items
      );
    };

  } // namespace ampkey
} // namespace gr

#endif /* INCLUDED_AMPKEY_AMP_PTT_IMPL_H */

