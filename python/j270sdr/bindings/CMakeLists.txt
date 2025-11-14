# Copyright 2020 Free Software Foundation, Inc.
#
# This file is part of GNU Radio
#
# SPDX-License-Identifier: GPL-3.0-or-later
#

########################################################################
# Check if there is C++ code at all
########################################################################
if (NOT j270sdr_sources)
    message(STATUS "No C++ sources... skipping python bindings")
    return()
endif (NOT j270sdr_sources)

########################################################################
# Check for pygccxml
########################################################################
gr_python_check_module_raw("pygccxml" "import pygccxml" PYGCCXML_FOUND)

include(GrPybind)

########################################################################
# Python Bindings
########################################################################

list(APPEND j270sdr_python_files
        J270SDRReceiver_python.cc
        J270SDRTransmitter_python.cc
    VirtualBasicLayer_python.cc
    VirtualModulation_python.cc python_bindings.cc)

gr_pybind_make_oot(j270sdr ../../.. gr::j270sdr "${j270sdr_python_files}")

# copy bindings extension for use in QA test module
add_custom_command(
        TARGET j270sdr_python
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:j270sdr_python>
        ${PROJECT_BINARY_DIR}/test_modules/gnuradio/j270sdr/)

install(
        TARGETS j270sdr_python
        DESTINATION ${GR_PYTHON_DIR}/../../../lib/site-packages/gnuradio/j270sdr
        COMPONENT pythonapi)
