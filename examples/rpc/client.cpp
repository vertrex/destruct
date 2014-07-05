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
  sockaddr_in server;

  this->__connectionSocket = socket(AF_INET , SOCK_STREAM , 0);
  if (this->__connectionSocket == -1)
    throw std::string("Could not create socket");
  std::cout << "socket created" << std::endl;
     
  server.sin_addr.s_addr = inet_addr(addr.c_str());
  server.sin_family = AF_INET;
  server.sin_port = htons(port);
 
  if (connect(this->__connectionSocket, (sockaddr *)&server , sizeof(server)) < 0)
    throw std::string("connect failed. Error");
}

Destruct::DStruct* Client::remoteFind(const std::string name)
{
  DStruct *dstruct = NULL; 
  NetworkStream stream = this->stream();

  std::string structBuff;
  stream.write("findDStruct");
  stream.write(name);
  stream.read(structBuff);  

  Destruct::Destruct& destruct = Destruct::Destruct::instance();
  DStruct*  streamStringStruct = destruct.find("DStreamString");
  DStreamString* streamString = new DStreamString(streamStringStruct, RealValue<DObject*>(DNone));

  streamString->write(structBuff.c_str(), structBuff.size());

  DSerialize* binarySerializer = DSerializers::to("Binary");
  dstruct = binarySerializer->deserialize(*streamString);// use buff then send to compact data

  if (dstruct)
  {
    std::cout << "[OK] client get struct " << name << " : " << std::endl;
    destruct.registerDStruct(dstruct);
    this->print(dstruct); 
  } 
  else
    std::cout << "Struct " << name << " is NULL can't show content " << std::endl;
  return (dstruct);
}

bool    Client::print(DStruct* dstruct) const
{
  Destruct::Destruct& destruct = Destruct::Destruct::instance();
  DStruct* streamStruct = destruct.find("DStreamCout");
  DStream* outStream = new DStream(streamStruct);  
  if (outStream == NULL)
    std::cout << "Can't find stream to output fs tree" << std::endl;

  if (!dstruct)
    return (false);
  
  Destruct::DSerializers::to("Text")->serialize(*outStream, *dstruct);
  return (true);
}

bool    Client::print(DObject* dobject) const
{
  Destruct::Destruct& destruct = Destruct::Destruct::instance();
  DStruct* streamStruct = destruct.find("DStreamCout");
  DStream* outStream = new DStream(streamStruct);  
  if (outStream == NULL)
    std::cout << "Can't find stream to output fs tree" << std::endl;

  if (!dobject)
    return (false);

  Destruct::DSerializers::to("Text")->serialize(*outStream, *dobject);
  return (true);
}

void    Client::__close(void)
{
  //close(this->__connectionSocket);
}

void    Client::start(void)
{
  //Destruct::DStruct* directoryS = this->remoteFind("Directory");  //recursive & method
  Destruct::DStruct* fileS = this->remoteFind("File"); 
  if (!fileS)
    throw std::string("Directory struct not found");

  Destruct::DStruct* directoryS = this->remoteFind("Directory"); 
  if (!directoryS)
    throw std::string("Directory struct not found");

   Destruct::DStruct* vectorS = this->remoteFind("DVectorObject"); 
  if (!vectorS)
    throw std::string("Directory struct not found");


  RPCObject* remote = new RPCObject(this->stream(), 0, directoryS, this->__objectManager); // 0 is root use id instead of string but path could work too
 
  DUnicodeString remoteName = remote->getValue("name").get<DUnicodeString>();
  std::cout << "root name : " << remoteName << std::endl;

  std::cout << "Set remove value to 'rename-by-remote'" << std::endl;
  remote->setValue("name", RealValue<DUnicodeString>("rename-by-remote"));

  remoteName = remote->getValue("name").get<DUnicodeString>();
  std::cout << "Root name after setValue('name') : " << remoteName << std::endl;

  this->stream().write("show");

  remoteName = remote->call("path").get<DUnicodeString>();
  std::cout << "Root  path : " << remoteName << std::endl;

  DObject* remoteChild = remote->getValue("children").get<DObject*>();

  std::cout << "Iterating on child " << std::endl;
  DUInt64 size = remoteChild->call("size").get<DUInt64>();
  for (DUInt64 i = 0; i < size; ++i)
  { 
    Destruct::DObject* child = remoteChild->call("get", RealValue<DUInt64>(i)).get<DObject*>();
    std::cout <<  "child is of type : " <<  child->instanceOf()->name() << std::endl;
  }
  std::cout << "done " << std::endl;
  //this->print(remote); ///XXX XXX XXX RPC OBJECT DOESN'T IMPLEMENT FUNCTION BY ID( pos in list of attribute ONLY NAME (String) CODE IT NOW TO MAKE IT WORK AND IT WILL BE CRRRRAZY
}
