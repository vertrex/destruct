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
py_destruct.cpp
py_dstruct.cpp
py_dattribute.cpp
py_dnullobject.cpp
py_dtype.cpp
py_dvalue.cpp
py_dobject.cpp
py_dmethodobject.cpp
#py_dstream.cpp
py_dserialize.cpp
destructmodule.cpp
)

add_library(_destruct SHARED ${pydestruct_files})

set_target_properties(_destruct PROPERTIES PREFIX "")
target_link_libraries(_destruct destruct python2.7)

include_directories("../")

#add_subdirectory(examples)
