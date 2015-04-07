#include "dbuffer.hpp"

DBuffer::DBuffer() : __data(0), __size(0)
{
}

DBuffer::DBuffer(uint8_t* data , int32_t size) : __data(data), __size(size)
{
}

DBuffer::DBuffer(DBuffer const& copy) : __data(copy.__data), __size(copy.__size)
{
}

uint8_t*        DBuffer::data(void)
{
  return (this->__data);
}

int32_t         DBuffer::size(void)
{
  return (this->__size);
}
