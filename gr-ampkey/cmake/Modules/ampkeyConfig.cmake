INCLUDE(FindPkgConfig)
PKG_CHECK_MODULES(PC_AMPKEY ampkey)

FIND_PATH(
    AMPKEY_INCLUDE_DIRS
    NAMES ampkey/api.h
    HINTS $ENV{AMPKEY_DIR}/include
        ${PC_AMPKEY_INCLUDEDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/include
          /usr/local/include
          /usr/include
)

FIND_LIBRARY(
    AMPKEY_LIBRARIES
    NAMES gnuradio-ampkey
    HINTS $ENV{AMPKEY_DIR}/lib
        ${PC_AMPKEY_LIBDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/lib
          ${CMAKE_INSTALL_PREFIX}/lib64
          /usr/local/lib
          /usr/local/lib64
          /usr/lib
          /usr/lib64
          )

include("${CMAKE_CURRENT_LIST_DIR}/ampkeyTarget.cmake")

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(AMPKEY DEFAULT_MSG AMPKEY_LIBRARIES AMPKEY_INCLUDE_DIRS)
MARK_AS_ADVANCED(AMPKEY_LIBRARIES AMPKEY_INCLUDE_DIRS)
