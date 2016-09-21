#include "dbuffer.hpp"

#include <string>
#include <iostream> 

namespace Destruct
{

/*
 *  RealBuffer
 */

RealBuffer::RealBuffer() : data(NULL), size(0)
{
}

RealBuffer::RealBuffer(int32_t size_) : data(new uint8_t[size_]), size(size_)
{
}

RealBuffer::RealBuffer(uint8_t* data_, int32_t size_) : data(new uint8_t[size_]), size(size_)
{
  memcpy(data, data_, size);
}

RealBuffer::RealBuffer(RealBuffer const& copy) : data(copy.data), size(copy.size)
{
}
 
RealBuffer::~RealBuffer()
{
  delete[] data;
}
 
/**
 *  DBuffer
 */
DBuffer::DBuffer() : __realBuffer(new RealBuffer()) //must not be used
{
}

DBuffer::DBuffer(int32_t size) : __realBuffer(new RealBuffer(size)) //allocate buff for reuse
{
}

DBuffer::DBuffer(uint8_t* data , int32_t size) : __realBuffer(new RealBuffer(data, size))
{
}

DBuffer::DBuffer(DBuffer const& copy) : __realBuffer(copy.__realBuffer)
{
  this->__realBuffer->addRef();
}

DBuffer::~DBuffer()
{
  this->__realBuffer->destroy();
}

uint8_t*        DBuffer::data(void)
{
  return (this->__realBuffer->data);
}

int32_t         DBuffer::size(void)
{
  return (this->__realBuffer->size);
}

}
