#ifndef __NETWORKSTREAM__
#define __NETWORKSTREAM__

#include <sstream>
#include <iostream>
#include <string.h>

#include "protocol/dcppobject.hpp"

namespace Destruct
{

class StreamBuffer
{
public:
  StreamBuffer(); 
  StreamBuffer(const StreamBuffer& copy);
  ~StreamBuffer();

  void          write(const char* inbuff, uint32_t size);
  void          read(char* inbuff, uint32_t size);
  uint32_t	toRead(void);
  void          reset(void);

private:
  char*		__buffer;
  uint32_t	__currentRead;
  uint32_t	__currentWrite;
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
//DValue  seek(DValue const& args);
 
protected: 
  ~NetworkStream();
private:
  int32_t       __socket;
   
  StreamBuffer	__readStream;
  StreamBuffer	__writeStream;
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
