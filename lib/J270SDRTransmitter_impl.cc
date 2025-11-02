// C++
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
    static std::vector<uint16_t> buffer;
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
    }

    int
    J270SDRTransmitter_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
        size_t len = ninput_items[0];

        if (!instance) {
            consume_each(len);
            return WORK_DONE;
        }

        auto in = static_cast<const input_type*>(input_items[0]);
        buffer.resize(len * 2 + 2);
        auto buffer_with_offset = buffer.data();

        for (size_t i = 0; i < len; i++){
            int16_t raw_i = std::clamp(in[i].real() * 32767.0f, -32768.0f, 32767.0f);
            int16_t raw_q = std::clamp(in[i].imag() * 32767.0f, -32768.0f, 32767.0f);
            uint16_t vi = static_cast<uint16_t>(raw_i);
            uint16_t vq = static_cast<uint16_t>(raw_q);
            buffer_with_offset[i * 2]     = vq;
            buffer_with_offset[i * 2 + 1] = vi;
        }

        instance->write(reinterpret_cast<const uint8_t*>(buffer.data()), len * 4);
        consume_each(len);
        return 0;
    }

  } /* namespace j270sdr */
} /* namespace gr */