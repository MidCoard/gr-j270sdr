########################################################################
# Check if there is C++ code at all
########################################################################
if (NOT j270sdr_sources)
    message(STATUS "No C++ sources... skipping python bindings")
    return()
endif (NOT j270sdr_sources)

########################################################################
# Remove pygccxml requirement (we use pre-generated bindings)
########################################################################
# gr_python_check_module_raw("pygccxml" "import pygccxml" PYGCCXML_FOUND)

include(GrPybind)

########################################################################
# Python Bindings
########################################################################

list(APPEND j270sdr_python_files
        J270SDRReceiver_python.cc
        J270SDRTransmitter_python.cc
        python_bindings.cc)

# Tell GNU Radio that bindings are pre-generated / manual
gr_pybind_make_oot(
        j270sdr
        ../../..
        gr::j270sdr
        "${j270sdr_python_files}"
        MANUAL
)

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