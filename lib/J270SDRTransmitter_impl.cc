// C++
/* -*- c++ -*- */
/*
 * Copyright 2025 MidCoard.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <gnuradio/io_signature.h>
#include "J270SDRTransmitter_impl.h"

#include <util.h>

namespace gr {
  namespace j270sdr {

    using input_type = gr_complex;
    J270SDRTransmitter::sptr
    J270SDRTransmitter::make(const std::string& name, bool dds, const std::string& channel, float frequency, int sampleRate, int gain)
    {
      return gnuradio::make_block_sptr<J270SDRTransmitter_impl>(
        name, dds, channel, frequency, sampleRate, gain);
    }

  static J270SDRControl::SampleRate convertSampleRate(int sampleRate) {
        switch (sampleRate) {
        case 4000000: return J270SDRControl::S4M;
        case 2000000: return J270SDRControl::S2M;
        case 1333333: return J270SDRControl::S4_3M;
        case 1000000: return J270SDRControl::S1M;
        case 800000: return J270SDRControl::S800K;
        case 666667: return J270SDRControl::S2000_3K;
        case 500000: return J270SDRControl::S500K;
        case 400000: return J270SDRControl::S400K;
        default: return J270SDRControl::S4M;
        }
    }



    /*
     * The private constructor
     */
    J270SDRTransmitter_impl::J270SDRTransmitter_impl(const std::string& name, bool dds,
                                                     const std::string& channel, float frequency, int sampleRate, int gain)
      : gr::block("J270SDRTransmitter",
              gr::io_signature::make(1 /* min inputs */, 1 /* max inputs */, sizeof(input_type)),
              gr::io_signature::make(0 /* min outputs */, 0 /*max outputs */, 0))
    {
        try {
            instance = J270SDR::getInstance(name);
        } catch (const std::exception& e) {
            std::cerr << "Cannot open J270 device: " << e.what() << std::endl;
            auto devices = J270SDR::listUSBDevices();
            if (devices.empty())
                std::cerr << "No J270 device found." << std::endl;
            else {
                std::cerr << "Available devices: " << std::endl;
                for (const auto& dev : devices)
                    std::cerr << dev << std::endl;
            }
            instance = nullptr;
        }
        if (instance) {
            J270SDRControl::Channel c = J270SDRControl::R9;
            if (channel == "R24") {
                c = J270SDRControl::R24;
                instance->getControl()->enableR24();
            }
            instance->getControl()->useRxTx(instance->getControl()->getRxChannel(), c);
            if (dds)
                instance->getControl()->enableDDS();
            else instance->getControl()->disableDDS();
            instance->getControl()->setFrequency(c, frequency);
            instance->getControl()->setTxSampleRate(c, convertSampleRate(sampleRate));
            instance->getControl()->setTransmitPower(c, gain);
            instance->getControl()->resetTx(c);
        }
    }

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

        auto in = (input_type*)input_items[0];
        auto chunk = util::packIQ(in, len, true);
        instance->write(chunk.data, chunk.size);
        consume_each(len);
        return 0;
    }

  } /* namespace j270sdr */
} /* namespace gr */