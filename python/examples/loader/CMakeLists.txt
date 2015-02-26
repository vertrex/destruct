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

set (python_loader_files
  py_loader.cpp
)


include_directories("../")
include_directories("../../")
include_directories("../../../")
include_directories("../../../examples/loader")

add_library(_loader SHARED ${python_loader_files})

set_target_properties(_loader PROPERTIES PREFIX "")
if (MSVC)
  target_link_libraries(_loader _destruct destruct_loader ${PYTHON_LIBRARY})
else(MSVC)
  target_link_libraries(_loader _destruct destruct_loader python2.7)
endif(MSVC)
