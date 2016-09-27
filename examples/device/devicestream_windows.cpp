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
#include "devicestream_windows.hpp"

using namespace Destruct;


/**
 *  DeviceStream Windows 
 */
DeviceStream::DeviceStream(DStruct* dstruct, DValue const& args) : DCppObject<DeviceStream>(dstruct, args), __offset(0), __lastOffset(0)
{
  this->init();
  this->__size = ((DObject*)args)->getValue("size");
  this->__path = ((DObject*)args)->getValue("path").get<DUnicodeString>();

 this->__handle = CreateFile(this->__path.c_str(), GENERIC_READ, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
 if (this->__handle == INVALID_HANDLE_VALUE)
    throw DException("Can't open device");

}

DeviceStream::~DeviceStream()
{
}

DBuffer DeviceStream::read(DValue const& args)
{
  DInt64	size = args;
  uint64_t  newSize;
  LARGE_INTEGER newOffset;	
  LARGE_INTEGER newOffsetRes;

  if (size % 4096)
    newSize = ((size/4096)+1) * 4096;
  else
    newSize = size;

  if (this->__offset % 4096)
    newOffset.QuadPart = (this->__offset/4096) * 4096;
  else
    newOffset.QuadPart = this->__offset;
  uint64_t leak = this->__offset - newOffset.QuadPart;

 /* if (this->__offset > this->__devSize)
  {
    this->__currentSize = 0;
    return;
  }*/
  if (this->__lastOffset != this->__offset)
  {
    SetFilePointerEx(this->__handle, newOffset, &newOffsetRes, 0);
    if (newOffsetRes.QuadPart != newOffset.QuadPart)
     std::cout << "Can't seek to " << newOffset.QuadPart << " seek to " << newOffsetRes.QuadPart << std::endl;
	this->__lastOffset = newOffsetRes.QuadPart;
	this->__offset = newOffsetRes.QuadPart;
  }
  DWORD readed;

  uint8_t* newBuffer = new uint8_t[newSize];
  ReadFile(this->__handle, (void*)(newBuffer), (DWORD)newSize,  &readed ,0);
  if (readed != newSize)
    std::cout << "Can't read " << newSize << " read only " << readed << std::endl;
  this->__offset += readed;
  this->__lastOffset += readed; //mouais ca devrait pas etre relative a size plutot ???

  DBuffer dbuffer(((uint8_t*)newBuffer+ leak) , (int32_t)size);
  delete[] newBuffer;

  return (dbuffer);
}

DUInt64 DeviceStream::size(void)
{
  return (this->__size);
}

void    DeviceStream::seek(DValue const& args)
{
  if (this->__offset + args.get<DUInt64>() < this->__size)
    this->__offset = args.get<DUInt64>();
 return ;// (n);
}

DUInt64 DeviceStream::tell(void)
{
  return (this->__offset);
}

void    DeviceStream::close(void)
{
}
