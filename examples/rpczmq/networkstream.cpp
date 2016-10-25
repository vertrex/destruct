#include <iostream>
#include <sstream>
#include <string.h>

#include "zmq.h"

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

DBuffer NetworkStream::read(void)
{
  zmq_msg_t msg;
  zmq_msg_init(&msg);
  if (zmq_msg_recv(&msg, this->__socket, 0) == -1)
    throw DException("NetworkStream::read zmq_msg_recv " + std::string(zmq_strerror(errno)));

  DBuffer buffer((uint8_t*)zmq_msg_data(&msg), (int32_t)zmq_msg_size(&msg));
  zmq_msg_close(&msg);

  return (buffer);
}

void deleteDBuffer(void *data, void *hint)
{
  DBuffer* buffer = static_cast<DBuffer*>(hint);
  delete buffer;
}

DInt64  NetworkStream::write(DValue const& args)
{
  DBuffer buffer = args;

  zmq_msg_t msg;

  DBuffer* copy = new DBuffer(buffer);
  zmq_msg_init_data(&msg, buffer.data(), buffer.size(), deleteDBuffer, (void*)copy);
  if (zmq_msg_send(&msg, this->__socket, ZMQ_SNDMORE) == -1)
    throw DException("NetworkStream::write zmq_msg_send " + std::string(zmq_strerror(errno)));
  return (buffer.size()); //XXX check
}

void    NetworkStream::request(void)
{
  this->flushWrite();

  zmq_msg_t resultmsg;
  if (zmq_msg_init(&resultmsg) != 0)
    throw DException("NetworkStream::request can't init message");
  if (zmq_msg_recv(&resultmsg, this->__socket, 0) == -1)
     throw DException("NetworkStream::request zmq_msg_recv " + std::string(zmq_strerror(errno)));
  int8_t result = *((int8_t*)zmq_msg_data(&resultmsg));
  zmq_msg_close(&resultmsg);
  if (result == -1)
  {
    zmq_msg_t msg;
    zmq_msg_init(&msg);
    if (zmq_msg_recv(&msg, this->__socket, 0) == -1)
     throw DException("NetworkStream::request zmq_msg_recv " + std::string(zmq_strerror(errno)));
    DUnicodeString error(std::string((char*)zmq_msg_data(&msg), zmq_msg_size(&msg)));
    zmq_msg_close(&msg);
    throw DException(error);
  }
}

void NetworkStream::reply(void)
{
  this->flushRead();

  uint8_t result = 1;
  zmq_msg_t resultmsg;
  zmq_msg_init_size(&resultmsg, sizeof(uint8_t));
  memcpy(zmq_msg_data(&resultmsg), &result, sizeof(int8_t));
  if (zmq_msg_send(&resultmsg, this->__socket, ZMQ_SNDMORE) == -1)
     throw DException("NetworkStream::reply zmq_msg_send " + std::string(zmq_strerror(errno)));
  return; 
}

void    NetworkStream::replyError(DValue const& args)
{
  this->flushRead();

  int8_t result = -1;
  zmq_msg_t resultmsg;
  zmq_msg_init_size(&resultmsg, sizeof(int8_t));
  memcpy(zmq_msg_data(&resultmsg), &result, sizeof(int8_t));
  if (zmq_msg_send(&resultmsg, this->__socket, ZMQ_SNDMORE) == -1)
     throw DException("NetworkStream::replyError zmq_msg_send " + std::string(zmq_strerror(errno)));

  DUnicodeString error = args;
  zmq_msg_t msg;
  zmq_msg_init_size(&msg, error.size());
  memcpy(zmq_msg_data(&msg), error.c_str(), error.size());
  if (zmq_msg_send(&msg, this->__socket, 0) == -1)
     throw DException("NetworkStream::replyError zmq_msg_send " + std::string(zmq_strerror(errno)));
  
  return;
}

void    NetworkStream::flushWrite(void)
{
  zmq_msg_t end;
  zmq_msg_init_size(&end, 0);
  if (zmq_msg_send(&end, this->__socket, 0) == -1)
     throw DException("NetworkStream::flushWrite zmq_msg_send " + std::string(zmq_strerror(errno)));
}

void    NetworkStream::flushRead(void)
{
  zmq_msg_t end;
  zmq_msg_init(&end);
  if (zmq_msg_recv(&end, this->__socket, 0) == -1)
     throw DException("NetworkStream::flushReadzmq_msg_recv " + std::string(zmq_strerror(errno)));
  zmq_msg_close(&end);
}
