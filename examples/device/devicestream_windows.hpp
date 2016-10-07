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

#include "device_common.hpp"
#include "devicestream_cache.hpp"

#pragma comment(lib, "advapi32.lib")
#include <windows.h>
#include <stdio.h>
#include <aclapi.h>

using namespace Destruct;


/*
class ReadWork 
{
public:
  //ReadWork(Destruct::DObject* astream,  uint64_t apage);
  ReadWork(HANDLE ahandle,  uint64_t apage);
  HANDLE				  handle;
  uint64_t                page;
};

EXPORT ThreadResult   CacheWorker(ThreadData rq);
*/
class DeviceStream : public DCppObject<DeviceStream>
{
public:
  DeviceStream(DStruct* dstruct, DValue const& args);

  DBuffer       read(DValue const& args); 
  DBuffer       oldRead(DValue const& args);
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
  DUInt64		        __offset;
  DUInt64		        __lastOffset;
  HANDLE		        __handle;

  BufferCache&                  __cache;
  const uint64_t                __cacheBufferSize;
//  uint64_t                      __lastOffset;
//  WorkQueue<ReadWork*>*         __readQueue;
 // ThreadStruct                  __workerThread;
};

#endif
