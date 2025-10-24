/* -*- c++ -*- */
/*
 * Copyright 2025 MidCoard.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_J270SDR_J270SDRTRANSMITTER_IMPL_H
#define INCLUDED_J270SDR_J270SDRTRANSMITTER_IMPL_H

#include <gnuradio/j270sdr/J270SDRTransmitter.h>
#include "GRScope.h"

namespace gr {
  namespace j270sdr {

    class J270SDRTransmitter_impl : public J270SDRTransmitter
    {
     private:
      std::shared_ptr<J270SDR> instance;

     public:
      J270SDRTransmitter_impl(float a);
      ~J270SDRTransmitter_impl() override = default;

      // Where all the action really happens
      void forecast (int noutput_items, gr_vector_int &ninput_items_required) override;

      int general_work(int noutput_items,
           gr_vector_int &ninput_items,
           gr_vector_const_void_star &input_items,
           gr_vector_void_star &output_items) override;

    };

  } // namespace j270sdr
} // namespace gr

#endif /* INCLUDED_J270SDR_J270SDRTRANSMITTER_IMPL_H */
