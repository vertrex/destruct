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

add_subdirectory(dtest)
add_subdirectory(rpc)
add_subdirectory(db)
#add_subdirectory(refcount)

include_directories("../")
set (test_files
test.cpp
)

add_executable(test ${test_files})
target_link_libraries(test destruct destruct_test)
