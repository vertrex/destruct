#ifndef __NETWORKSTREAM__
#define __NETWORKSTREAM__

#include <sstream>
#include <iostream>
#include <string.h>

#include "protocol/dcppobject.hpp"
//#include "destruct.hpp"
//#include "protocol/dstream.hpp"

namespace Destruct
{

class StreamBuffer
{
public:
	StreamBuffer() : buffer(new char[4096]), currentRead(0), currentWrite(0), __maxSize(4096)
	{
	}

	//copy constructor

	void	write(const char* inbuff, uint32_t size)
	{
	   if (currentWrite + size > this->__maxSize) //maxSize > realloc until some size ?
	   {
	     std::cout << "Write overflow " << currentWrite + size << " > " << this->__maxSize << std::endl;
		 throw DException("StreamBuffer::WriteOverflow");
	   }
	   memcpy(this->buffer + currentWrite, inbuff, size);
	   currentWrite += size;
	}

	void	read(char* inbuff, uint32_t size)
	{
	   if (currentRead + size > this->__maxSize)
	   {
		 std::cout << "Read overflow " << currentRead + size << " > " << this->__maxSize << std::endl;
	     throw DException("StreamBuffer::ReadOverflow");
	   }
	   memcpy(inbuff, this->buffer + currentRead, size);
	   currentRead += size;
	}

	uint32_t	toRead(void)
	{
	   return (this->currentWrite - this->currentRead);
	}

	void		reset(void)
	{
	  currentRead = 0;
	  currentWrite = 0;
	}
private:
	char*		buffer;
	uint32_t	currentRead;
	uint32_t	currentWrite;
	uint32_t	__maxSize;
};

class NetworkStream : public DCppObject<NetworkStream>
{
public:
  NetworkStream(DStruct* dstruct, DValue const& args);
  NetworkStream(const NetworkStream& copy);

  DBuffer  read(DValue const& args); 
  DInt64   write(DValue const& args);
  void     flush(void) ; 
//  DValue  seek(DValue const& args);
  
  ~NetworkStream();
private:
  int32_t           __socket;
//int32_t           __send(void* buff, int32_t size); 
//int32_t           __recv(void* buff, int32_t size);
   
  StreamBuffer		__readStream;
  StreamBuffer		__writeStream;
  //std::stringstream __readStream;
  //std::stringstream __writeStream;
public:
  RealValue<DFunctionObject* > _read, _write, _flush;

  static size_t ownAttributeCount()
  {
    return (3);
  }

  static DAttribute* ownAttributeBegin()
  {
    static DAttribute  attributes[] = 
    {
       DAttribute(DType::DBufferType, "read",  DType::DInt64Type), 
       DAttribute(DType::DInt64Type,  "write", DType::DBufferType),
       DAttribute(DType::DNoneType, "flush", DType::DNoneType),
    };
    return (attributes);
  }

  static DPointer<NetworkStream>* memberBegin()
  {
    static DPointer<NetworkStream> memberPointer[] = 
    {
       DPointer<NetworkStream>(&NetworkStream::_read, &NetworkStream::read),
       DPointer<NetworkStream>(&NetworkStream::_write, &NetworkStream::write),
       DPointer<NetworkStream>(&NetworkStream::_flush, &NetworkStream::flush),
    };
    return (memberPointer);
  }

  static DAttribute* ownAttributeEnd()
  {
    return (ownAttributeBegin() + ownAttributeCount());
  }

  static DPointer<NetworkStream>*  memberEnd()
  {
    return (memberBegin() + ownAttributeCount());
  } 
};

}

#endif
