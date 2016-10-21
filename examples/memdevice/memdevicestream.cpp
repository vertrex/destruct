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

#include <string>
#include <windows.h>
#include <Shlwapi.h>


#include "dstructs.hpp"
#include "memdevicedriver.hpp"
#include "memdevicestream.hpp"

using namespace Destruct;

/**
 *  DeviceStream Windows 
 */
MemoryDeviceStream::MemoryDeviceStream(DStruct* dstruct, DValue const& args) : DCppObject<MemoryDeviceStream>(dstruct, args)
{
  this->init();
  this->__size = 0;

  HANDLE handle = CreateFile("\\\\.\\pmem", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
  if (handle != INVALID_HANDLE_VALUE)
  {
     DWORD ioctlSize;
     uint64_t memorySize;
     struct PmemMemoryInfo info;          
     ZeroMemory(&info, sizeof(info));

     if (DeviceIoControl(handle, PMEM_INFO_IOCTRL, NULL, 0, (char *)&info, sizeof(info), &ioctlSize, NULL) == TRUE)
     {
       for (__int64 i = 0; i < info.NumberOfRuns.QuadPart; i++) 
         memorySize = info.Run[i].start + info.Run[i].length;
       this->__size = memorySize;
     }
	 else
       throw DException("Can't determine memory device size");
  }
  else 
    throw DException("Can't open memory device");
}

MemoryDeviceStream::~MemoryDeviceStream()
{
}

DBuffer MemoryDeviceStream::read(DValue const& args)
{
  DInt64 size = args;
  DWORD readed;

  DBuffer dbuffer((int32_t)size);
 
  ReadFile(this->__handle, (void*)(dbuffer.data()), (DWORD)size,  &readed ,0);
  return (dbuffer);
}

DUInt64 MemoryDeviceStream::size(void)
{
  return (this->__size);
}

void    MemoryDeviceStream::seek(DValue const& args)
{
  DUInt64 offset = args;
  if (offset < this->__size)
  {
	LARGE_INTEGER newOffset;
	newOffset.QuadPart = offset;
    LARGE_INTEGER newOffsetRes;
    SetFilePointerEx(this->__handle, newOffset, &newOffsetRes, 0);
  }
  else
   throw DException("DeviceStream::seek can't seek");
}

DUInt64 MemoryDeviceStream::tell(void)
{
 // return (this->__offset);
  return (0); //XXX 
}

void    MemoryDeviceStream::close(void)
{
	CloseHandle(this->__handle);
}
