#include <iostream>
#include<stdio.h>
#include<string.h>    //strlen
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h>    //write

#include "server.hpp"
#include "networkstream.hpp"
#include "rpcobject.hpp"

#include "destruct.hpp"
#include "dvalue.hpp"
#include "protocol/dstream.hpp"
#include "protocol/dserialize.hpp"

Server::Server()
{
  this->__bind();
  this->__listen();
}

void            Server::__bind(void)
{
  this->__listenSocket = socket(AF_INET , SOCK_STREAM, 0);
  int on = 1;
  setsockopt(this->__listenSocket, SOL_SOCKET, SO_REUSEADDR, (const char *) &on, sizeof(on));
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

  this->root = directoryStruct->newObject();
  DObject* children = root->getValue("children").get<DObject*>();
 
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

  std::cout << "DSeriailize tree " << std::endl;
  Destruct::DSerializers::to("Text")->serialize(*stream, *root);
}

void            Server::findDStruct(NetworkStream stream)
{
  std::string name;
  stream.read(name);

  Destruct::Destruct& destruct = Destruct::Destruct::instance();

  DStruct* dstruct = destruct.find(name);
  if (!dstruct)
   throw std::string("DStruct not found");

  DSerialize* binarySerializer = new DSerializeRPC();
  DStruct* streamStringStruct = destruct.find("DStreamString"); //makeNew ou cast new object !! XXX
  DStreamString* streamString = new DStreamString(streamStringStruct, RealValue<DObject*>(DNone));
  binarySerializer->serialize(*streamString, *dstruct);// use buff then send to compact data

  std::string structBuff = streamString->str();
  stream.write(structBuff);
  std::cout << " ok deserializing struct " << name << std::endl;

}

void            Server::getValue(NetworkStream stream, DObject* object)
{
  std::string name;
  stream.read(name);
  std::cout << "Remote call -> getValue : " << name <<  std::endl;
  Destruct::DValue value = object->getValue(name);

  Destruct::Destruct& destruct = Destruct::Destruct::instance();
  DStruct* streamStringStruct = destruct.find("DStreamString"); //makeNew ou cast new object !! XXX
  DStreamString streamString(streamStringStruct, RealValue<DObject*>(DNone));

  DType type = object->instanceOf()->attribute(name).type();

  DSerialize* binarySerializer = new DSerializeRPC();
  binarySerializer->serialize(streamString, value, type);

  std::string buffValue = streamString.str();
  stream.write(buffValue); //XXX \0 ? send size then ..
}

void            Server::setValue(NetworkStream stream, DObject* object)
{
  std::cout << "Remote call -> setValue" << std::endl;
  std::string name, args;
  stream.read(name);
  stream.read(args);

  Destruct::Destruct& destruct = Destruct::Destruct::instance();
  DStruct* streamStringStruct = destruct.find("DStreamString"); //makeNew ou cast new object !! XXX
  DStreamString streamString(streamStringStruct, RealValue<DObject*>(DNone));

  streamString.write(args.c_str(), args.size());

  Destruct::DValue value = object->getValue(name);  //to type it at construction //better call attributes(find).type.newValue()
  streamString >> value;

  object->setValue(name, value);
}

void            Server::call(NetworkStream stream, DObject* object)
{
  std::cout << "Remote call -> call" << std::endl;
  std::string name, args;
  stream.read(name);
  stream.read(args);
  Destruct::DValue value = object->call(name, RealValue<DUnicodeString>(args)); //XXX
  DUnicodeString rvalue = value.get<DUnicodeString>();//XXX
  stream.write(rvalue); 

  std::cout << "Object.call" << std::endl;
}

void            Server::call0(NetworkStream stream, DObject* object)
{
  std::string name;
  stream.read(name);
  std::cout << "Object.call 0 args " << name << std::endl;
  Destruct::DValue value = object->call(name);
 
  Destruct::Destruct& destruct = Destruct::Destruct::instance();
  DStruct* streamStringStruct = destruct.find("DStreamString"); //makeNew ou cast new object !! XXX
  DStreamString streamString(streamStringStruct, RealValue<DObject*>(DNone));

  streamString << value;  //XXX special case for dobject ? 

  std::string buffValue = streamString.str();
  stream.write(buffValue); //XXX \0 ? send size then ..
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

  DObject* currentObject = this->root;

  while (true)
  {
    std::cout << "Wait for message..." << std::endl;
    std::string msg;
    NetworkStream stream = this->stream();
    stream.read(msg);
    if (msg == "show") 
      this->showFS();
    //else if (msg == changeObject)
   //objectPath 
    else if (msg == "findDStruct")
      this->findDStruct(stream);
    else if(msg == "setValue")
      this->setValue(stream, currentObject);
    else if(msg == "getValue")
      this->getValue(stream, currentObject);
    else if(msg == "call")
      this->call(stream, currentObject);
    else if(msg == "call0")
      this->call0(stream, currentObject); 
    else 
      this->unknown(stream);
  }
}
