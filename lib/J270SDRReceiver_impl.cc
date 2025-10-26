/* -*- c++ -*- */
/*
 * Copyright 2025 MidCoard.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <gnuradio/io_signature.h>
#include "J270SDRReceiver_impl.h"
#include <algorithm>
#include "J270SDR.h"
#include "util.h"

namespace gr {
  namespace j270sdr {

    using output_type = gr_complex;
    std::vector<uint16_t> buffer;
    J270SDRReceiver::sptr
    J270SDRReceiver::make(int sample_rate, int points)
    {
      return gnuradio::make_block_sptr<J270SDRReceiver_impl>(
        sample_rate, points);
    }


    /*
     * The private constructor
     */
    J270SDRReceiver_impl::J270SDRReceiver_impl(int sample_rate, int points)
      : gr::block("J270SDRReceiver",
              gr::io_signature::make(0 /* min inputs */, 0 /* max inputs */, 0),
              gr::io_signature::make(1 /* min outputs */, 1 /*max outputs */, sizeof(output_type)))
      , d_sample_rate(sample_rate), d_points(points), instance(init())
    {
        instance->startRxThread();
    }
    J270SDRReceiver_impl::~J270SDRReceiver_impl()
    {
    }

    void
    J270SDRReceiver_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
    }

    int
    J270SDRReceiver_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
      auto out = static_cast<output_type*>(output_items[0]);

        instance->read((uint8_t*)buffer.data(), noutput_items * 2);
        for (int i = 0; i < noutput_items; i++)
            out[i] = output_type(buffer[i * 2], buffer[i * 2 + 1]);

      return noutput_items;
    }

  } /* namespace j270sdr */
} /* namespace gr */

