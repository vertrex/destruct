#include <iostream>
#include <sstream>
#include <string.h>

#include "zmq.h"
#include "czmq.h"

#include "networkstream.hpp"

NetworkStream::NetworkStream(DStruct* dstruct, DValue const& args) : DCppObject<NetworkStream>(dstruct, args), __socket(NULL), __context(NULL)
{
  this->init();
}

NetworkStream::NetworkStream(const NetworkStream& rhs) : DCppObject<NetworkStream>(rhs), __socket(rhs.__socket), __context(rhs.__context)
{
  this->copy(this, rhs);
}

NetworkStream::~NetworkStream()
{
}

DOpaque  NetworkStream::recv(void)
{
  zmsg_t* msg = zmsg_recv(this->__socket);
  if (msg == NULL)
    throw DException("NetworkStream::recv can't receive message");

  zframe_t* resultf = zmsg_pop(msg);
  DInt8 result = *((DInt8*)zframe_data(resultf));
  zframe_destroy(&resultf);
  if (result == -1)
  {
    zframe_t* errorf = zmsg_pop(msg);
    DUnicodeString error(std::string((char*)zframe_data(errorf), zframe_size(errorf)));
    zframe_destroy(&errorf);
    zmsg_destroy(&msg);
    throw DException(error);
  }

  return (msg);
}

void    NetworkStream::send(DValue const& args) //DValue const& args ? 
{
  zmsg_t* msg = (zmsg_t*) args.get<DOpaque>();

  DInt8 value = 1;
  zframe_t* frame = zframe_new((DInt8*)&value, sizeof(value));
  zmsg_prepend(msg, &frame);

  zmsg_send(&msg, this->__socket);
  zmsg_destroy(&msg);
}

void    NetworkStream::sendError(DValue const& args)
{
  zmsg_t* msg = zmsg_new();

  DInt8 value = -1;
  zframe_t* frame = zframe_new((DInt8*)&value, sizeof(value));
  zmsg_append(msg, &frame);

  DUnicodeString error = args;
  zframe_t* errorf = zframe_new(error.c_str(), error.size());
  zmsg_append(msg, &errorf); 

  zmsg_send(&msg, this->__socket);
  zmsg_destroy(&msg);
}
