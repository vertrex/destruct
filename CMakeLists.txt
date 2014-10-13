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

project (destruct)

cmake_minimum_required (VERSION 2.6)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules/")

set(CMAKE_INCLUDE_PATH "${INCLUDEDIR}")
set(CMAKE_LIBRARY_PATH "${LIBDIR}")

include("cmake_modules/dff/macros.cmake")

IF (NOT ${CMAKE_BINARY_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR})
  SET(DEDICATED_BUILD_DIR 1)
  message(STATUS "Building project in dedicated build directory : ${CMAKE_BINARY_DIR}")
ENDIF (NOT ${CMAKE_BINARY_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR})

option(ENABLE_DEBUG "Compile using -g flag ? Useful for debugging" OFF)
add_definitions(-D__STDC_LIMIT_MACROS)
if(UNIX)
  if(ENABLE_DEBUG)
    #add_definitions(-D_FILE_OFFSET_BITS=64 -D_LARGEFILE64_SOURCE -g -Wall -std=c++0x)
    #add_definitions(-D_FILE_OFFSET_BITS=64 -D_LARGEFILE64_SOURCE -g -Wall -fstack-protector-all -D_FORTIFY_SOURCE=2) #fstack-protector-strong 
    #add_definitions(-D_FILE_OFFSET_BITS=64 -D_LARGEFILE64_SOURCE -g -pg -Wall) #fstack-protector-strong 
    add_definitions(-D_FILE_OFFSET_BITS=64 -D_LARGEFILE64_SOURCE -g -Wall) #fstack-protector-strong 
    message(STATUS "Will use -g for debugging -- yes")
  else(ENABLE_DEBUG)
    #add_definitions(-D_FILE_OFFSET_BITS=64 -D_LARGEFILE64_SOURCE -O2 -std=c++0x)
    add_definitions(-D_FILE_OFFSET_BITS=64 -D_LARGEFILE64_SOURCE -O2 -Wall) 
    message(STATUS "Will use -g for debugging -- no")
  endif(ENABLE_DEBUG)
endif(UNIX)

if (ICU_DEP_PATH)
  if (WIN32)
    if (HAVE_64_BIT)
      set(ICU_PATH_SUFFIX "64")
    else()
      set(ICU_PATH_SUFFIX "")
    endif()
    set(ICU_INCLUDE_PATH "${ICU_DEP_PATH}/include")
    set(ICU_LIBRARIES_PATH "${ICU_DEP_PATH}/lib${ICU_PATH_SUFFIX}")
    set(ICU_DYNLIB_PATH "${ICU_DEP_PATH}/bin${ICU_PATH_SUFFIX}")
  endif (WIN32)
endif (ICU_DEP_PATH)

find_package(ICU REQUIRED)
FIND_PACKAGE(PythonInterp REQUIRED)

execute_process ( COMMAND ${PYTHON_EXECUTABLE} -c "from distutils.sysconfig import get_python_lib; print(get_python_lib())" OUTPUT_VARIABLE PYTHON_SITE_PACKAGES_PATH OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process ( COMMAND ${PYTHON_EXECUTABLE} -c "from distutils.sysconfig import get_python_lib; print(get_python_lib())" OUTPUT_VARIABLE PYTHON_SITE_PACKAGES_PATH OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process ( COMMAND ${PYTHON_EXECUTABLE} -c "from distutils.sysconfig import get_python_version; print(get_python_version())" OUTPUT_VARIABLE PYTHON_VERSION OUTPUT_STRIP_TRAILING_WHITESPACE)
if (UNIX)
  if (APPLE)
    execute_process ( COMMAND ${PYTHON_EXECUTABLE} -c "from distutils.sysconfig import get_config_vars; print(get_config_vars()[\"PYTHONFRAMEWORKPREFIX\"] + \"/\" + get_config_vars()[\"LDLIBRARY\"])" OUTPUT_VARIABLE PYTHON_LIBRARIES OUTPUT_STRIP_TRAILING_WHITESPACE)
  else ()
    execute_process ( COMMAND ${PYTHON_EXECUTABLE} -c "from distutils.sysconfig import get_config_vars; print(get_config_vars()[\"LIBDIR\"] + \"/\" + get_config_vars()[\"LDLIBRARY\"])" OUTPUT_VARIABLE PYTHON_LIBRARIES OUTPUT_STRIP_TRAILING_WHITESPACE)
  endif (APPLE)
  execute_process ( COMMAND ${PYTHON_EXECUTABLE} -c "from distutils.sysconfig import get_python_inc; print(get_python_inc())" OUTPUT_VARIABLE PYTHON_INCLUDE_PATH OUTPUT_STRIP_TRAILING_WHITESPACE)
  execute_process ( COMMAND ${PYTHON_EXECUTABLE} -c "from distutils.sysconfig import get_config_vars; print(get_config_vars()[\"BINDIR\"])" OUTPUT_VARIABLE PYTHON_BIN_PATH OUTPUT_STRIP_TRAILING_WHITESPACE)
  if(NOT PYTHON_LIBRARIES)
    message(SEND_ERROR "No Python library found ; please install Python package, at least version 2.5.")
  else(NOT PYTHON_LIBRARIES)
    message(STATUS "Python library found: ${PYTHON_LIBRARIES}")
  endif(NOT PYTHON_LIBRARIES)
  if(NOT EXISTS ${PYTHON_INCLUDE_PATH}/Python.h)
    message(SEND_ERROR "No Python include/header file found; install Python development package.")
  else(NOT EXISTS ${PYTHON_INCLUDE_PATH}/Python.h)
    message(STATUS "Python header found: ${PYTHON_INCLUDE_PATH}")
  endif(NOT EXISTS ${PYTHON_INCLUDE_PATH}/Python.h)
else (UNIX)
  find_package(PythonLibs)
  find_package(PythonInterp)
  SET(PYTHON_BIN_PATH ${PYTHON_EXECUTABLE})
endif(UNIX)

message(STATUS "Python in: ${PYTHON_BIN_PATH}")


FIND_PACKAGE(PythonLibrary REQUIRED)
INCLUDE(PythonMacros)
IF (NOT MSVC)
  ADD_DEFINITIONS(-fPIC)
ENDIF ()

find_package(SWIG REQUIRED)
if (${SWIG_VERSION_MAJOR} LESS 2 OR (${SWIG_VERSION_MAJOR} EQUAL 2 AND ${SWIG_VERSION_PATCH} LESS 7))
  message(FATAL_ERROR "Need SWIG version >= 2.0.7 (current version is ${SWIG_VERSION})")
else()
  message(STATUS "Found compatible SWIG version (${SWIG_VERSION})")
endif()
include(${SWIG_USE_FILE})

# Check 64 bit
if( "${CMAKE_SIZEOF_VOID_P}" EQUAL 4 )
  set( HAVE_64_BITS 0 )
else( "${CMAKE_SIZEOF_VOID_P}" EQUAL 4 )
  if (WIN32)
    set(CMAKE_SWIG_FLAGS ${CMAKE_SWIG_FLAGS} -DSWIGWORDSIZE32)
  else()
    set(CMAKE_SWIG_FLAGS ${CMAKE_SWIG_FLAGS} -DSWIGWORDSIZE64)
  endif()
  set( HAVE_64_BITS 1 )
endif( "${CMAKE_SIZEOF_VOID_P}" EQUAL 4 )
#

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
destruct.cpp
dnullobject.cpp
dexception.cpp

protocol/protocol.cpp
protocol/dmutablestruct.cpp
protocol/dmutableobject.cpp
protocol/diterator.cpp
protocol/dserialize.cpp
protocol/dstream.cpp
)

add_library(destruct SHARED ${destruct_source_files})


#add_subdirectory(examples)
add_subdirectory(python)
#add_subdirectory(doc)
