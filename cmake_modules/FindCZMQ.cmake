# Locate libyal
# This module defines
# LIZMQ_LIBRARIES
# CZMQ_FOUND, if false, do not try to link to ffmpeg
# CZMQ_INCLUDE_DIR, where to find the headers
#
# $CZMQ_DIR is an environment variable that would
# correspond to the ./configure --prefix=$CZMQ_DIR
#
# Create by Frederic Baguelin based on Robert Osfield's work for FindFFmpeg.cmake.


FIND_PATH(CZMQ_INCLUDE_DIR czmq.h
  PATHS
  ${DEPENDENCIES_INCLUDE_DIR}
  ${CZMQ_ROOT}/include
  $ENV{CZMQ_DIR}/include
  ~/Library/Frameworks
  /Library/Frameworks
  /usr/local/include
  /usr/include
  /sw/include # Fink
  /opt/local/include # DarwinPorts
  /opt/csw/include # Blastwave
  /opt/include
  /usr/freeware/include
  DOC "Location of CZMQ Headers"
  )

FIND_LIBRARY(CZMQ_LIBRARY
  NAMES czmq
  PATHS
  ${DEPENDENCIES_LIBRARIES_DIR}
  ${CZMQ_ROOT}/lib
  $ENV{CZMQ_DIR}/lib
  ~/Library/Frameworks
  /Library/Frameworks
  /usr/local/lib
  /usr/local/lib64
  /usr/lib
  /usr/lib64
  /usr/lib/x86_64-linux-gnu/
  /sw/lib
  /opt/local/lib
  /opt/csw/lib
  /opt/lib
  /usr/freeware/lib64
  DOC "Location of CZMQ Libraries"
  )

message(${CZMQ_LIBRARY})

if(EXISTS "${CZMQ_INCLUDE_DIR}/czmq_library.h")
  file(READ "${CZMQ_INCLUDE_DIR}/czmq_library.h" CZMQ_DEFINITIONS)
  string(REGEX REPLACE ".*#define[\t ]*CZMQ_VERSION_MAJOR[\t ]*([0-9]+).*" "\\1" CZMQ_VERSION_MAJOR  "${CZMQ_DEFINITIONS}")
  string(REGEX REPLACE ".*#define[\t ]*CZMQ_VERSION_MINOR[\t ]*([0-9]+).*" "\\1" CZMQ_VERSION_MINOR  "${CZMQ_DEFINITIONS}")
  string(REGEX REPLACE ".*#define[\t ]*CZMQ_VERSION_PATCH[\t ]*([0-9]+).*" "\\1" CZMQ_VERSION_PATCH  "${CZMQ_DEFINITIONS}")
  set(CZMQ_VERSION "${CZMQ_VERSION_MAJOR}.${CZMQ_VERSION_MINOR}.${CZMQ_VERSION_PATCH}")
  message("CZMQ version: ${CZMQ_VERSION}")
endif()

    
IF (WIN32)
  
  FILE(GLOB CZMQ_RUNTIME_LIBRARY ${DEPENDENCIES_RUNTIME_DIR}/czmq*.dll)
  MESSAGE("CZMQ_RUNTIME_LIBRARY : ${CZMQ_RUNTIME_LIBRARY}")
  
ENDIF(WIN32)

IF (CZMQ_LIBRARY AND CZMQ_INCLUDE_DIR)
  SET(CZMQ_FOUND 1)
ENDIF (CZMQ_LIBRARY AND CZMQ_INCLUDE_DIR)
