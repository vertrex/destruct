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
  zmq_msg_t msg;
  zmq_msg_init(&msg);
 
  int num =zmq_msg_recv(&msg, this->__socket, 0);
  //if (num == -1)
  //std::cout << "NetworkStream::read zmq_msg_recv error : " <<  zmq_strerror(zmq_errno()) << std::endl;
  //else
  //std::cout << "NetworkStream::read OK !" << std::endl;


  DBuffer buffer((uint8_t*)zmq_msg_data(&msg), zmq_msg_size(&msg));
  zmq_msg_close(&msg);

  return (buffer);
}

DInt64  NetworkStream::write(DValue const& args)
{
  DBuffer buffer = args;

  zmq_msg_t msg;
  zmq_msg_init_size(&msg, buffer.size());
  memcpy(zmq_msg_data(&msg), buffer.data(), buffer.size());

  int num = zmq_msg_send(&msg, this->__socket, ZMQ_SNDMORE);
  //if (num != -1)
  //std::cout << "NetworkStream::send zmq_msg_send error : " <<  zmq_strerror(zmq_errno()) << std::endl;
  //else
  //std::cout << "NetworkStream::send OK !" << std::endl;
  
  return (buffer.size()); //XXX check
}

void    NetworkStream::request(void)
{
  zmq_msg_t end;
  zmq_msg_init_size(&end, 0);
  int num = zmq_msg_send(&end, this->__socket, 0);

 
  zmq_msg_t resultmsg;
  zmq_msg_init(&resultmsg);
  num = zmq_msg_recv(&resultmsg, this->__socket, 0);
  //if (num == -1)
  //std::cout << "NetworkStream::read zmq_msg_recv error : " <<  zmq_strerror(zmq_errno()) << std::endl;
  //else
  //std::cout << "NetworkStream::read OK !" << std::endl;

  int8_t result = *((int8_t*)zmq_msg_data(&resultmsg));

  if (result == -1)
  {
    zmq_msg_close(&resultmsg);

    zmq_msg_t msg;
    zmq_msg_init(&msg);
    int num =zmq_msg_recv(&msg, this->__socket, 0);
    //if (num == -1)
    //std::cout << "NetworkStream::read zmq_msg_recv error : " <<  zmq_strerror(zmq_errno()) << std::endl;
    //else
    //std::cout << "NetworkStream::read OK !" << std::endl;

    DUnicodeString error(std::string((char*)zmq_msg_data(&msg), zmq_msg_size(&msg)));
    zmq_msg_close(&msg);
    throw DException(error);
  }

  zmq_msg_close(&resultmsg);
}

void NetworkStream::reply(DValue const& args)
{
  zmq_msg_t end;
  zmq_msg_init(&end);
  int num = zmq_msg_recv(&end, this->__socket, 0);
  zmq_msg_close(&end);

  uint8_t result = 1;
  zmq_msg_t resultmsg;
  zmq_msg_init_size(&resultmsg, sizeof(uint8_t));
  memcpy(zmq_msg_data(&resultmsg), &result, sizeof(int8_t));
  num = zmq_msg_send(&resultmsg, this->__socket, ZMQ_SNDMORE);
  //if (num != -1) 
  //std::cout << "NetworkStream::reply zmq_msg_send MORE error : " <<  zmq_strerror(zmq_errno()) << std::endl;
  //else
  //std::cout << "NetworkStream::send OK !" << std::endl;

  return; 
}

void    NetworkStream::replyError(DValue const& args)
{
  zmq_msg_t end;
  zmq_msg_init(&end);
  int num = zmq_msg_recv(&end, this->__socket, 0);
  zmq_msg_close(&end);


  int8_t result = -1;
  zmq_msg_t resultmsg;
  zmq_msg_init_size(&resultmsg, sizeof(int8_t));
  memcpy(zmq_msg_data(&resultmsg), &result, sizeof(int8_t));

  num = zmq_msg_send(&resultmsg, this->__socket, ZMQ_SNDMORE);
  //if (num != -1) 
  //std::cout << "NetworkStream::replyError zmq_msg_send MORE error : " <<  zmq_strerror(zmq_errno()) << std::endl;
  //else
  //std::cout << "NetworkStream::send OK !" << std::endl;

  DUnicodeString error = args;

  zmq_msg_t msg;
  zmq_msg_init_size(&msg, error.size());
  memcpy(zmq_msg_data(&msg), error.c_str(), error.size());

  num = zmq_msg_send(&msg, this->__socket, 0);
  //if (num != -1)
  //std::cout << "NetworkStream::send zmq_msg_send error : " <<  zmq_strerror(zmq_errno()) << std::endl;
  //else
  //std::cout << "NetworkStream::send OK !" << std::endl;
  
  return;
}

void    NetworkStream::flushWrite(void)
{
  zmq_msg_t end;
  zmq_msg_init_size(&end, 0);
  int num = zmq_msg_send(&end, this->__socket, 0);
}

void    NetworkStream::flushRead(void)
{
  zmq_msg_t end;
  zmq_msg_init(&end);
  int num = zmq_msg_recv(&end, this->__socket, 0);
  zmq_msg_close(&end);
}

}
