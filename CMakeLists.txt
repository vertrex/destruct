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

IF (NOT ${CMAKE_BINARY_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR})
  SET(DEDICATED_BUILD_DIR 1)
  message(STATUS "Building project in dedicated build directory : ${CMAKE_BINARY_DIR}")
ENDIF (NOT ${CMAKE_BINARY_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR})

option(ENABLE_DEBUG "Compile using -g flag ? Useful for debugging" OFF)
add_definitions(-D__STDC_LIMIT_MACROS)
if(UNIX)
  if(ENABLE_DEBUG)
    add_definitions(-D_FILE_OFFSET_BITS=64 -D_LARGEFILE64_SOURCE -g -Wall -std=c++0x)
    message(STATUS "Will use -g for debugging -- yes")
  else(ENABLE_DEBUG)
    add_definitions(-D_FILE_OFFSET_BITS=64 -D_LARGEFILE64_SOURCE -O2 -std=c++0x)
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

set (source_files
dattribute.cpp 
ddynamicobject.cpp 
dobject.cpp
dsimpleobject.cpp 
dstruct.cpp 
dtype.cpp 
dvalue.cpp
destruct.cpp
dserialize.cpp
destruct_test.cpp 
)

set (main 
test.cpp)

add_subdirectory(python)
add_library(destruct SHARED ${source_files})
add_executable(test ${main})

target_link_libraries(test destruct)
