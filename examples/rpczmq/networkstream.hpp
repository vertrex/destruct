#ifndef __NETWORKSTREAM_HPP__
#define __NETWORKSTREAM_HPP__

#include <sstream>
#include <iostream>
#include <string.h>

#include "protocol/dcppobject.hpp"

using namespace Destruct;

class NetworkStream : public DCppObject<NetworkStream>
{
public:
  NetworkStream(DStruct* dstruct, DValue const& args);
  NetworkStream(NetworkStream const& copy);


  DBuffer       read(void); 
  DInt64        write(DValue const& args);
  void          request(void);
  void          reply(void);
  void          replyError(DValue const& args);
  void          flushWrite(void);
  void          flushRead(void);

  DOpaque       recv(void);
  void          send(DValue const& args);
  void          sendError(DValue const& args);

  std::stringstream __stream;
  void*         __socket; //XXX should be private but need to copy by cast...
  void*         __context;
protected: 
  ~NetworkStream();
public:
  RealValue<DFunctionObject* > _read, _write, _request, _reply, _replyError, _flushWrite, _flushRead, _recv, _send, _sendError;

  static size_t ownAttributeCount()
  {
    return (9);
  }

  static DAttribute* ownAttributeBegin()
  {
    static DAttribute  attributes[] = 
    {
       DAttribute(DType::DBufferType, "read",  DType::DNoneType),  //no size because get a message
       DAttribute(DType::DInt64Type,  "write", DType::DBufferType),

       DAttribute(DType::DNoneType,  "request", DType::DNoneType),
       DAttribute(DType::DNoneType,  "reply", DType::DNoneType),
       DAttribute(DType::DNoneType,  "replyError", DType::DInt8Type),

       DAttribute(DType::DNoneType, "flushWrite",  DType::DNoneType),
       DAttribute(DType::DNoneType, "flushRead",  DType::DNoneType),

       DAttribute(DType::DOpaqueType, "recv", DType::DNoneType),
       DAttribute(DType::DNoneType, "send", DType::DOpaqueType),
       DAttribute(DType::DNoneType, "sendError", DType::DOpaqueType),
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

       DPointer<NetworkStream>(&NetworkStream::_recv, &NetworkStream::recv),
       DPointer<NetworkStream>(&NetworkStream::_send, &NetworkStream::send),
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

#endif
