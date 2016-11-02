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
include_directories("../")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/modules)

add_subdirectory(loader)
add_subdirectory(dtest)
add_subdirectory(db)
add_subdirectory(refcount)
add_subdirectory(inheritance)
add_subdirectory(threading)
add_subdirectory(xmacro)
#add_subdirectory(filetype)

add_subdirectory(rpc)
add_subdirectory(rpczmq)
#add_subdirectory(birpczmq)
add_subdirectory(registry)
add_subdirectory(device)
add_subdirectory(performance)

if (WIN32)
add_subdirectory(memdevice)
endif(WIN32)

set (test_files
test.cpp
)

add_executable(test_destruct ${test_files})
target_link_libraries(test_destruct destruct destruct_test)
