#include <iostream>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "server.hpp"
#include "serverobject.hpp"

using namespace Destruct;

Server::Server(uint32_t port) : __networkStream(NULL), __serializer(NULL)
{
  this->__bind(port);
}

Server::~Server()
{
  delete this->__networkStream;
  delete this->__serializer;
}

void            Server::__bind(int32_t port)
{
  this->__listenSocket = socket(AF_INET , SOCK_STREAM, 0);
  int on = 1;
  setsockopt(this->__listenSocket, SOL_SOCKET, SO_REUSEADDR, (const char *)&on, sizeof(on));
  if (this->__listenSocket == -1)
    throw std::string("Could not create socket");
     
  sockaddr_in server;
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = INADDR_ANY;
  server.sin_port = htons(port);
  if(bind(this->__listenSocket,(sockaddr *)&server , sizeof(server)) < 0)
    throw std::string("bind failed. Error");
}

void            Server::__listen(void) 
{
  int c;
  struct sockaddr_in client;
 
  listen(this->__listenSocket , 3);
  std::cout << "Waiting for incoming connections ..." << std::endl;
  c = sizeof(sockaddr_in);
  this->__connectionSocket = accept(this->__listenSocket, (sockaddr *)&client, (socklen_t*)&c);
  if (this->__connectionSocket < 0)
    throw std::string("accept failed");
  std::cout << "Connection accepted" << std::endl;

  delete this->__networkStream;
  delete this->__serializer;
  this->__networkStream = new NetworkStream(NULL, RealValue<DInt32>(this->__connectionSocket));
  this->__serializer = new DSerializeRPC(*this->__networkStream, this->__objectManager, this->__functionObjectManager);
}


void    Server::findDStruct(void)
{
  std::string name;
  this->__networkStream->read(name);

  std::cout << "Send DStruct " << name << std::endl;
  Destruct::Destruct& destruct = Destruct::Destruct::instance();
  DStruct* dstruct = destruct.find(name);
  if (!dstruct)
   throw std::string("DStruct not found");

  this->__serializer->serialize(*this->__networkStream, *dstruct);
  this->__networkStream->flush();
}
 
void Server::unknown(const std::string& cmd)
{
  std::cout << "Receive unknown command : " << cmd << std::endl;
  this->__networkStream->write("Unknown command : " + cmd);
  this->__networkStream->flush();
}

void            Server::daemonize(void)
{
  while (1)
  {
    try
    {
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

void            Server::serve(void)
{
  this->__listen();
  ServerObject serverObject(*this->__networkStream, this->__serializer, this->__objectManager, this->__functionObjectManager);
  this->initRoot(); //again
  this->showRoot();

  while (true)
  {
    //std::cout << "Wait for message..." << std::endl;
    std::string msg;
    this->__networkStream->read(msg);

    if (msg == "show") 
      this->showRoot();
    else if (msg == "findDStruct")
      this->findDStruct();
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
      this->unknown(msg);
  }
}

ObjectManager<DObject*>& Server::objectManager(void) 
{
  return (this->__objectManager);
}

ObjectManager<ServerFunctionObject*>& Server::functionObjectManager(void) 
{
  return (this->__functionObjectManager);
}

void            Server::showRoot(void)
{
  Destruct::Destruct& destruct = Destruct::Destruct::instance();
  DStruct* streamStruct = destruct.find("DStreamCout");

  DStream* stream = new DStream(streamStruct); 
  if (stream == NULL)
    std::cout << "Can't find stream to output fs tree" << std::endl;

  Destruct::DSerializers::to("Text")->serialize(*stream, this->__objectManager.object(0));
  stream->destroy();
}
