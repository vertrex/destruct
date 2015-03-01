# DFF -- An Open Source Digital Forensics Framework
# Copyright (C) 2011 ArxSys
#
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

include_directories("../../")

set(threading_libfiles
  workerpool.cpp
  threadsafeobject.cpp
)

add_library(dthreading SHARED ${threading_libfiles})
if (MSVC)
target_link_libraries(dthreading destruct)
else (MSVC)
target_link_libraries(dthreading pthread destruct)
endif(MSVC)

set(threading_files
threading.cpp)

add_executable(threading ${threading_files})
target_link_libraries(threading dthreading)
