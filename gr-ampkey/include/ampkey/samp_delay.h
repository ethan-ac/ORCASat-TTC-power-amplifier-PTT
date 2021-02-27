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

#ifndef INCLUDED_AMPKEY_SAMP_DELAY_H
#define INCLUDED_AMPKEY_SAMP_DELAY_H

#include <ampkey/api.h>
#include <gnuradio/block.h>

namespace gr {
  namespace ampkey {

    /*!
     * \brief <+description of block+>
     * \ingroup ampkey
     *
     */
    class AMPKEY_API samp_delay : virtual public gr::block
    {
     public:
      typedef boost::shared_ptr<samp_delay> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of ampkey::samp_delay.
       *
       * To avoid accidental use of raw pointers, ampkey::samp_delay's
       * constructor is in a private implementation
       * class. ampkey::samp_delay::make is the public interface for
       * creating new instances.
       */
      static sptr make(size_t itemsize, int pre_tx, int samp_rate);
    };

  } // namespace ampkey
} // namespace gr

#endif /* INCLUDED_AMPKEY_SAMP_DELAY_H */

