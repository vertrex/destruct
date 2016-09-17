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

DBuffer NetworkStream::read(DValue const& args)
{
  DInt64 size = args; //DUInt32 [M ´T 

  
  DBuffer buffer(size);
  int num = zmq_recv(this->__socket, buffer.data(), size, 0);
  if (num != size)
    std::cout << "NetworkStream::read got " << num << " asked " << size << std::endl;

  return (buffer);
}

DInt64  NetworkStream::write(DValue const& args)
{
  DBuffer buffer = args;

  int num = zmq_send(this->__socket, buffer.data(), buffer.size(), 0);
  if (num != buffer.size())
    std::cout << "NetworkStream::write got " << num << " asked " << buffer.size() << std::endl;
  
  return (buffer.size()); //XXX check
}

DInt8   NetworkStream::request(void)
{
  int8_t result;
  int num = zmq_recv(this->__socket,  &result, sizeof(int8_t), 0);
  if (num != sizeof(int8_t))
    std::cout << "NetworkStream::reply can't get reply got " << num << std::endl;
  return (result);
}

void    NetworkStream::reply(DValue const& args)
{
  DInt8 result = args;

  int num = zmq_send(this->__socket, &result, sizeof(int8_t), 0);
  //if (num != buffer.size())
//std::cout << "NetworkStream::write got " << num << " asked " << buffer.size() << std::endl;
  
}

}
