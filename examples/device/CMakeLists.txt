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
  device.cpp
  )

if (UNIX)
  set (device_libfiles ${device_libfiles}
  devicelist_unix.cpp
  devicestream_unix.cpp
  )
else (UNIX)
  set (device_libfiles ${device_libfiles}
  devicelist_windows.cpp
  devicestream_windows.cpp
  devicestream_cache.cpp
  )
endif (UNIX)


add_library(destruct_device SHARED ${device_libfiles})
if (UNIX)
  target_link_libraries(destruct_device destruct destruct_rpczmq udev)
else (UNIX)
  target_link_libraries(destruct_device destruct destruct_rpczmq)
endif (UNIX)

#set(device_files
#devicerpc.cpp)

#add_executable(devicerpc ${device_files})
#target_link_libraries(devicerpc destruct destruct_rpczmq destruct_device) #can import now
