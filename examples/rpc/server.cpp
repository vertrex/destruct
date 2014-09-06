#include <iostream>
#include<stdio.h>
#include<string.h>    //strlen
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h>    //write

#include "fsobject.hpp"
#include "server.hpp"
#include "networkstream.hpp"
#include "rpcobject.hpp"
#include "serializerpc.hpp"

#include "destruct.hpp"
#include "dvalue.hpp"
#include "protocol/dstream.hpp"
#include "protocol/dserialize.hpp"

#include "dattribute.hpp"
#include "dsimpleobject.hpp"

using namespace Destruct;

Server::Server()
{
  this->__bind();
  this->__listen();
}

void            Server::__bind(void)
{
  this->__listenSocket = socket(AF_INET , SOCK_STREAM, 0);
  int on = 1;
  setsockopt(this->__listenSocket, SOL_SOCKET, SO_REUSEADDR, (const char *)&on, sizeof(on));
  if (this->__listenSocket == -1)
    throw std::string("Could not create socket");
     
  sockaddr_in server;
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = INADDR_ANY;
  server.sin_port = htons(0xdff);
  if(bind(this->__listenSocket,(sockaddr *)&server , sizeof(server)) < 0)
    throw std::string("bind failed. Error");
}

void            Server::__listen(void) 
{
  int c;
  struct sockaddr_in client;
 
  listen(this->__listenSocket , 3);
  std::cout << "Waiting for incoming connections..." << std::endl;
  c = sizeof(sockaddr_in);
  this->__connectionSocket = accept(this->__listenSocket, (sockaddr *)&client, (socklen_t*)&c);
  if (this->__connectionSocket < 0)
    throw std::string("accept failed");
  std::cout << "Connection accepted" << std::endl;
}

/**
  Create Fake FS :

  Root/File1
       File2
       Directory1/File3 
*/
void            Server::initFS(void)
{
  DStruct* fileStruct = makeNewDCpp<File>("File");
  DStruct* directoryStruct = makeNewDCpp<Directory>("Directory");
  Destruct::Destruct& dstruct = Destruct::Destruct::instance();

  dstruct.registerDStruct(fileStruct);
  dstruct.registerDStruct(directoryStruct);

  DObject* root = directoryStruct->newObject();
  this->__objectManager.registerObject(root);
  DObject* children =  root->getValue("children").get<DObject*>();
 
  DObject* file1 = fileStruct->newObject();

  file1->setValue("name", RealValue<DUnicodeString>("File1"));
  children->call("push", RealValue<DObject*>(file1));

  File* file2 = new File(fileStruct, RealValue<DObject*>(DNone)); 
  file2->name = "File2"; 
  children->call("push", RealValue<DObject*>(file2));

  DObject* directory1 = directoryStruct->newObject();
  children->call("push", RealValue<DObject*>(directory1));
  DObject* d1children = directory1->getValue("children").get<DObject*>();
/*  
  Directory* directory1 = new Directory(directoryStruct, RealValue<DObject*>(DNone));
  directory1->name = "Directory1";
  DObject* d1children = directory1->children;
  */

  File* file3 = new File(fileStruct, RealValue<DObject*>(DNone));  
  this->__objectManager.registerObject(file3);
  file3->name = "File3"; 
  d1children->call("push", RealValue<DObject*>(file3));
}

void            Server::serve(void)
{
  DObject* currentObject = RealValue<DObject*>(DNone);
  NetworkStream stream = NetworkStream(NULL, RealValue<DInt32>(this->__connectionSocket));
  RPCServer rpcServer(stream, this->__objectManager);
  this->initFS();
  this->showRoot();

  uint64_t id = 0;
  while (true)
  {
    std::cout << "Wait for message..." << std::endl;
    std::string msg;
    rpcServer.networkStream().read(msg);
  //   stream.read(msg);
    if (msg == "show") 
      this->showRoot();
    else if (msg == "findDStruct")
    {
      rpcServer.findDStruct();
    }
    else if(msg == "setValue")
    {
      stream.read(&id); 
      currentObject = this->__objectManager.object(id);
      rpcServer.setValue(currentObject);
    }
    else if(msg == "getValue")
    {
      stream.read(&id); 
      currentObject = this->__objectManager.object(id);
      rpcServer.getValue(currentObject);
    }
    else if(msg == "call")
    {
      stream.read(&id); 
      currentObject = this->__objectManager.object(id);
      rpcServer.call(currentObject);
    }
    else if(msg == "call0")
    {
      stream.read(&id);
      currentObject = this->__objectManager.object(id);
      rpcServer.call0(currentObject);
    }
    else
      rpcServer.unknown(msg);
  }
}

void            Server::showRoot(void)
{
  Destruct::Destruct& destruct = Destruct::Destruct::instance();
  DStruct* streamStruct = destruct.find("DStreamCout");

  DStream* stream = new DStream(streamStruct); 
  if (stream == NULL)
    std::cout << "Can't find stream to output fs tree" << std::endl;

  Destruct::DSerializers::to("Text")->serialize(*stream, *this->__objectManager.object(0));
}
