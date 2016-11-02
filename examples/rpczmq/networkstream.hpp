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

  DOpaque       recv(void);
  void          send(DValue const& args);
  void          sendError(DValue const& args);

  void*         __socket; //XXX should be private but need to copy by cast...
protected: 
  ~NetworkStream();
public:
  RealValue<DFunctionObject* >  _recv, _send, _sendError;

  static size_t ownAttributeCount()
  {
    return (3);
  }

  static DAttribute* ownAttributeBegin()
  {
    static DAttribute  attributes[] = 
    {
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
       DPointer<NetworkStream>(&NetworkStream::_recv, &NetworkStream::recv),
       DPointer<NetworkStream>(&NetworkStream::_send, &NetworkStream::send),
       DPointer<NetworkStream>(&NetworkStream::_sendError, &NetworkStream::sendError),
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
