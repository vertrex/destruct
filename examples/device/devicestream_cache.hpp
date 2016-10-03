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

#ifndef __AGENTCACHE_HH__
#define __AGENTCACHE_HH__

#include <time.h>
#include <map>
#include <queue>
#include <stdint.h>

#include "threading.hpp"
#include "export.hpp"
#include "drealvalue.hpp"

namespace Destruct
{
class DObject;
}

bool operator <(const timespec& lhs, const timespec& rhs);

class CacheSlot
{
public:
  CacheSlot();
  ~CacheSlot();
  uint8_t*      buffer;
  timespec      cacheHits;
};

class BufferCache //XXX et si y a 2 device on fait quoi ?????? il faut un cache par device ou un cache qui peut contenir les pages de plusieur de vices ...  
{
public:
  EXPORT        static BufferCache&     instance();

  //            read(offset, size, buff);
  //            read et pre-cache ds une thread ....

  uint8_t*      find(uint64_t page);
  uint8_t*      insert(uint8_t* buffer, uint64_t page); //USE dbuffer directly ? buff + size so different size can be cached !
  uint32_t      slotCount() const;
  uint32_t      bufferSize() const;
private:
  BufferCache(uint32_t slotCount, uint32_t bufferSize);
  BufferCache(BufferCache const&);
  ~BufferCache();

  uint32_t      __slotCount;
  uint32_t      __bufferSize;

  std::map<uint64_t, CacheSlot*>  __cacheSlots;
  std::map<timespec, uint64_t>    __oldest;

  mutex_def(__mutex); 
};


/**
 *   ReadAhead 
 */
template <typename T> 
class WorkQueue
{ 
public:
  WorkQueue() 
  {
     pthread_mutex_init(&this->__mutex, NULL);
     pthread_cond_init(&this->__condv, NULL);
  }

  ~WorkQueue() 
  {
    pthread_mutex_destroy(&this->__mutex);
    pthread_cond_destroy(&this->__condv);
  }

  void add(T item) 
  {
    pthread_mutex_lock(&this->__mutex);
    this->__queue.push(item);
    pthread_cond_signal(&this->__condv);
    pthread_mutex_unlock(&this->__mutex);
  }

  T remove() 
  {
    pthread_mutex_lock(&this->__mutex);
    while (this->__queue.empty()) 
      pthread_cond_wait(&this->__condv, &this->__mutex);
    
    T item = this->__queue.front();
    this->__queue.pop();
    pthread_mutex_unlock(&this->__mutex);
    return item;
  }

  //int size() 
  //{
  //pthread_mutex_lock(&this->__mutex);
  //int size = this->__queue.size();
  //pthread_mutex_unlock(&this->__mutex);
  //return size;
  //}
private:
  std::queue<T>         __queue;
  pthread_mutex_t      __mutex;
  pthread_cond_t       __condv;
};

#endif
