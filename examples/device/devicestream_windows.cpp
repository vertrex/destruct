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
/*
ReadWork::ReadWork(HANDLE ahandle,  uint64_t apage) : handle(ahandle), page(apage)
{
}

ThreadResult   CacheWorker(ThreadData rq)
{
  WorkQueue<ReadWork*>* queue = static_cast<WorkQueue<ReadWork*>* >(rq);
  BufferCache& cache = BufferCache::instance();
  uint64_t   pageSize = cache.bufferSize();
  uint8_t*	 buffer = new uint8_t[pageSize];

  uint64_t  lastSeek = 0;
  while (1)
  {
    ReadWork* work = queue->remove();
    for (uint64_t i = 0; i < 4; ++i)
    {
      if (cache.find(work->page + i) == NULL) 
      {
          uint64_t toSeek = (work->page + i) * pageSize;
		  
          if (toSeek != lastSeek)
          {
			LARGE_INTEGER newOffset;
			newOffset.QuadPart = toSeek;
			LARGE_INTEGER newOffsetRes;
			SetFilePointerEx(work->handle, newOffset, &newOffsetRes, 0);
			if (newOffsetRes.QuadPart != newOffset.QuadPart)
			  std::cout << "Worker can't seek to block " << std::endl;//comment on previens leclient ????????
            lastSeek = newOffsetRes.QuadPart;
          }
		  DWORD readed;
		  ReadFile(work->handle, (void*)(buffer), (DWORD)pageSize,  &readed ,0);
		  if (readed != pageSize)
		    std::cout << "Can't read all page " << std::endl;
          lastSeek += pageSize;
<<<<<<< HEAD
          cache.insert(buffer, (work->page + i));
=======
          cache.insert(dbuffer.data(), (work->page + i));
        }
        catch (Destruct::DException const& e)
        {
          std::cout << "error reading " << e.error() << std::endl; //how to put error ? cache insert workpage, error ?
        }
>>>>>>> 539338eb11ad9b1064807922329f7e582c13752e
      }
    }
    delete work;
  }
}

*/
/**
 *  DeviceStream Windows 
 */
DeviceStream::DeviceStream(DStruct* dstruct, DValue const& args) : DCppObject<DeviceStream>(dstruct, args), __offset(0), __lastOffset(0), __cache(BufferCache::instance()), __cacheBufferSize(BufferCache::instance().bufferSize()) //XXX un cache pour tous les devices 
{
  this->init();
//  std::cout << "Device stream open" << std::endl;
  this->__size = ((DObject*)args)->getValue("size");
  this->__path = ((DObject*)args)->getValue("path").get<DUnicodeString>();

  this->__handle = CreateFile(this->__path.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
  if (this->__handle == INVALID_HANDLE_VALUE)
    throw DException("DeviceStream can't open device " + this->__path);

 // this->__readQueue = new WorkQueue<ReadWork*>();
 // createThread(CacheWorker, (void*)this->__readQueue, this->__workerThread);
}

DeviceStream::~DeviceStream()
{
}

DBuffer DeviceStream::oldRead(DValue const& args)
{
	

 DInt64        dsize = args;
  uint64_t  newSize;
  LARGE_INTEGER newOffset;      
  LARGE_INTEGER newOffsetRes;

  if (dsize % 4096)
    newSize = ((dsize/4096)+1) * 4096;
  else
    newSize = dsize;

  if (this->__offset % 4096)
    newOffset.QuadPart = (this->__offset/4096) * 4096;
  else
    newOffset.QuadPart = this->__offset;
  uint64_t leak = this->__offset - newOffset.QuadPart;
  if (leak)
	  newSize += 4096; //fix temporaire car c debeux ex : on lit de 10485760 mais on a seek donc le newSize va pas rajouter un block mais comme y a un leak fo en rajouter 1 donc la y a des cas ou ca v en rajouter 2
 /* if (this->__offset > this->__devSize)
  {
    this->__currentSize = 0;
    return;
  }*/
//  if (this->__lastOffset != this->__offset)
 // {
    SetFilePointerEx(this->__handle, newOffset, &newOffsetRes, 0);
    if (newOffsetRes.QuadPart != newOffset.QuadPart)
     std::cout << "Can't seek to " << newOffset.QuadPart << " seek to " << newOffsetRes.QuadPart << std::endl;
        this->__lastOffset = newOffsetRes.QuadPart;
        this->__offset = newOffsetRes.QuadPart;
  //}
  DWORD readed;

  uint8_t* newBuffer = new uint8_t[newSize]; //find a copy on write possibility with dbuffer would avoid copy 10mo buffer !
  ReadFile(this->__handle, (void*)(newBuffer), (DWORD)newSize,  &readed ,0);
  if (readed != newSize)
    std::cout << "Can't read " << newSize << " read only " << readed << std::endl;
  if ((DWORD)dsize < readed)
	  std::cout << "Must return " << dsize << " read only " << readed << std::endl;
  this->__offset += readed;
//  this->__lastOffset += readed; //mouais ca devrait pas etre relative a size plutot ???

  //std::cout << "new size" << newSize << "leak " << leak  << " copy size " << dsize << std::endl;
  DBuffer dbuffer(((uint8_t*)newBuffer+ leak) , (int32_t)dsize);
  delete[] newBuffer;

  return (dbuffer);

}

DBuffer DeviceStream::read(DValue const& args)
{
	return (this->oldRead(args));
	
	//return this->oldRead(args);
	/*
  DInt64	size = args;
  DBuffer dbuffer((int32_t)size);

  uint64_t readed = 0;

  if (size > this->__cacheBufferSize)
	return (this->oldRead(args));

  while (readed < (uint64_t) size)
  {
	  uint64_t page = this->__offset / this->__cacheBufferSize;
	  uint64_t pageStartOffset = this->__offset % this->__cacheBufferSize;
	 
	  uint8_t* pageBuff = this->__cache.find(page);
	  if (pageBuff == NULL)
	  {
		 this->__readQueue->add(new ReadWork(this->__handle, page));
		 while (pageBuff == NULL)
		   pageBuff = this->__cache.find(page);
	  }
	  uint64_t sizeToRead = this->__cacheBufferSize - pageStartOffset;
	  if (sizeToRead > (size - readed))
		sizeToRead = size - readed;
	  memcpy((void*)(dbuffer.data() + readed), (void*)(pageBuff + pageStartOffset), sizeToRead);
	  readed += sizeToRead;
	  //this->__offset += sizeToRead;
  }
  this->__offset += size;
  
  return (dbuffer);*/
}

DUInt64 DeviceStream::size(void)
{
  return (this->__size);
}

void    DeviceStream::seek(DValue const& args)
{
  DUInt64 newOffset = args;
  if (newOffset < this->__size)
    this->__offset = newOffset;
  else
	 throw DException("DeviceStream::seek can't seek"); //to  this->__offset);
 return ;// (n);
}

DUInt64 DeviceStream::tell(void)
{
  return (this->__offset);
}

void    DeviceStream::close(void)
{
}
