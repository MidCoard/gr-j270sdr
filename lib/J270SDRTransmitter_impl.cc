/* -*- c++ -*- */
/*
 * Copyright 2025 MidCoard.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <gnuradio/io_signature.h>
#include "J270SDRTransmitter_impl.h"

namespace gr {
  namespace j270sdr {

    using input_type = gr_complex;
    J270SDRTransmitter::sptr
    J270SDRTransmitter::make(float a)
    {
      return gnuradio::make_block_sptr<J270SDRTransmitter_impl>(
        a);
    }


    /*
     * The private constructor
     */
    J270SDRTransmitter_impl::J270SDRTransmitter_impl(float a)
      : gr::block("J270SDRTransmitter",
              gr::io_signature::make(1 /* min inputs */, 1 /* max inputs */, sizeof(input_type)),
              gr::io_signature::make(0 /* min outputs */, 0 /*max outputs */, 0)), instance(init())
    {}


    void
    J270SDRTransmitter_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
    #pragma message("implement a forecast that fills in how many items on each input you need to produce noutput_items and remove this warning")
      /* <+forecast+> e.g. ninput_items_required[0] = noutput_items */
    }

    int
    J270SDRTransmitter_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
      auto in = static_cast<const input_type*>(input_items[0]);


      consume_each (noutput_items);
      return noutput_items;
    }

  } /* namespace j270sdr */
} /* namespace gr */
