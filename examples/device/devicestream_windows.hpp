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

#ifndef __DEVICESTREAM_UNIX_HPP__
#define __DEVICESTREAM_UNIX_HPP__

#include "device_common.hpp"

using namespace Destruct;

#pragma comment(lib, "advapi32.lib")
#include <windows.h>
#include <stdio.h>
#include <aclapi.h>

class DeviceBuffer
{
private:
  HANDLE			__handle;
  uint8_t*			__buffer;
  uint64_t			__offset;
  uint32_t			__BPS;
  DWORD				__currentSize;
  uint64_t			__devSize;
  void				fillBuff(uint64_t offset);
public:
  DeviceBuffer(HANDLE handle, uint32_t size, uint32_t BPS,  uint64_t DevSize);
  ~DeviceBuffer();
  uint32_t			__size;
  uint32_t			getData(void* buff, uint32_t size, uint64_t offset);
};

class DeviceStream : public DCppObject<DeviceStream>
{
public:
  DeviceStream(DStruct* dstruct, DValue const& args);

  DBuffer       read(DValue const& args);
  DUInt64       size(void);
  void          seek(DValue const& args);
  DUInt64       tell(void);
  void          close(void);

  attributeCount(DeviceStream, 5)

  attributeList(
                function(DBuffer, read, DInt64)
                function(DUInt64, size, DNone)
                function(DNone, seek, DUInt64)
                function(DUInt64, tell, DNone)
                function(DNone, close, DNone)
               )

  memberList(DeviceStream, 
              method(DeviceStream, read)
              method(DeviceStream, size)
              method(DeviceStream, seek)
              method(DeviceStream, tell)
              method(DeviceStream, close)
            )

protected:
  ~DeviceStream();
private:
  RealValue<DFunctionObject*>   _read, _size, _seek, _tell, _close;
  DUnicodeString                __path;
  DUInt64                       __size;
  DUInt64			__offset;
  DeviceBuffer*                 __deviceBuffer;
};

#endif
