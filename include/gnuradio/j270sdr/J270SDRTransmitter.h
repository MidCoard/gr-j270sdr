/* -*- c++ -*- */
/*
 * Copyright 2025 MidCoard.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_J270SDR_J270SDRTRANSMITTER_H
#define INCLUDED_J270SDR_J270SDRTRANSMITTER_H

#include <gnuradio/j270sdr/api.h>
#include <gnuradio/block.h>

namespace gr {
  namespace j270sdr {

    /*!
     * \brief <+description of block+>
     * \ingroup j270sdr
     *
     */
    class J270SDR_API J270SDRTransmitter : virtual public gr::block
    {
     public:
      typedef std::shared_ptr<J270SDRTransmitter> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of j270sdr::J270SDRTransmitter.
       *
       * To avoid accidental use of raw pointers, j270sdr::J270SDRTransmitter's
       * constructor is in a private implementation
       * class. j270sdr::J270SDRTransmitter::make is the public interface for
       * creating new instances.
       */
      static sptr make(float a);
    };

  } // namespace j270sdr
} // namespace gr

#endif /* INCLUDED_J270SDR_J270SDRTRANSMITTER_H */
