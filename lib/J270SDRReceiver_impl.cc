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
std::vector<int16_t> buffer;
J270SDRReceiver::sptr
J270SDRReceiver::make(int points)
{
    return gnuradio::make_block_sptr<J270SDRReceiver_impl>(
      points);
}


/*
 * The private constructor
 */
J270SDRReceiver_impl::J270SDRReceiver_impl(int points)
  : gr::block("J270SDRReceiver",
          gr::io_signature::make(0 /* min inputs */, 0 /* max inputs */, 0),
          gr::io_signature::make(1 /* min outputs */, 1 /*max outputs */, sizeof(output_type)))
  , d_points(points), instance(init())
{
    instance->startRxThread();
}
J270SDRReceiver_impl::~J270SDRReceiver_impl()
{
}

void
J270SDRReceiver_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
{
    std::cout << "fuck me " << std::endl;
}

int
J270SDRReceiver_impl::general_work (int noutput_items,
                   gr_vector_int &ninput_items,
                   gr_vector_const_void_star &input_items,
                   gr_vector_void_star &output_items)
{
    auto out = static_cast<output_type*>(output_items[0]);
    buffer.resize(noutput_items * 2);
    auto status = instance->read((uint8_t*)buffer.data(), noutput_items * 4);
    if (status.second) {
        if (status.first)
            std::cout << "data corrupted" << std::endl;
        for (int i = 0; i < noutput_items; i++)
            out[i] = output_type(buffer[i * 2] / 65535.0, buffer[i * 2 + 1] / 65535.0);
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

