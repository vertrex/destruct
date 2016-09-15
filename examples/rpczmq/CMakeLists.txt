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

include_directories("../../")

set (rpczmq_libfiles
fsobject.cpp
networkstream.cpp
serializerpc.cpp
server.cpp
serverobject.cpp
serverfunctionobject.cpp
objectmanager.cpp
client.cpp
clientobject.cpp
clientfunctionobject.cpp
)

add_library(destruct_rpczmq SHARED ${rpczmq_libfiles})
if (MSVC)
  target_link_libraries(destruct_rpczmq destruct Ws2_32.lib ${LIBZMQ_LIBRARY}) 
else (MSVC)
  target_link_libraries(destruct_rpczmq destruct zmq)
endif (MSVC)

set (rpc_files
rpc.cpp)

add_executable(rpczmq ${rpc_files})
target_link_libraries(rpczmq destruct_rpczmq)
