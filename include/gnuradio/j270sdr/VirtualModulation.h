/* -*- c++ -*- */
/*
 * Copyright 2025 MidCoard.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_J270SDR_VIRTUALMODULATION_H
#define INCLUDED_J270SDR_VIRTUALMODULATION_H

#include <gnuradio/j270sdr/api.h>
#include <gnuradio/block.h>

namespace gr {
  namespace j270sdr {

    /*!
     * \brief <+description of block+>
     * \ingroup j270sdr
     *
     */
    class J270SDR_API VirtualModulation : virtual public gr::block
    {
     public:
      typedef std::shared_ptr<VirtualModulation> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of j270sdr::VirtualModulation.
       *
       * To avoid accidental use of raw pointers, j270sdr::VirtualModulation's
       * constructor is in a private implementation
       * class. j270sdr::VirtualModulation::make is the public interface for
       * creating new instances.
       */
      static sptr make();
    };

  } // namespace j270sdr
} // namespace gr

#endif /* INCLUDED_J270SDR_VIRTUALMODULATION_H */
