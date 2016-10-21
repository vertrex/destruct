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
include_directories("../rpc")


set(device_libfiles
  memdevicedriver.cpp
  memdevicestream.cpp
  )

add_library(destruct_device SHARED ${device_libfiles})
target_link_libraries(destruct_device destruct)
