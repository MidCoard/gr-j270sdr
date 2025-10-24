/* -*- c++ -*- */
/*
 * Copyright 2025 MidCoard.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_J270SDR_J270SDRRECEIVER_IMPL_H
#define INCLUDED_J270SDR_J270SDRRECEIVER_IMPL_H

#include <j270sdr/J270SDRReceiver.h>
#include "GRScope.h"

namespace gr {
  namespace j270sdr {

    class J270SDRReceiver_impl : public J270SDRReceiver
    {
     private:
      std::shared_ptr<J270SDR> instance;
      double d_sample_rate;

     public:
      explicit J270SDRReceiver_impl(double sample_rate);
      ~J270SDRReceiver_impl() override;

      // Where all the action really happens
      void forecast (int noutput_items, gr_vector_int &ninput_items_required) override;

      int general_work(int noutput_items,
           gr_vector_int &ninput_items,
           gr_vector_const_void_star &input_items,
           gr_vector_void_star &output_items) override;

    };

  } // namespace j270sdr
} // namespace gr

#endif /* INCLUDED_J270SDR_J270SDRRECEIVER_IMPL_H */
