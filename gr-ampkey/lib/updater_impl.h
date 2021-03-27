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

#ifndef INCLUDED_AMPKEY_UPDATER_IMPL_H
#define INCLUDED_AMPKEY_UPDATER_IMPL_H

#include <ampkey/updater.h>

namespace gr {
  namespace ampkey {

    class updater_impl : public updater
    {
     private:
      bool d_state;

     public:
      updater_impl(bool state);
      ~updater_impl();
      
      int change(int *stet);
      
      int general_work(int noutput_items,
           gr_vector_int &ninput_items,
           gr_vector_const_void_star &input_items,
           gr_vector_void_star &output_items);

    };

  } // namespace ampkey
} // namespace gr

#endif /* INCLUDED_AMPKEY_UPDATER_IMPL_H */

