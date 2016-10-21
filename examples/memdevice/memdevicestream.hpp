/*
 * DFF -- An Open Source Digital Forensics Framework
 * Copyright (C) 2009-2011 ArxSys
 * This program is free software, distributed under the terms of
 * the GNU General Public License Version 2. See the LICENSE file
 * at the top of the source tree.
 *  
 * See http: *www.digital-forensic.org for more information about this
 * project. Please do not directly contact any of the maintainers of
 * DFF for assistance; the project provides a web site, mailing lists
 * and IRC channels for your use.
 * 
 * Author(s):
 *  Solal Jacob <sja@digital-forensic.org>
 */

#ifndef __DEVICESTREAM_WINDOWS_HPP__
#define __DEVICESTREAM_WINDOWS_HPP__

#include "memdevice_common.hpp"

#pragma comment(lib, "advapi32.lib")
#include <windows.h>
#include <stdio.h>
#include <aclapi.h>

using namespace Destruct;

class MemoryDeviceStream : public DCppObject<MemoryDeviceStream>
{
public:
  MemoryDeviceStream(DStruct* dstruct, DValue const& args);

  DBuffer       read(DValue const& args); 
  DUInt64       size(void);
  void          seek(DValue const& args);
  DUInt64       tell(void);
  void          close(void);

  attributeCount(MemoryDeviceStream, 5)

  attributeList(
                function(DBuffer, read, DInt64)
                function(DUInt64, size, DNone)
                function(DNone, seek, DUInt64)
                function(DUInt64, tell, DNone)
                function(DNone, close, DNone)
               )

  memberList(MemoryDeviceStream, 
              method(MemoryDeviceStream, read)
              method(MemoryDeviceStream, size)
              method(MemoryDeviceStream, seek)
              method(MemoryDeviceStream, tell)
              method(MemoryDeviceStream, close)
            )

protected:
  ~MemoryDeviceStream();
private:
  HANDLE                        __handle;
  uint64_t                      __size;
  RealValue<DFunctionObject*>   _read, _size, _seek, _tell, _close;
};

#endif
