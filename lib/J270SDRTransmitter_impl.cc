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
    static std::vector<int16_t> buffer;
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
        std::cout << "Transmitter general_work called with ninput_items: " << ninput_items[0] << ", noutput_items: " << noutput_items << std::endl;
        if (!instance) {
            consume_each (noutput_items);
            return WORK_DONE;
        }
      auto in = static_cast<const input_type*>(input_items[0]);
        size_t len = ninput_items[0];
        buffer.resize(len * 2);

        for (size_t i = 0; i < len; i++){
            buffer[i * 2] = static_cast<int16_t>(std::clamp(in[i].real() * 32767.0f, -32768.0f, 32767.0f));
            buffer[i * 2 + 1] = static_cast<int16_t>(std::clamp(in[i].imag() * 32767.0f, -32768.0f, 32767.0f));
        }

        instance->write(reinterpret_cast<const uint8_t*>(buffer.data()), len * 4);
      consume_each (noutput_items);
      return 0;
    }

  } /* namespace j270sdr */
} /* namespace gr */
