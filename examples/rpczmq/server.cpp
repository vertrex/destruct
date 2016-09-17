#include <iostream>
#include <stdio.h>
#include <string.h>

#ifdef WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>
#include <unistd.h>
#endif

#include "server.hpp"
#include "serverobject.hpp"
#include "zmq.h"

using namespace Destruct;

Server::Server(uint32_t port) : DCppObject<Server>(NULL, RealValue<DUInt32>(port)), __objectManager(NULL)
{
  this->__bind(port);
  this->__objectManager = DStructs::instance().generate("ObjectManager");
}

Server::Server(DStruct* dstruct, DValue const& args) : DCppObject<Server>(dstruct, args), __objectManager(NULL) 
{
  this->init();
  this->__bind(args.get<DUInt32>());
  this->__objectManager = DStructs::instance().generate("ObjectManager");
}

Server::~Server()
{
  zmq_close(this->__socket);
  zmq_ctx_destroy(this->__context);

  this->__objectManager->call("clear"); //called in destroy 
  this->__objectManager->destroy();
}

void    Server::setRoot(RealValue<DObject*> root)
{
  this->__objectManager->call("registerObject", root);
}

void    Server::__bind(int32_t port)
{
  this->__context = zmq_ctx_new();
  this->__socket = zmq_socket(this->__context, ZMQ_PAIR);//rep
  int rc = zmq_bind(this->__socket, "tcp://*:3583"); //XXX port
}

void    Server::daemonize(void)
{
  while (1)
  {
    try
    {
      std::cout << "Daemonize this->Server()" << std::endl;
      this->serve();
    }
    catch (DException const& exception)
    {
      std::cout << "Server error " << exception.error() << std::endl << "Restarting server ... " << std::endl;
    }
    catch (...)
    {
      std::cout << "Server error : unknown " << std::endl << " Restarting server ... " << std::endl;
    }
  }
}

void    Server::serve(void)
{
  std::cout << "Create destruct server" << std::endl;
  ServerObject serverObject(this->__socket, this->__context);
  
  while (true)
  {
    //std::cout << "Wait for message..." << std::endl;
    DUnicodeString msg = serverObject.cmd();

    if (msg == "find")
      serverObject.find();
    else if (msg == "generate")
      serverObject.generate();
    else if(msg == "setValue")
      serverObject.setValue();
    else if(msg == "getValue")
      serverObject.getValue();
    else if(msg == "call")
      serverObject.call();
    else if(msg == "call0")
      serverObject.call0();
    else if(msg == "functionCall")
      serverObject.functionCall();
    else if(msg == "functionCall0")
      serverObject.functionCall0();
    else
      serverObject.unknown(msg);
  }
}

DObject* Server::objectManager(void) 
{
  return (this->__objectManager);
}

