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

#ifndef INCLUDED_AMPKEY_AMP_PTT_H
#define INCLUDED_AMPKEY_AMP_PTT_H

#include <ampkey/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
  namespace ampkey {

    /*!
     * \brief <+description of block+>
     * \ingroup ampkey
     *
     */
    class AMPKEY_API amp_ptt : virtual public gr::sync_block
    {
     public:
      typedef boost::shared_ptr<amp_ptt> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of ampkey::amp_ptt.
       *
       * To avoid accidental use of raw pointers, ampkey::amp_ptt's
       * constructor is in a private implementation
       * class. ampkey::amp_ptt::make is the public interface for
       * creating new instances.
       */
      static sptr make(size_t itemsize, int pre_tx, int post_tx, std::string file);
    };

  } // namespace ampkey
} // namespace gr

#endif /* INCLUDED_AMPKEY_AMP_PTT_H */

