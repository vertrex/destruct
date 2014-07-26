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

int32_t         Server::_receive(void* buff, int32_t size) 
{
  return (recv(this->__connectionSocket, buff, size, 0));
}

int32_t         Server::_send(void* buff, int32_t size) const
{
  return (send(this->__connectionSocket, buff, size, 0));
}

NetworkStream   Server::stream(void)
{
  return (NetworkStream(NULL, RealValue<DInt32>(this->__connectionSocket))); //XXX

//  return (NetworkStream(this->__connectionSocket)); //XXX
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
  //this->__objectManager.registerObject(file1);

  file1->setValue("name", RealValue<DUnicodeString>("File1"));
  children->call("push", RealValue<DObject*>(file1));

  File* file2 = new File(fileStruct, RealValue<DObject*>(DNone)); 
  //this->__objectManager.registerObject(file2); 
  file2->name = "File2"; 
  children->call("push", RealValue<DObject*>(file2));

  DObject* directory1 = directoryStruct->newObject();
  //this->__objectManager.registerObject(directory1);
  children->call("push", RealValue<DObject*>(directory1));
  DObject* d1children = directory1->getValue("children").get<DObject*>();
  //this->__objectManager.registerObject(d1children);

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

void            Server::showFS(void)
{
  std::cout << "Remote exec -> Execute 'show' on server : done" << std::endl;
  Destruct::Destruct& destruct = Destruct::Destruct::instance();
  
  DStruct* streamStruct = destruct.find("DStreamCout");

  DStream* stream = new  DStream(streamStruct);  
  if (stream == NULL)
    std::cout << "Can't find stream to output fs tree" << std::endl;

  Destruct::DSerializers::to("Text")->serialize(*stream, *this->__objectManager.object(0));
}

void            Server::findDStruct(NetworkStream stream)
{
  std::string name;
  stream.read(name);

  Destruct::Destruct& destruct = Destruct::Destruct::instance();

  DStruct* dstruct = destruct.find(name);
  if (!dstruct)
   throw std::string("DStruct not found");

  DSerialize* binarySerializer = new DSerializeRPC(this->stream(), this->__objectManager);

  DStruct* streamStringStruct = destruct.find("DStreamString"); //makeNew ou cast new object !! XXX
  DStreamString* streamString = new DStreamString(streamStringStruct, RealValue<DObject*>(DNone));
  binarySerializer->serialize(*streamString, *dstruct);// use buff then send to compact data

  std::string structBuff = streamString->str();
  stream.write(structBuff);
  std::cout << " ok deserializing struct " << name << std::endl;

}

void            Server::unknown(NetworkStream stream)
{
  std::cout << "Receive unknown command" << std::endl;
  stream.write("Unknown command");
}

void            Server::serve(void)
{
  this->initFS();
  this->showFS();
  DObject* currentObject = RealValue<DObject*>(DNone);

  RPCServer rpcServer(this->stream(), this->__objectManager);

  uint64_t id = 0;
  NetworkStream stream = this->stream();
  while (true)
  {
    std::cout << "Wait for message..." << std::endl;
    std::string msg;
    stream.read(msg);
//readObject ID !

    if (msg == "show") 
      this->showFS();
    else if (msg == "findDStruct")
    {
      this->findDStruct(stream);
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
      stream.read(&id); //XXX is working vby ret ? 
      currentObject = this->__objectManager.object(id);
      rpcServer.call0(currentObject);
    }
    else
    { 
      std::cout << "unknown command " << std::endl;
      this->unknown(stream);
    }
  }
}

RPCServer::RPCServer(NetworkStream networkStream, ObjectManager & objectManager) : __networkStream(networkStream), __objectManager(objectManager)
{

}

void    RPCServer::getValue(DObject* object)
{
  std::string name;
  this->__networkStream.read(name);
  Destruct::DValue value = object->getValue(name);

  Destruct::Destruct& destruct = Destruct::Destruct::instance();
  DStruct* streamStringStruct = destruct.find("DStreamString"); //makeNew ou cast new object !! XXX
  DStreamString streamString(streamStringStruct, RealValue<DObject*>(DNone));

  DType type = object->instanceOf()->attribute(name).type();

  DSerialize* binarySerializer = new DSerializeRPC(this->__networkStream, this->__objectManager);
  binarySerializer->serialize(streamString, value, type.getType());

  std::string buffValue = streamString.str();
  this->__networkStream.write(buffValue); //XXX \0 ? send size then ..
}

void    RPCServer::setValue(DObject* object)
{
  std::string name, args;
  this->__networkStream.read(name);
  this->__networkStream.read(args);

  Destruct::Destruct& destruct = Destruct::Destruct::instance();
  DStruct* streamStringStruct = destruct.find("DStreamString"); //makeNew ou cast new object !! XXX
  DStreamString streamString(streamStringStruct, RealValue<DObject*>(DNone));

  streamString.write(args.c_str(), args.size());

  Destruct::DValue value = object->getValue(name);  //to type it at construction //better call attributes(find).type.newValue()
  streamString >> value;

  object->setValue(name, value);
}

void    RPCServer::call(DObject* object)
{
  std::string name, sargs;
  this->__networkStream.read(name);
  this->__networkStream.read(sargs);

/* get arg value hugly not compatible with obj must use serializer*/
  Destruct::Destruct& destruct = Destruct::Destruct::instance();
  DStruct* streamStringStruct = destruct.find("DStreamString"); //makeNew ou cast new object !! XXX
  DStreamString streamString(streamStringStruct, RealValue<DObject*>(DNone));
  streamString.write(sargs.c_str(), sargs.size());

//XXX 
  Destruct::DValue args(DType(object->instanceOf()->attribute(name).type().getArgumentType()).newValue());
  streamString >> args;

/* call func with args */
  Destruct::DValue value = object->call(name, args); 

/* send return value */
  DStreamString rstreamString(streamStringStruct, RealValue<DObject*>(DNone));
  DType type = object->instanceOf()->attribute(name).type();
  DSerialize* binarySerializer = new DSerializeRPC(this->__networkStream, this->__objectManager);
  binarySerializer->serialize(rstreamString, value, type.getReturnType());

  std::string buffValue = rstreamString.str();
  this->__networkStream.write(buffValue); //XXX \0 ? send size then ..
}

void    RPCServer::call0(DObject* object)
{
  std::string name;
  this->__networkStream.read(name);

  Destruct::DValue value = object->call(name); 
  
  Destruct::Destruct& destruct = Destruct::Destruct::instance();
  DStruct* streamStringStruct = destruct.find("DStreamString"); //makeNew ou cast new object !! XXX
  DStreamString streamString(streamStringStruct, RealValue<DObject*>(DNone));

  DType type = object->instanceOf()->attribute(name).type();
  DSerialize* binarySerializer = new DSerializeRPC(this->__networkStream, this->__objectManager);
  binarySerializer->serialize(streamString, value, type.getReturnType());

  std::string buffValue = streamString.str();
  this->__networkStream.write(buffValue); //XXX \0 ? send size then ..
}

ObjectManager::ObjectManager() : __currentID(0)
{
}

uint64_t        ObjectManager::registerObject(DObject* object)
{
  std::map<uint64_t, DObject*>::const_iterator i= this->__objectsID.begin();
  for (; i != this->__objectsID.end(); ++i)
    if (i->second == object)
      return i->first;

  uint64_t id = this->__currentID;
  this->__objectsID[id] = object;
  this->__currentID++;
  return (id);
}

DObject*        ObjectManager::object(uint64_t id) const
{
  std::map<uint64_t, DObject*>::const_iterator object = this->__objectsID.find(id);
  if (object != this->__objectsID.end())
    return (object->second);
  return RealValue<DObject*>(DNone); 
}
