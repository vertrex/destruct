# DFF -- An Open Source Digital Forensics Framework
# Copyright (C) 2009-2013 ArxSys
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

set (pydestruct_files
py_module.cpp
py_dstructs.cpp
py_dstruct.cpp
py_dattribute.cpp
py_dnullobject.cpp
py_dtype.cpp
py_dvalue.cpp
py_dobject.cpp
py_dmethodobject.cpp
#py_dstream.cpp
#py_dserialize.cpp
destructmodule.cpp
)

if (MSVC)
  find_package(PythonLibs)
  #if (PythonLibs_FOUND)
    include_directories(${PYTHON_INCLUDE_PATH})
    include_directories(${PYTHON_LIBRARY})
    #  else()
    #set(PYTHON_LIBRARY        "C:/Python27/libs/python27.lib")
    #set(PYTHON_INCLUDE_DIR    "C:/Python27/include")
    #  endif()
  set(PYTHON_BIN_PATH ${PYTHON_EXECUTABLE})
endif(MSVC)

if (UNIX)
  FIND_PACKAGE(PythonLibrary REQUIRED)
    include_directories(${PYTHON_INCLUDE_PATH})
endif(UNIX)

add_library(_destruct SHARED ${pydestruct_files})

set_target_properties(_destruct PROPERTIES PREFIX "")

file(RELATIVE_PATH rpath ${CMAKE_BINARY_DIR} ${CMAKE_CURRENT_BINARY_DIR})
if (MSVC)
  target_link_libraries(_destruct destruct ${PYTHON_LIBRARY})
  set_target_properties (_destruct PROPERTIES
    SUFFIX ".pyd"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_BINARY_DIR}"
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_CURRENT_BINARY_DIR}"
    RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_CURRENT_BINARY_DIR}"
    )
  file(APPEND "${CMAKE_BINARY_DIR}/installed_files.log" "${rpath}/_destruct.pyd\n")
else(MSVC)
  target_link_libraries(_destruct destruct python2.7)
  file(APPEND "${CMAKE_BINARY_DIR}/installed_files.log" "${rpath}/_destruct.so\n")
endif(MSVC)

file(APPEND "${CMAKE_BINARY_DIR}/installed_files.log" "${rpath}/__init__.py\n")

configure_file(__init__.py __init__.py COPYONLY)
include_directories("../")

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/__init__.py DESTINATION ${INSTALL_FILE_DESTINATION}/${rpath})
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/_destruct.pyd DESTINATION ${INSTALL_FILE_DESTINATION}/${rpath})
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/destruct.dll DESTINATION ${INSTALL_FILE_DESTINATION}/${rpath})

add_subdirectory(examples)
