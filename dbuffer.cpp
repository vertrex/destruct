#include "dbuffer.hpp"

#include <string>
#include <iostream> 

DBuffer::DBuffer() : __data(0), __size(0)
{
}

DBuffer::DBuffer(uint8_t* data , int32_t size) : __data(data), __size(size)
{
  //std::cout << "DBuffer::DBuffer(data, size)" << std::endl;
  //must be refcounted or new / memcpy data then delete at end !
}

DBuffer::DBuffer(DBuffer const& copy) : __data(copy.__data), __size(copy.__size)
{
  //std::cout << "DBuffer::DBuffer(copy)" << std::endl;
}

DBuffer::~DBuffer()
{
  //std::cout << "DBuffer::~DBuffer" << std::endl;
  //delete[] data or big leak will occur 
}

uint8_t*        DBuffer::data(void)
{
  return (this->__data);
}

int32_t         DBuffer::size(void)
{
  return (this->__size);
}
