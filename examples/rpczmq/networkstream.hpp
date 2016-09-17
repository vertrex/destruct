#ifndef __NETWORKSTREAM__
#define __NETWORKSTREAM__

#include <sstream>
#include <iostream>
#include <string.h>

#include "protocol/dcppobject.hpp"

namespace Destruct
{

class NetworkStream : public DCppObject<NetworkStream>
{
public:
  NetworkStream(DStruct* dstruct, DValue const& args);
  NetworkStream(const NetworkStream& copy);

  DBuffer  read(DValue const& args); 
  DInt64   write(DValue const& args);
  DInt8    request(void);
  void     reply(DValue const& args);

  void*         __socket; //XXX should be private but need to copy by cast...
  void*         __context;
protected: 
  ~NetworkStream();
public:
  RealValue<DFunctionObject* > _read, _write, _request, _reply;

  static size_t ownAttributeCount()
  {
    return (4);
  }

  static DAttribute* ownAttributeBegin()
  {
    static DAttribute  attributes[] = 
    {
       DAttribute(DType::DBufferType, "read",  DType::DInt64Type), 
       DAttribute(DType::DInt64Type,  "write", DType::DBufferType),
       DAttribute(DType::DInt8Type,  "request", DType::DNoneType),
       DAttribute(DType::DNoneType,  "reply", DType::DInt8Type),
    };
    return (attributes);
  }

  static DPointer<NetworkStream>* memberBegin()
  {
    static DPointer<NetworkStream> memberPointer[] = 
    {
       DPointer<NetworkStream>(&NetworkStream::_read, &NetworkStream::read),
       DPointer<NetworkStream>(&NetworkStream::_write, &NetworkStream::write),
       DPointer<NetworkStream>(&NetworkStream::_request, &NetworkStream::request),
       DPointer<NetworkStream>(&NetworkStream::_reply, &NetworkStream::reply),
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
