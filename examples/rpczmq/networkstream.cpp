//#ifdef WIN32
//#include <winsock2.h>
//#include <ws2tcpip.h>
//#else
//#include <arpa/inet.h>
//#endif
#include <iostream>
#include <sstream>
#include <string.h>
#include <signal.h>

#include "networkstream.hpp"

#include "zmq.h"

namespace Destruct 
{

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
  zmq_msg_recv(&msg, this->__socket, 0);

  DBuffer buffer((uint8_t*)zmq_msg_data(&msg), (int32_t)zmq_msg_size(&msg));
  zmq_msg_close(&msg);

  return (buffer);
}

void my_free (void *data, void *hint)
{
	DBuffer* buffer = static_cast<DBuffer*>(hint);
	delete buffer;
}

DInt64  NetworkStream::write(DValue const& args)
{
  DBuffer buffer = args;

  zmq_msg_t msg;

  DBuffer* copy = new DBuffer(buffer);
  zmq_msg_init_data(&msg, buffer.data(), buffer.size(), my_free , (void*)copy);
  zmq_msg_send(&msg, this->__socket, ZMQ_SNDMORE);
  
  return (buffer.size()); //XXX check
}

void    NetworkStream::request(void)
{
  zmq_msg_t end;
  zmq_msg_init_size(&end, 0);
  zmq_msg_send(&end, this->__socket, 0);

  zmq_msg_t resultmsg;
  zmq_msg_init(&resultmsg);
  zmq_msg_recv(&resultmsg, this->__socket, 0);
  int8_t result = *((int8_t*)zmq_msg_data(&resultmsg));
  if (result == -1)
  {
    zmq_msg_close(&resultmsg);

    zmq_msg_t msg;
    zmq_msg_init(&msg);
    zmq_msg_recv(&msg, this->__socket, 0);
    DUnicodeString error(std::string((char*)zmq_msg_data(&msg), zmq_msg_size(&msg)));
    zmq_msg_close(&msg);
    throw DException(error);
  }

  zmq_msg_close(&resultmsg);
}

void NetworkStream::reply(void)
{
  zmq_msg_t end;
  zmq_msg_init(&end);
  zmq_msg_recv(&end, this->__socket, 0);
  zmq_msg_close(&end);

  uint8_t result = 1;
  zmq_msg_t resultmsg;
  zmq_msg_init_size(&resultmsg, sizeof(uint8_t));
  memcpy(zmq_msg_data(&resultmsg), &result, sizeof(int8_t));
  zmq_msg_send(&resultmsg, this->__socket, ZMQ_SNDMORE);

  return; 
}

void    NetworkStream::replyError(DValue const& args)
{
  zmq_msg_t end;
  zmq_msg_init(&end);
  zmq_msg_recv(&end, this->__socket, 0);
  zmq_msg_close(&end);

  int8_t result = -1;
  zmq_msg_t resultmsg;
  zmq_msg_init_size(&resultmsg, sizeof(int8_t));
  memcpy(zmq_msg_data(&resultmsg), &result, sizeof(int8_t));
  zmq_msg_send(&resultmsg, this->__socket, ZMQ_SNDMORE);

  DUnicodeString error = args;
  zmq_msg_t msg;
  zmq_msg_init_size(&msg, error.size());
  memcpy(zmq_msg_data(&msg), error.c_str(), error.size());
  zmq_msg_send(&msg, this->__socket, 0);
  
  return;
}

void    NetworkStream::flushWrite(void)
{
  zmq_msg_t end;
  zmq_msg_init_size(&end, 0);
  zmq_msg_send(&end, this->__socket, 0);
}

void    NetworkStream::flushRead(void)
{
  zmq_msg_t end;
  zmq_msg_init(&end);
  zmq_msg_recv(&end, this->__socket, 0);
  zmq_msg_close(&end);
}

}
