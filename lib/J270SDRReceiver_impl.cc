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

namespace gr {
namespace j270sdr {

using output_type = gr_complex;
static std::vector<int16_t> buffer;
J270SDRReceiver::sptr
J270SDRReceiver::make(int points, bool dds)
{
    return gnuradio::make_block_sptr<J270SDRReceiver_impl>(
      points, dds);
}


/*
 * The private constructor
 */
J270SDRReceiver_impl::J270SDRReceiver_impl(int points, bool dds)
  : gr::block("J270SDRReceiver",
          gr::io_signature::make(0 /* min inputs */, 0 /* max inputs */, 0),
          gr::io_signature::make(1 /* min outputs */, 1 /*max outputs */, sizeof(output_type)))
  , d_points(points), instance(init())
{
    if (instance) {
        instance->getControl()->setR9BaseFrequency(J270SDRControl::F754M, 48200300);
        if (dds)
            instance->getControl()->enableDDS();
        else instance->getControl()->disableDDS();
        instance->startRxThread();
        if (!instance->selfCalibrate())
            std::cerr << "J270SDRReceiver_impl::J270SDRReceiver_impl calibration failed" << std::endl;

        // util::DataChunk preambleData(200);
        // for (int i = 0; i < 100; i++) {
        //     preambleData[i] = 1;
        //     preambleData[i + 100] = 0;
        // }
        //
        // thread = std::thread([&] {
        //     auto modulation = std::make_unique<util::ASKModulation>(util::SamplesPerSymbol::SPS_4, 100, 100, std::move(preambleData));
        //     BasicLayer basicLayer(instance, std::move(modulation));
        //     PhysicalLayer layer(std::move(basicLayer));
        //     LinkLayer linkLayer(std::move(layer));
        //     while (true) {
        //         util::DataChunk dataChunk(50);
        //         for (int i = 0; i < 50;i++)
        //             dataChunk[i] = i;
        //         linkLayer.postToLayer(std::move(dataChunk));
        //         std::this_thread::sleep_for(std::chrono::milliseconds(10));
        //     }
        // });
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
    buffer.resize(noutput_items * 2 + 10);
    auto status = instance->read((uint8_t*)buffer.data(), noutput_items * 4);
    if (status.second) {
        for (int i = 0; i < noutput_items; i++)
            out[i] = output_type(buffer[i * 2] / 32767.0f, buffer[i * 2 + 1] / 32767.0f);
        d_read_points += noutput_items;
        if (d_read_points > d_points)
            return WORK_DONE;
        return noutput_items;
    }
    std::cout << "J270SDRReceiver_impl::general_work read failed" << std::endl;
    return 0;
}

} /* namespace j270sdr */
} /* namespace gr */

