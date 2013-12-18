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

set (python_destruct_test_files
  py_destruct_test.cpp
)

add_subdirectory(output)

include_directories("../")
include_directories("../../")
include_directories("../../../")
include_directories("../../../examples/dtest")

add_library(_dtest SHARED ${python_destruct_test_files})

set_target_properties(_dtest PROPERTIES PREFIX "")
target_link_libraries(_dtest _destruct destruct_test python2.7)
