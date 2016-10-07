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

#include "dexception.hpp"
#include "devicestream_cache.hpp"

using namespace Destruct;

__int64 GetQPCTime()
{
    LARGE_INTEGER qpcTime;
    QueryPerformanceCounter(&qpcTime);
    return qpcTime.QuadPart;
}

CacheSlot::CacheSlot() : buffer(NULL)
{
}

CacheSlot::~CacheSlot()
{
  delete[] buffer;
}



#ifndef WIN32
bool operator <(const timespec& lhs, const timespec& rhs)
{
  if (lhs.tv_sec == rhs.tv_sec)
    return (lhs.tv_nsec < rhs.tv_nsec);
  else
    return (lhs.tv_sec < rhs.tv_sec);
}
#endif

BufferCache&    BufferCache::instance()
{
  static BufferCache    bufferCache(1000 * 2, 4096 * 10);
  return bufferCache;
}

BufferCache::BufferCache(uint32_t slotCount, uint32_t bufferSize) : __slotCount(slotCount), __bufferSize(bufferSize)
{
  mutex_init(&this->__mutex);
}

BufferCache::~BufferCache()
{
};

uint32_t        BufferCache::bufferSize(void) const
{
  return (this->__bufferSize);
}

uint32_t        BufferCache::slotCount(void) const
{
  return (this->__slotCount);
}

uint32_t hits = 0;
uint64_t miss = 0;

uint8_t*        BufferCache::find(uint64_t page)
{
  mutex_lock(&__mutex);
  std::map<uint64_t, CacheSlot*>::iterator it  = this->__cacheSlots.find(page);

  if (it != this->__cacheSlots.end())
  { 
     CacheSlot* cacheSlot = it->second;

#ifdef WIN32
  std::map<uint64_t, uint64_t>::iterator oldestit = this->__oldest.find(cacheSlot->cacheHits); 
#else
  std::map<timespec, uint64_t>::iterator oldestit = this->__oldest.find(cacheSlot->cacheHits); 
#endif
     //std::map<timespec, uint64_t>::iterator oldestit = this->__oldest.find(cacheSlot->cacheHits);
	 if (oldestit != this->__oldest.end())
       this->__oldest.erase(oldestit);
	 else //time is not precise enough
		 std::cout << "Error cache find can't destroy page " << page << " found int oldest list " << cacheSlot->cacheHits << std::endl;

#ifdef WIN32
    // cacheSlot->cacheHits = GetTickCount64();
	 cacheSlot->cacheHits = GetQPCTime();
#else
	 clock_gettime(CLOCK_MONOTONIC_RAW, &(cacheSlot->cacheHits));
#endif
     this->__oldest[cacheSlot->cacheHits] = page;
     mutex_unlock(&__mutex);
     return (cacheSlot->buffer);
  }
  mutex_unlock(&__mutex);
  return (NULL);
};

uint8_t*        BufferCache::insert(uint8_t* buffer, uint64_t pageNumber)
{
  mutex_lock(&__mutex);
  if (__cacheSlots.size() < __slotCount)
  {
    CacheSlot* newSlot = new CacheSlot;
    newSlot->buffer = new uint8_t[this->__bufferSize];
    memcpy(newSlot->buffer, buffer, this->__bufferSize);

	#ifdef WIN32
     //newSlot->cacheHits = GetTickCount64();
	 	newSlot->cacheHits = GetQPCTime();
	#else
	 clock_gettime(CLOCK_MONOTONIC_RAW, &(newSlot->cacheHits));
	#endif

    this->__cacheSlots[pageNumber] = newSlot;
    this->__oldest[newSlot->cacheHits] = pageNumber;

    mutex_unlock(&__mutex);
    return (newSlot->buffer);
  } 

#ifdef WIN32
  std::map<uint64_t, uint64_t>::iterator oldestit = this->__oldest.begin(); 
#else
  std::map<timespec, uint64_t>::iterator oldestit = this->__oldest.begin(); 
#endif
  std::map<uint64_t, CacheSlot*>::iterator slotit = this->__cacheSlots.find(oldestit->second);

  if (slotit == this->__cacheSlots.end())
  {
    std::cout << "page " << oldestit->second << " not found in __cacheSlots of size " << this->__cacheSlots.size() << " oldest list size " << __oldest.size() << std::endl;
  }
  else
  {
          //std::cout << "ERASING " << std::endl;
    this->__cacheSlots.erase(slotit);
    this->__oldest.erase(oldestit);
  }

  CacheSlot* newSlot = new CacheSlot;
  newSlot->buffer = new uint8_t[this->__bufferSize];
  memcpy(newSlot->buffer, buffer, this->__bufferSize);
   #ifdef WIN32
    // newSlot->cacheHits = GetTickCount64();
     newSlot->cacheHits = GetQPCTime();
   #else
	 clock_gettime(CLOCK_MONOTONIC_RAW, &(newSlot->cacheHits));
   #endif

  this->__cacheSlots[pageNumber] = newSlot;
  this->__oldest[newSlot->cacheHits] = pageNumber;

  mutex_unlock(&__mutex);
  return (newSlot->buffer); //can be destroyed before being copied !!!! as it's unlock ! better copy it 
};
