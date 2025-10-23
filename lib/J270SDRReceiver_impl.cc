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

namespace gr {
  namespace j270sdr {

    #pragma message("set the following appropriately and remove this warning")
    using input_type = float;
    #pragma message("set the following appropriately and remove this warning")
    using output_type = float;
    J270SDRReceiver::sptr
    J270SDRReceiver::make(double sample_rate)
    {
      return gnuradio::make_block_sptr<J270SDRReceiver_impl>(
        sample_rate);
    }


    /*
     * The private constructor
     */
    J270SDRReceiver_impl::J270SDRReceiver_impl(double sample_rate)
      : gr::block("J270SDRReceiver",
              gr::io_signature::make(0 /* min inputs */, 0 /* max inputs */, 0),
              gr::io_signature::make(1 /* min outputs */, 1 /*max outputs */, sizeof(output_type)))
      , d_sample_rate(sample_rate)
    {}

    void
    J270SDRReceiver_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
    #pragma message("implement a forecast that fills in how many items on each input you need to produce noutput_items and remove this warning")
      (void)noutput_items;
      ninput_items_required.clear();
    }

    int
    J270SDRReceiver_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
      // 无输入块，不访问 input_items
      (void)ninput_items;
      (void)input_items;

      auto out = static_cast<output_type*>(output_items[0]);

      #pragma message("Implement the signal processing in your block and remove this warning")
      // 生成占位输出（全 0），以避免未初始化数据
      std::fill_n(out, noutput_items, static_cast<output_type>(0));

      // 无输入，无需 consume_each

      // 告知运行时我们生产了多少输出项
      return noutput_items;
    }

  } /* namespace j270sdr */
} /* namespace gr */
