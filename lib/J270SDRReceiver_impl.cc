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
#include "LinkLayer.h"

#include <ASKModulation.h>

namespace gr {
namespace j270sdr {

using output_type = gr_complex;
static std::vector<int16_t> buffer;
J270SDRReceiver::sptr
J270SDRReceiver::make(const std::string& name, bool agc, const std::string& channel, float frequency, int sampleRate, int bandwidth, int gain)
{
    return gnuradio::make_block_sptr<J270SDRReceiver_impl>(name, agc, channel, frequency, sampleRate, bandwidth, gain);
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


static J270SDRControl::Bandwidth convertBandwidth(int bandwidth) {
    switch (bandwidth) {
        case 160000: return J270SDRControl::BW160K;
        case 200000: return J270SDRControl::BW200K;
        case 250000: return J270SDRControl::BW250K;
        case 320000: return J270SDRControl::BW320K;
        case 400000: return J270SDRControl::BW400K;
        case 500000: return J270SDRControl::BW500K;
        case 630000: return J270SDRControl::BW630K;
        case 800000: return J270SDRControl::BW800K;
        case 1000000: return J270SDRControl::BW1000K;
        case 1250000: return J270SDRControl::BW1250K;
        case 1600000: return J270SDRControl::BW1600K;
        case 2000000: return J270SDRControl::BW2000K;
    default: return J270SDRControl::BW2000K;
    }

}

/*
 * The private constructor
 */
J270SDRReceiver_impl::J270SDRReceiver_impl(const std::string& name, bool agc, const std::string& channel, float frequency, int sampleRate, int bandwidth, int gain)
  : gr::block("J270SDRReceiver",
          gr::io_signature::make(0 /* min inputs */, 0 /* max inputs */, 0),
          gr::io_signature::make(1 /* min outputs */, 1 /*max outputs */, sizeof(output_type)))
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
        instance->getControl()->useRxTx(c, instance->getControl()->getTxChannel());
        instance->getControl()->setFrequency(c, frequency);
        instance->getControl()->setRxSampleRate(c, convertSampleRate(sampleRate));
        instance->getControl()->setBandwidth(c, convertBandwidth(bandwidth));
        if (agc)
            instance->getControl()->enableAGC(c);
        else
            instance->getControl()->disableAGC(c);
        instance->getControl()->setAGCS(c, gain);
        instance->getControl()->resetRx(c);
        instance->startRxThread();
        if (!instance->selfCalibrate())
            std::cerr << "J270SDRReceiver_impl::J270SDRReceiver_impl calibration failed" << std::endl;
    }
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
    if (!instance)
        return WORK_DONE;
    auto out = static_cast<output_type*>(output_items[0]);
    buffer.resize(noutput_items * 2);
    auto status = instance->read((uint8_t*)buffer.data(), noutput_items * 4);
    if (status) {
        for (int i = 0; i < noutput_items; i++)
            out[i] = output_type(buffer[i * 2] / 32767.0f, buffer[i * 2 + 1] / 32767.0f);
        return noutput_items;
    }
    std::cout << "J270SDRReceiver_impl::general_work read failed" << std::endl;
    return 0;
}

} /* namespace j270sdr */
} /* namespace gr */

