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
 *  DeviceBuffer
 */


DeviceBuffer::DeviceBuffer(HANDLE hndl, uint32_t size,  uint32_t bps, uint64_t devSize) : __handle(hndl), __offset(0), __BPS(bps), __currentSize(0), __devSize(devSize), __size(size * bps) 
{
  std::cout << "cache size " << this->__size << std::endl;
  this->__buffer = (uint8_t *)malloc(this->__size);
  this->fillBuff(0);
}

DeviceBuffer::~DeviceBuffer()
{
   CloseHandle(this->__handle);
   free(this->__buffer);
}

void DeviceBuffer::fillBuff(uint64_t offset)
{
  //std::cout << "DeviceBuffer::fillBuff" << std::endl;
  LARGE_INTEGER sizeConv;
  LARGE_INTEGER newOffset;
	
  if (this->__offset > this->__devSize)
  {
    this->__currentSize = 0;
    return;
  }
  this->__offset = ((offset / this->__BPS) * this->__BPS);
  sizeConv.QuadPart = this->__offset;
  SetFilePointerEx(this->__handle, sizeConv, &newOffset, 0);
  DWORD gsize;
  if (this->__offset + this->__size > this->__devSize)
    gsize = (DWORD)(this->__devSize - this->__offset);
  else
    gsize = this->__size;
  ReadFile(this->__handle, (void*)(this->__buffer), gsize,  &(this->__currentSize) ,0);
}

uint32_t	DeviceBuffer::getData(void *buff, uint32_t size, uint64_t offset)
{
 // if (size 

  if ((offset < this->__offset) || (offset > this->__offset + this->__currentSize) 
      ||(offset + size > this->__offset + this->__currentSize))
  {
    this->fillBuff(offset);
  }

  uint64_t leak = offset - this->__offset;
  if (size > this->__currentSize - leak)
    size = (uint32_t)(this->__currentSize - leak);
  memcpy(buff, (((char*)this->__buffer) + leak), size);

  return (size);
}

  uint32_t			DeviceBuffer::readLarge(void* largeBuff, uint32_t size, uint64_t offset)
  {
	 // std::cout << "Read large " << size << std::endl;
    uint64_t  newSize;
	LARGE_INTEGER newOffset;	
	LARGE_INTEGER newOffsetRes;

	if (size % 512)
       newSize = ((size/512)+1) * 512;
	else
		newSize = size;

	if (offset % 512)
       newOffset.QuadPart = (offset/512) * 512;
	else
	  newOffset.QuadPart = offset;

	uint64_t leak = offset - newOffset.QuadPart;

	
 /* if (this->__offset > this->__devSize)
  {
    this->__currentSize = 0;
    return;
  }*/

//	if (newOffset.QuadPart != this->__offset)
  SetFilePointerEx(this->__handle, newOffset, &newOffsetRes, 0);
  DWORD readed;

  uint8_t* newBuffer = new uint8_t[newSize];
  ReadFile(this->__handle, (void*)(newBuffer), (DWORD)newSize,  &readed ,0);

  memcpy((void*)largeBuff, (void*)((uint8_t*)newBuffer+ leak), size);
  delete[] newBuffer;
  //this->__offset += size;

   return (size);
  }

/**
 *  DeviceStream Windows 
 */
DeviceStream::DeviceStream(DStruct* dstruct, DValue const& args) : DCppObject<DeviceStream>(dstruct, args), __offset(0)
{
  this->init();
  this->__size = ((DObject*)args)->getValue("size");
  this->__path = ((DObject*)args)->getValue("path").get<DUnicodeString>();

 int hnd = (int)CreateFile(this->__path.c_str(), GENERIC_READ, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
 if (((HANDLE)hnd) == INVALID_HANDLE_VALUE)
    throw DException("Can't open device");

 //this->__deviceBuffer = new DeviceBuffer((HANDLE)hnd, 100 * sizeof(uint8_t), 4096, this->__size);
  this->__deviceBuffer = new DeviceBuffer((HANDLE)hnd, 2 , 4096, this->__size);
}

DeviceStream::~DeviceStream()
{
	delete this->__deviceBuffer;
}

DBuffer DeviceStream::read(DValue const& args)
{
  uint32_t				origSize = (int32_t)args.get<DInt64>();
  uint32_t              readed;
  uint32_t              aReaded = 0;

  DBuffer dbuffer(origSize);

 // if (origSize > 4096 * 2) // cache SIZE avoir ici
//  {
     this->__deviceBuffer->readLarge((void*)dbuffer.data(), origSize, this->__offset);
	 this->__offset += ((uint64_t)readed);
     return dbuffer;
//  }


  //std::cout << "DeviceStream::read " << origSize << std::endl;
  while (aReaded < origSize)
  {
    readed = this->__deviceBuffer->getData(((uint8_t *)dbuffer.data() + aReaded), origSize - aReaded, this->__offset);
    this->__offset += ((uint64_t)readed);
    aReaded += readed;
    if (this->__offset > this->__size)
    {
      this->__offset = this->__size;
	  std::cout << "DeviceStream::read this->__offset > this->__size " << std::endl;
      return dbuffer;
      //return (aReaded);
    }
/*    if (readed < this->__deviceBuffer->__size)
	{
	  std::cout << "readed < this->__deviceBuffer->__size readed " << readed << " aReaded  " << aReaded <<  " origSize "  << origSize << std::endl;
      return dbuffer;
	}*/
            //return (aReaded); 
  }
  if (aReaded != origSize)
	 std::cout << "aReaded != origSize aReaded "  << aReaded << " origSize " << origSize << std::endl;
  return dbuffer;
  //return aReaded;
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
  delete this->__deviceBuffer;
  this->__deviceBuffer = NULL;
}
