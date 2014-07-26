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

set (rpc_files
fsobject.cpp
rpcobject.cpp
networkstream.cpp
server.cpp
client.cpp
rpc.cpp
serializerpc.cpp
rpcserver.cpp)

add_executable(rpc ${rpc_files})
target_link_libraries(rpc destruct)
