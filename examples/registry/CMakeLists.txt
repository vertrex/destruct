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

set(registry_libfiles
  registry.cpp
  regf.cpp
  subkeys.cpp
  values.cpp
  namedkey.cpp
  valuekey.cpp
  registryrpc.cpp
)

add_library(destruct_registry SHARED ${registry_libfiles})
target_link_libraries(destruct_registry destruct)

set(registry_files
registryrpc.cpp)

add_executable(registryrpc ${registry_files})
target_link_libraries(registryrpc destruct destruct_registry)
