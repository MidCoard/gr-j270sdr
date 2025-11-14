/* -*- c++ -*- */
/*
 * Copyright 2025 MidCoard.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_J270SDR_VIRTUALBASICLAYER_IMPL_H
#define INCLUDED_J270SDR_VIRTUALBASICLAYER_IMPL_H

#include <gnuradio/j270sdr/VirtualBasicLayer.h>

namespace gr {
  namespace j270sdr {

    class VirtualBasicLayer_impl : public VirtualBasicLayer
    {
     private:
      // Nothing to declare in this block.

     public:
      VirtualBasicLayer_impl();
      ~VirtualBasicLayer_impl();

      // Where all the action really happens
      void forecast (int noutput_items, gr_vector_int &ninput_items_required);

      int general_work(int noutput_items,
           gr_vector_int &ninput_items,
           gr_vector_const_void_star &input_items,
           gr_vector_void_star &output_items);

    };

  } // namespace j270sdr
} // namespace gr

#endif /* INCLUDED_J270SDR_VIRTUALBASICLAYER_IMPL_H */
