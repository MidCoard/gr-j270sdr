/* -*- c++ -*- */
/*
 * Copyright 2025 MidCoard.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_J270SDR_VIRTUALMODULATION_IMPL_H
#define INCLUDED_J270SDR_VIRTUALMODULATION_IMPL_H

#include <gnuradio/j270sdr/VirtualModulation.h>

#include <Modulation.h>
#include <RingBuffer.h>

namespace gr {
  namespace j270sdr {

    class VirtualModulation_impl : public VirtualModulation
    {
     private:
        std::unique_ptr<util::Modulation> d_modulation;
        ByteRingBuffer d_buffer1;
        ByteRingBuffer d_buffer2;
        uint8_t* d_cache;

     public:
      VirtualModulation_impl();
      ~VirtualModulation_impl() override;

      // Where all the action really happens
      void forecast (int noutput_items, gr_vector_int &ninput_items_required) override;

      int general_work(int noutput_items,
           gr_vector_int &ninput_items,
           gr_vector_const_void_star &input_items,
           gr_vector_void_star &output_items) override;

    };

  } // namespace j270sdr
} // namespace gr

#endif /* INCLUDED_J270SDR_VIRTUALMODULATION_IMPL_H */
