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
   

  std::stringstream __readStream;
  std::stringstream __writeStream;
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

//class DValue;
//class NetworkStream : public DStream
//{
//public:
  //EXPORT NetworkStream(DStruct* dstruct, DValue const& args);
  //EXPORT NetworkStream(NetworkStream const& copy);
  //EXPORT ~NetworkStream();

  //EXPORT DStream&          operator>>(DStreamString& output);
  //EXPORT int32_t           read(void* buff, int32_t size);
  //EXPORT int32_t           read(DUnicodeString& readValue);
  //EXPORT int32_t           read(uint64_t*  id);

  //EXPORT DStream&          operator<<(DStreamString& input);
  //EXPORT int32_t           write(DUnicodeString const& str); 
  //EXPORT int32_t           write(uint64_t id) ;

  //};

}

#endif
