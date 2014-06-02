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
  setsockopt(this->__listenSocket, SOL_SOCKET,
      SO_REUSEADDR,
        (const char *) &on, sizeof(on));
  if (this->__listenSocket == -1)
    throw std::string("Could not create socket");
     
  struct sockaddr_in server;
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = INADDR_ANY;
  server.sin_port = htons(0xdff);
  if(bind(this->__listenSocket,(struct sockaddr *)&server , sizeof(server)) < 0)
    throw std::string("bind failed. Error");
}

void            Server::__listen(void) 
{
  int c;
  struct sockaddr_in client;
 
  listen(this->__listenSocket , 3);
  std::cout << "Waiting for incoming connections..." << std::endl;
  c = sizeof(struct sockaddr_in);
  this->__connectionSocket = accept(this->__listenSocket, (struct sockaddr *)&client, (socklen_t*)&c);
  if (this->__connectionSocket < 0)
    throw std::string("accept failed");
  std::cout << "Connection accepted" << std::endl;
}

int32_t         Server::_receive(void* buff, int32_t size) 
{
  return(recv(this->__connectionSocket, buff, size, 0));
}

int32_t         Server::_send(void* buff, int32_t size) const
{
  return (send(this->__connectionSocket, buff, size, 0));
}

NetworkStream   Server::stream(void)
{
  return (NetworkStream(this->__connectionSocket));
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

/*  Directory* directory1 = new Directory(directoryStruct, RealValue<DObject*>(DNone));
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

void            Server::getValue(NetworkStream stream, DObject* object)
{
  std::cout << "Remote call -> getValue : " << std::endl;
  std::string name;
  stream.read(name);
  Destruct::DValue value = object->getValue(name);
  DUnicodeString rvalue = value.get<DUnicodeString>();
  stream.write(rvalue);
}

void            Server::setValue(NetworkStream stream, DObject* object)
{
  std::cout << "Remote call -> setValue" << std::endl;
  std::string name, args;
  stream.read(name);
  stream.read(args);
  object->setValue(name, RealValue<DUnicodeString>(args));
}

void            Server::call(NetworkStream stream, DObject* object)
{
  std::cout << "Remote call -> call" << std::endl;
  std::string name, args;
  stream.read(name);
  stream.read(args);
  Destruct::DValue value = object->call(name, RealValue<DUnicodeString>(args));
  DUnicodeString rvalue = value.get<DUnicodeString>();
  stream.write(rvalue); 

  std::cout << "Object.call" << std::endl;
}

void            Server::call0(NetworkStream stream, DObject* object)
{
  std::cout << "Object.call 0 args" << std::endl;
  std::string name;
  stream.read(name);
  Destruct::DValue value = object->call(name);
  DUnicodeString rvalue = value.get<DUnicodeString>();
  stream.write(rvalue);
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

  int  readed;
  DObject* currentObject = this->root;
  readed = 1;

  while (true)
  {
    std::cout << "Wait for message..." << std::endl;
    if (readed > 0 && readed <= 512)
    {
      std::string msg;
      NetworkStream stream = this->stream();
      stream.read(msg);
      readed = msg.size();
      if (msg == "show") 
      {
        this->showFS();
      }
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
    else
     break;
  }
     
  if(readed == 0)
    std::cout << "Client disconnected" << std::endl;
  else if(readed == -1)
   std::cout << "recv failed" << std::endl;
}
/*
void sendObject(void)
{
  this->serializeBin(root);

  return DRPCProxyObject(this, root);
}


int main(int argc, char** argv)
{
  std::cout << "server listen on port 0xdff" << std::endl;
  Server server;

  try 
  {
    server.serve();
  }
  catch (const std::string& error)
  {
    std::cout << error << std::endl;
  }
*
  DObject root; //
  DObject file;
  DObject directory;
  root->add(file)
  root->add(directory);
  root->show();

  rpcroot = dserver.send(root);
  rpcroot.receive();

  return (0); 
}
*/
