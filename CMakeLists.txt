# Destruct -- The data structure analysis library 
# Copyright (C) 2013-2014 ArxSys
# This program is free software, distributed under the terms of
# the GNU General Public License Version 2. See the LICENSE file
# at the top of the source tree.
#  
# See http://www.digital-forensic.org for more information about this
# project. Please do not directly contact any of the maintainers of
# DFF for assistance; the project provides a web site, mailing lists
# and IRC channels for your use.
# 
# Author(s):
#  Solal Jacob <sja@digital-forensic.org>

#project (destruct)

cmake_minimum_required (VERSION 2.6)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules/")

set(CMAKE_INCLUDE_PATH "${INCLUDEDIR}")
set(CMAKE_LIBRARY_PATH "${LIBDIR}")

if (WIN32)
  if (DEPENDENCIES_PATH)
    set(DEPENDENCIES_INCLUDE_DIR ${DEPENDENCIES_PATH}/include)
    if (HAVE_64_BITS)
      set(DEPENDENCIES_LIBRARIES_DIR ${DEPENDENCIES_PATH}/lib64)
      set(DEPENDENCIES_RUNTIME_DIR ${DEPENDENCIES_PATH}/bin64)
    else (HAVE_64_BITS)
      set(DEPENDENCIES_LIBRARY_DIR ${DEPENDENCIES_PATH}/lib)
      set(DEPENDENCIES_RUNTIME_DIR ${DEPENDENCIES_PATH}/bin)
    endif(HAVE_64_BITS)
    message("Headers path ${DEPENDENCIES_INCLUDE_DIR}")
    message("Libraries path ${DEPENDENCIES_LIBRARIES_DIR}")
    message("Runtime path ${DEPENDENCIES_RUNTIME_DIR}")
  else (DEPENDENCIES_PATH)
    message(FATAL_ERROR "On Windows platform DEPENDENCIES_PATH must be provided and point to the directory containing include lib[64] and bin[64] folders")
  endif(DEPENDENCIES_PATH)  
endif(WIN32)


if (WIN32)
  set(ICU_INCLUDE_PATH ${DEPENDENCIES_INCLUDE_DIR}/icu)
  set(ICU_LIBRARIES_PATH ${DEPENDENCIES_LIBRARIES_DIR})
  set(ICU_DYNLIB_PATH ${DEPENDENCIES_RUNTIME_DIR})
  message(${ICU_INCLUDE_PATH})
endif (WIN32)

find_package(ICU REQUIRED)

include_directories(ICU_INCLUDE_PATH)
include_directories(${ICU_INCLUDE_DIRS})

IF (NOT ${CMAKE_BINARY_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR})
  SET(DEDICATED_BUILD_DIR 1)
  message(STATUS "Building project in dedicated build directory : ${CMAKE_BINARY_DIR}")
ENDIF (NOT ${CMAKE_BINARY_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR})

option(ENABLE_DEBUG "Compile using -g flag ? Useful for debugging" OFF)
add_definitions(-D__STDC_LIMIT_MACROS)
if(UNIX)
  if (ENABLE_LEAK_DETECTION)
    set (CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} -fsanitize=leak")
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=leak")
    set (CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fsanitize=leak")
    add_definitions(-D_FILE_OFFSET_BITS=64 -D_LARGEFILE64_SOURCE -g -Wall -O1 -fsanitize=leak -fno-omit-frame-pointer -fno-optimize-sibling-calls)
    message(STATUS "Compile using lsan library to detect memory leak, use LD_PRELOAD=path_to_gcc_lsan.so ./dff.py to run")
  elseif(ENABLE_DEBUG)
    add_definitions(-D_FILE_OFFSET_BITS=64 -D_LARGEFILE64_SOURCE -g -Wall)
    message(STATUS "Compile using -g and no optimization")
  else(ENABLE_LEAK_DETECTION)
    add_definitions(-D_FILE_OFFSET_BITS=64 -D_LARGEFILE64_SOURCE -O2 -Wall)
    message(STATUS "Compile with optimization")
  endif(ENABLE_LEAK_DETECTION)
endif(UNIX)


if (MSVC)
  set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /D__value=_value")
endif(MSVC)


include_directories(.)
add_subdirectory(protocol)

set (destruct_source_files
dattribute.cpp 
ddynamicobject.cpp 
dobject.cpp
dsimpleobject.cpp 
dstruct.cpp 
dtype.cpp 
dvalue.cpp
dstructs.cpp
dnullobject.cpp
dexception.cpp
dunicodestring.cpp
dbuffer.cpp

protocol/protocol.cpp
protocol/dmutablestruct.cpp
protocol/dmutableobject.cpp
protocol/dserializebinary.cpp
protocol/dserializetext.cpp
protocol/dserializexml.cpp
protocol/dserializeraw.cpp
protocol/dstream.cpp
protocol/dstreamcout.cpp
protocol/dstreamstring.cpp
protocol/traceobject.cpp
protocol/recursivetraceobject.cpp
protocol/import.cpp
)


configure_file(__init__.py __init__.py COPYONLY)


add_library(destruct SHARED ${destruct_source_files})
target_link_libraries(destruct ${ICU_LIBRARIES})

if(UNIX)
target_link_libraries(destruct dl)
endif(UNIX)

file(RELATIVE_PATH rpath ${CMAKE_BINARY_DIR} ${CMAKE_CURRENT_BINARY_DIR})
if ( CMAKE_GENERATOR MATCHES "Visual Studio")
  set_target_properties (destruct PROPERTIES
    SUFFIX ".dll"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_BINARY_DIR}/python"
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_CURRENT_BINARY_DIR}/python"
    RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_CURRENT_BINARY_DIR}/python"
    )
  file(APPEND "${CMAKE_BINARY_DIR}/installed_files.log" "${rpath}/destruct.dll\n")
elseif (UNIX)
  file(APPEND "${CMAKE_BINARY_DIR}/installed_files.log" "${rpath}/libdestruct.so\n")
endif()

file(APPEND "${CMAKE_BINARY_DIR}/installed_files.log" "${rpath}/__init__.py\n")

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/__init__.py DESTINATION ${INSTALL_FILE_DESTINATION}/${rpath})

add_subdirectory(examples)
add_subdirectory(python)
add_subdirectory(doc)
