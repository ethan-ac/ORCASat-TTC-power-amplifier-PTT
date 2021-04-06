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
#include "updater_impl.h"

namespace gr {
  namespace ampkey {

    updater::sptr updater::make(bool state)
    {
      return gnuradio::get_initial_sptr (new updater_impl(state));
    }


    /*
     * The private constructor
     */
    updater_impl::updater_impl(bool state)
      : gr::block("updater",
              gr::io_signature::make(1, 1, sizeof(gr_complex)),
              gr::io_signature::make(1, 1, sizeof(gr_complex))),
              d_state(state)
    {
    	
    }

    /*
     * Our virtual destructor.
     */
    updater_impl::~updater_impl()
    {
    }
    
    int updater_impl::change(int *stet)
    {
    	*stet = 5;
    	return *stet;
    }
    
    int
    updater_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
      const gr_complex *in = (const gr_complex *) input_items[0];
      gr_complex *out = (gr_complex *) output_items[0];
      
      int x = 3;
      std::cout << change(&x) << std::endl;
      std::cout << d_state << std::endl;
      // Tell runtime system how many input items we consumed on
      // each input stream.
      consume_each (noutput_items);

      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace ampkey */
} /* namespace gr */

