#include<sys/socket.h>
#include<arpa/inet.h>

#include "destruct.hpp"

#include "client.hpp"
#include "rpcobject.hpp"
#include "serializerpc.hpp"

Client::Client(std::string const& addr, uint32_t port)
{
  this->__connect(addr, port);
}

Client::~Client()
{
  this->__close();
}

void    Client::__connect(std::string const& addr, uint32_t port)
{
  sockaddr_in server;

  this->__connectionSocket = socket(AF_INET , SOCK_STREAM , 0);
  if (this->__connectionSocket == -1)
    throw std::string("Could not create socket");
     
  server.sin_addr.s_addr = inet_addr(addr.c_str());
  server.sin_family = AF_INET;
  server.sin_port = htons(port);
 
  if (connect(this->__connectionSocket, (sockaddr *)&server , sizeof(server)) < 0)
    throw std::string("connect failed. Error");
}

void    Client::__close(void)
{
  //close(this->__connectionSocket);
}

void    Client::start(void)
{
  NetworkStream networkStream(NULL, RealValue<DInt32>(this->__connectionSocket));
  Destruct::DStruct* fileS = this->__remoteFind(networkStream, "File"); 
  if (!fileS)
    throw std::string("Directory struct not found");

  Destruct::DStruct* directoryS = this->__remoteFind(networkStream, "Directory"); 
  if (!directoryS)
    throw std::string("Directory struct not found");

   Destruct::DStruct* vectorS = this->__remoteFind(networkStream, "DVectorObject"); 
  if (!vectorS)
    throw std::string("Directory struct not found");

  //0 is root server object
  RPCObject* remote = new RPCObject(networkStream, 0, directoryS, this->__objectManager); 
 
  std::cout << "root name : " << remote->getValue("name").get<DUnicodeString>() << std::endl;

  std::cout << "Set remove value to 'rename-by-remote'" << std::endl;
  remote->setValue("name", RealValue<DUnicodeString>("rename-by-remote"));

  std::cout << "Root name after setValue('name') : " << remote->getValue("name").get<DUnicodeString>() << std::endl;

  networkStream.write("show");

  std::cout << "Root  path : " << remote->call("path").get<DUnicodeString>() << std::endl;

  DObject* remoteChild = remote->getValue("children").get<DObject*>();
  std::cout << "Iterating on child " << std::endl;
  DUInt64 size = remoteChild->call("size").get<DUInt64>();
  
  for (DUInt64 i = 0; i < size; ++i)
  { 
    Destruct::DObject* child = remoteChild->call("get", RealValue<DUInt64>(i)).get<DObject*>();
    std::cout <<  "child(" << i << ") : " 
              << "'" << child->getValue("name").get<DUnicodeString>() << "'"
              << " is of type : " <<  child->instanceOf()->name() 
              << std::endl;

  }

  this->__print(remote);
  std::cout << "done !" << std::endl;
}

Destruct::DStruct* Client::__remoteFind(NetworkStream& stream, const std::string name)
{
  stream.write("findDStruct");
  stream.write(name);
  DSerializeRPC* rpcSerializer = new DSerializeRPC(stream, this->__objectManager);
  DStruct* dstruct = rpcSerializer->deserialize(stream);

  if (dstruct)
  {
    Destruct::Destruct& destruct = Destruct::Destruct::instance();
    destruct.registerDStruct(dstruct);
    this->__print(dstruct); 
  } 
  else
    std::cout << "Struct " << name << " is NULL can't show content " << std::endl;
  return (dstruct);
}

bool    Client::__print(DStruct* dstruct) const
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

bool    Client::__print(DObject* dobject) const
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
<<<<<<< HEAD

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
  //for (DUInt64 i = 0; i < size; ++i)
  for (DUInt64 i = 0; i < 100; ++i)
  { 
    //this->stream().write("prout");
    //Destruct::DObject* child = remoteChild->call("get", RealValue<DUInt64>(i)).get<DObject*>();
    Destruct::DObject* child = remoteChild->call("get", RealValue<DUInt64>(1)).get<DObject*>();
    //std::cout <<  "child is of type : " <<  child->instanceOf()->name() << std::endl;
  }
  std::cout << "done " << std::endl;
  //this->print(remote); ///XXX XXX XXX RPC OBJECT DOESN'T IMPLEMENT FUNCTION BY ID( pos in list of attribute ONLY NAME (String) CODE IT NOW TO MAKE IT WORK AND IT WILL BE CRRRRAZY
}
=======
>>>>>>> cac48dd671cf02d033e95a3c11ba308fe69e3f23
