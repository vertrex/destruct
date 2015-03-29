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

add_subdirectory(loader)
add_subdirectory(dtest)
add_subdirectory(db)
add_subdirectory(refcount)
add_subdirectory(inheritance)
add_subdirectory(threading)
add_subdirectory(filetype)

add_subdirectory(rpc)
if (UNIX)
add_subdirectory(registry)
endif (UNIX)

set (test_files
test.cpp
)

add_executable(test_destruct ${test_files})
target_link_libraries(test_destruct destruct destruct_test)
