/* -*- c++ -*- */
/*
 * Copyright 2025 MidCoard.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <gnuradio/io_signature.h>
#include <gnuradio/gr_complex.h>
#include <algorithm>
#include <cstdint>
#include "VirtualModulation_impl.h"
#include "ASKModulation.h"

namespace gr {
  namespace j270sdr {

    VirtualModulation::sptr
    VirtualModulation::make()
    {
      return gnuradio::make_block_sptr<VirtualModulation_impl>(
        );
    }


    /*
     * The private constructor
     */
    VirtualModulation_impl::VirtualModulation_impl()
      : gr::block("VirtualModulation",
              gr::io_signature::makev(2 /* min inputs */, 2 /* max inputs */,
                                       { static_cast<int>(sizeof(uint8_t)), static_cast<int>(sizeof(gr_complex)) }),
              gr::io_signature::makev(2 /* min outputs */, 2 /* max outputs */,
                                       { static_cast<int>(sizeof(uint8_t)), static_cast<int>(sizeof(gr_complex)) })), d_buffer1(4096 * 10), d_buffer2(4096 * 10), d_cache(new uint8_t[4096 * 4])
    {
        util::DataChunk preambleData(240);
        for (int i = 0; i < 80; i++)
            preambleData[i] = 1;
        for (int i = 80; i < 160; i++)
            preambleData[i] = 0;
        for (int i = 160; i < 200; i++)
            preambleData[i] = 1;
        for (int i = 200; i < 240; i++)
            preambleData[i] = 0;
        d_modulation = std::make_unique<util::ASKModulation>(util::SamplesPerSymbol::SPS_4, 100, 100, std::move(preambleData));
    }

    /*
     * Our virtual destructor.
     */
    VirtualModulation_impl::~VirtualModulation_impl()
    {
        delete [] d_cache;
    }

    void
    VirtualModulation_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
        ninput_items_required[0] = std::max(0, (noutput_items - 240) / 4);
        ninput_items_required[1] = noutput_items;
    }

    int
    VirtualModulation_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
      // Inputs
      auto in_bytes = static_cast<const uint8_t*>(input_items[0]);
      auto in_cplx  = static_cast<const gr_complex*>(input_items[1]);

      // Outputs
      auto out_bytes = static_cast<uint8_t*>(output_items[0]);
      auto out_cplx  = static_cast<gr_complex*>(output_items[1]);
      auto modulated = d_modulation->modulate(util::DataChunk::compose((uint8_t*)in_bytes, ninput_items[0]));
        // std::cout << "VirtualModulation_impl::accept " << ninput_items[0] << " bytes to modulate into " << modulated.size << " complex samples." << std::endl;
        consume(0, ninput_items[0]);
        if (modulated.size > noutput_items * 4) {
            std::cout << "VirtualModulation_impl::general_work insufficient output space for modulated samples: need " << modulated.size / 4 << " but have " << noutput_items << std::endl;
            produce(1, 0);
        } else {
            for (size_t i = 0; i < modulated.size / 4; i++) {
                uint16_t vi = (static_cast<uint16_t>(modulated.data[i * 4 + 2]) << 8) | static_cast<uint16_t>(modulated.data[i * 4 + 3]);
                uint16_t vq = (static_cast<uint16_t>(modulated.data[i * 4]) << 8) | static_cast<uint16_t>(modulated.data[i * 4 + 1]);
                float fi = static_cast<int16_t>(vi) / 32768.0f;
                float fq = static_cast<int16_t>(vq) / 32768.0f;
                out_cplx[i] = gr_complex(fi, fq);
            }
            produce(1, modulated.size / 4);
        }
        // d_buffer1.write(modulated.data, modulated.size);
       auto demodulated = d_modulation->demodulate(util::DataChunk::compose((uint8_t*)in_cplx, ninput_items[1] * 4));
        // std::cout << "VirtualModulation_impl::accept " << ninput_items[1] << " complex samples to demodulate into " << demodulated.size << " bytes." << std::endl;
        if (demodulated.size > noutput_items) {
            std::cout << "VirtualModulation_impl::general_work insufficient output space for demodulated bytes: need " << demodulated.size << " but have " << noutput_items << std::endl;
            produce(0, 0);
        } else {
            std::copy(demodulated.data, demodulated.data + demodulated.size, out_bytes);
            produce(0, demodulated.size);
        }
        consume(1, ninput_items[1]);
        // d_buffer2.write(demodulated.data, demodulated.size);
        // int n1 = 0,n2 = 0;
        // if (d_buffer1.read(d_cache, noutput_items * 4)) {
        //     for (int i = 0; i < noutput_items; i++) {
        //         uint16_t vi = (static_cast<uint16_t>(d_cache[i * 4 + 2]) << 8) | static_cast<uint16_t>(d_cache[i * 4 + 3]);
        //         uint16_t vq = (static_cast<uint16_t>(d_cache[i * 4]) << 8) | static_cast<uint16_t>(d_cache[i * 4 + 1]);
        //         float fi = static_cast<int16_t>(vi) / 32768.0f;
        //         float fq = static_cast<int16_t>(vq) / 32768.0f;
        //         out_cplx[i] = gr_complex(fi, fq);
        //     }
        //     produce(1, noutput_items);
        //     n1 = noutput_items;
        // }
        // else produce(1, 0);
        // if (d_buffer2.read(out_bytes, noutput_items)) {
        //     produce(0, noutput_items);
        //     n2 = noutput_items;
        // }
        // else produce(0, 0);
        // std::cout << "VirtualModulation_impl::general_work processed " << n1 << " complex samples and " << n2 << " bytes." << std::endl;

      return WORK_CALLED_PRODUCE;
    }

  } /* namespace j270sdr */
} /* namespace gr */
