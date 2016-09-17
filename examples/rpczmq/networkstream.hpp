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

  DBuffer       read(DValue const& args); 
  DInt64        write(DValue const& args);
  void          request(void);
  void          reply(void);
  void          replyError(DValue const& args);
  void          flushWrite(void);
  void          flushRead(void);


  void*         __socket; //XXX should be private but need to copy by cast...
  void*         __context;
protected: 
  ~NetworkStream();
public:
  RealValue<DFunctionObject* > _read, _write, _request, _reply, _replyError, _flushWrite, _flushRead;

  static size_t ownAttributeCount()
  {
    return (7);
  }

  static DAttribute* ownAttributeBegin()
  {
    static DAttribute  attributes[] = 
    {
       DAttribute(DType::DBufferType, "read",  DType::DInt64Type), 
       DAttribute(DType::DInt64Type,  "write", DType::DBufferType),

       DAttribute(DType::DNoneType,  "request", DType::DNoneType),
       DAttribute(DType::DNoneType,  "reply", DType::DNoneType),
       DAttribute(DType::DNoneType,  "replyError", DType::DInt8Type),

       DAttribute(DType::DNoneType, "flushWrite",  DType::DNoneType),
       DAttribute(DType::DNoneType, "flushRead",  DType::DNoneType),
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
       DPointer<NetworkStream>(&NetworkStream::_replyError, &NetworkStream::replyError),
       DPointer<NetworkStream>(&NetworkStream::_flushWrite, &NetworkStream::flushWrite),
       DPointer<NetworkStream>(&NetworkStream::_flushRead, &NetworkStream::flushRead),
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
