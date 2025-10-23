find_package(PkgConfig)

PKG_CHECK_MODULES(PC_GR_J270SDR gnuradio-j270sdr)

FIND_PATH(
    GR_J270SDR_INCLUDE_DIRS
    NAMES gnuradio/j270sdr/api.h
    HINTS $ENV{J270SDR_DIR}/include
        ${PC_J270SDR_INCLUDEDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/include
          /usr/local/include
          /usr/include
)

FIND_LIBRARY(
    GR_J270SDR_LIBRARIES
    NAMES gnuradio-j270sdr
    HINTS $ENV{J270SDR_DIR}/lib
        ${PC_J270SDR_LIBDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/lib
          ${CMAKE_INSTALL_PREFIX}/lib64
          /usr/local/lib
          /usr/local/lib64
          /usr/lib
          /usr/lib64
          )

include("${CMAKE_CURRENT_LIST_DIR}/gnuradio-j270sdrTarget.cmake")

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(GR_J270SDR DEFAULT_MSG GR_J270SDR_LIBRARIES GR_J270SDR_INCLUDE_DIRS)
MARK_AS_ADVANCED(GR_J270SDR_LIBRARIES GR_J270SDR_INCLUDE_DIRS)
