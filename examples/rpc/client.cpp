#include <iostream>
#include<stdio.h> //printf
#include<string.h>    //strlen
#include<sys/socket.h>    //socket
#include<arpa/inet.h> //inet_addr

#include "client.hpp"
#include "rpcobject.hpp"

#include "destruct.hpp"
#include "dstruct.hpp" 

#include "protocol/dserialize.hpp" 

Client::Client(std::string const& addr, uint32_t port)
{
  this->__connect(addr, port);
}

Client::~Client()
{
  this->__close();
}

NetworkStream   Client::stream(void)
{
  return (NetworkStream(NULL, RealValue<DInt32>(this->__connectionSocket))); //XXX
//  return (NetworkStream(this->__connectionSocket)); //XXX
}
void    Client::__connect(std::string const& addr, uint32_t port)
{
  struct sockaddr_in server;

  this->__connectionSocket = socket(AF_INET , SOCK_STREAM , 0);
  if (this->__connectionSocket == -1)
    throw std::string("Could not create socket");
  std::cout << "socket created" << std::endl;
     
  server.sin_addr.s_addr = inet_addr(addr.c_str());
  server.sin_family = AF_INET;
  server.sin_port = htons(port);
 
  if (connect(this->__connectionSocket, (struct sockaddr *)&server , sizeof(server)) < 0)
    throw std::string("connect failed. Error");
}

Destruct::DStruct* Client::remoteFind(const std::string name)
{
  NetworkStream stream = this->stream();

  stream.write("findDStruct");
  stream.write(name);

//  DStruct* dstruct = destruct.find(name);
//if already have ...

  DSerialize* binarySerializer = DSerializers::to("Binary");
  DStruct* dstruct = binarySerializer->deserialize(stream);// XXX au moins ca compile :) 
 
  ///XXX ca peut pas marcher en paralle le proto et pas le meme autant recup un buffer
  //et le deseralizer apres c plus simple fo un stream qui dserialize ds un buff c tout 

  //destruct.registerDStruct(dstruct);
  std::cout << "[OK] client get struct " << name << " : " << std::endl;
  Destruct::Destruct& destruct = Destruct::Destruct::instance();
  DStruct* streamStruct = destruct.find("DStreamCout");
  DStream* outStream = new  DStream(streamStruct);  
  if (outStream == NULL)
    std::cout << "Can't find stream to output fs tree" << std::endl;

  if (!dstruct)
  {
     std::cout << "Struct " << name << " is NULL can't remote deserialize " << std::endl;
     return (NULL);
  }
  Destruct::DSerializers::to("Text")->serialize(*outStream, *dstruct);

  return (dstruct);
}

void    Client::__close(void)
{
  //close(this->__connectionSocket);
}

void    Client::start(void)
{
  //Destruct::DStruct* directoryS = this->remoteFind("Directory");  //recursive & method
  Destruct::DStruct* directoryS = this->remoteFind("File");  // method XXX ?
  if (!directoryS)
    throw std::string("Directory struct not found");

  return ;
  RPCObject* remote = new RPCObject(this->stream(), "Root");
  DUnicodeString remoteName = remote->getValue("name").get<DUnicodeString>();
  std::cout << "root->name : " << remoteName << std::endl;

  remote->setValue("name", RealValue<DUnicodeString>("rename-by-remote"));

  remoteName = remote->getValue("name").get<DUnicodeString>();
  std::cout << "root->name after rename: " << remoteName << std::endl;

  this->stream().write("show");

  remoteName = remote->call("path").get<DUnicodeString>();
  std::cout << "remote call path : " << remoteName << std::endl;

  //Serializers::to("Text")->(remote); //show remote object locally
}
