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

set(performance_libfiles
  performance.cpp
)

add_library(destruct_performance SHARED ${performance_libfiles})
target_link_libraries(destruct_performance destruct)

set(performance_files
perf.cpp)

add_executable(perf ${performance_files})
target_link_libraries(perf destruct destruct_performance )
