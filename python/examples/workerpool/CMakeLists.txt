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

set (python_workerpool_files
  py_workerpool.cpp
)

include_directories("../")
include_directories("../../")
include_directories("../../../")

add_library(_workerpool SHARED ${python_workerpool_files})
configure_file(workerpool.py workerpool.py COPYONLY)

set_target_properties(_workerpool PROPERTIES PREFIX "")
if (MSVC)
  target_link_libraries(_workerpool _destruct dthreading ${PYTHON_LIBRARY})
else(MSVC)
  target_link_libraries(_workerpool _destruct dthreading python2.7)
endif(MSVC)
