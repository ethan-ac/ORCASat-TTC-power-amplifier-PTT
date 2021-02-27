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

#ifndef INCLUDED_AMPKEY_SAMP_DELAY_IMPL_H
#define INCLUDED_AMPKEY_SAMP_DELAY_IMPL_H

#include <ampkey/samp_delay.h>
// include for continuously running function
#include <atomic>

namespace gr {
  namespace ampkey {

    class samp_delay_impl : public samp_delay
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
      
      // sample rate of flowgraph through this block
      int d_samp_rate;
      
      // # samples before data is sent, used as value for delay
      int pre_tx_samp = d_samp_rate*d_pre_tx/1000;
      
      // time before block declares last sample has been received
      int ending_millis = 10;
      
      // sets length of delay in samples
      int d_delta;
      
      // clock current and target times, set to 0 initially so clock wont start on flowgraph startup
      long current_millis = 0;	// current time in milliseconds when clock is running
      long target_millis = 0;		// target time for current time to reach in milliseconds
      // time when block has finished adding delay samples and has begun passing actual data samples (not delay samples)
      long transmit_millis;
      
      // actual # data samples that went through the block before last sample has been received
      int count_actual_samples = 0;
      
      // makes it so transmit is only set once at proper time stated above
      bool transmit_state = true;
      
      // block receives some samples upon startup of the flowgraph
      // this prevents those samples from setting amp_tx
      bool init = false;


     public:
      samp_delay_impl(size_t itemsize, int pre_tx, int samp_rate);
      ~samp_delay_impl();

      // continuously running functions where clock runs
      void run();
      bool start() override;
      bool stop() override;
      
      // runs on flowgraph startup
      // for setting of delay initially
      int dly() const { return history() - 1; }
      void set_dly(int d);
      
      // runs when samples are received
      // makes sure # outputted samples matches # inputted samples
      void forecast (int noutput_items, gr_vector_int &ninput_items_required);
      
      // runs when samples are received
      // adds delay samples (inserts 0's) to data stream before each set of samples (which is a pdu converted to tagged stream) that are passed through this block
      int general_work(int noutput_items,
           gr_vector_int &ninput_items,
           gr_vector_const_void_star &input_items,
           gr_vector_void_star &output_items);

    };

  } // namespace ampkey
} // namespace gr

#endif /* INCLUDED_AMPKEY_SAMP_DELAY_IMPL_H */

