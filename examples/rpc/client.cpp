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

DObject* Client::start(void)
{
  NetworkStream networkStream(NULL, RealValue<DInt32>(this->__connectionSocket));

  //if (0 == 1)
  //{
  //for (uint64_t z = 0; z < 100; z++)
  //{
          ////networkStream.write("test");
          ////std::string toto;
          ////networkStream.read(toto);
          ////std::cout << toto << std::endl;
     //std::cout << "Iteration " <<  z << std::endl;
     //Destruct::DStruct* fileSS = this->__remoteFind(networkStream, "File"); 
     //if (!fileSS)
      //throw std::string("Directory struct not found");
     //std::cout << fileSS->name() << std::endl;
  //}
  //return ; }

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
  RPCObject* remote = new RPCObject(networkStream, 0, directoryS, this->__objectManager, this->__functionObjectManager); 
 
  std::cout << "root name : " << remote->getValue("name").get<DUnicodeString>() << std::endl;

  std::cout << "Set remote value to 'rename-by-remote'" << std::endl;
  remote->setValue("name", RealValue<DUnicodeString>("rename-by-remote"));

  std::cout << "Root name after setValue('name') : " << remote->getValue("name").get<DUnicodeString>() << std::endl;

  networkStream.write("show");

  std::cout << "Root  path : " << remote->call("path").get<DUnicodeString>() << std::endl;

  DObject* remoteChild = remote->getValue("children").get<DObject*>();
  std::cout << "Iterating on child " << std::endl;
  DUInt64 size = remoteChild->call("size").get<DUInt64>();
 
   
  for (DUInt64 i = 0; i < size; ++i)
  { 
    for (DUInt64 x = 0; x < 10; ++x) {
    Destruct::DObject* child = remoteChild->call("get", RealValue<DUInt64>(i)).get<DObject*>();
    std::cout <<  "child(" << i << ") : " 
              << "'" << child->getValue("name").get<DUnicodeString>() << "'"
              << " is of type : " <<  child->instanceOf()->name() 
              << std::endl;

     } break;
  }

  this->__print(remote);
  std::cout << "done !" << std::endl;
  return (remote);
}

Destruct::DStruct* Client::__remoteFind(NetworkStream& stream, const std::string name)
{
        //std::cout << "Client::_remoteFind(stream, " << name << ")" << std::endl;
  stream.write("findDStruct" );
  stream.write(name);

  //std::cout << "Client::_remoteFind new DSerializeRPC" << std::endl;
                                  ///XXX passer stream par ref pour eviter la copy
  DSerializeRPC* rpcSerializer = new DSerializeRPC(stream, this->__objectManager, this->__functionObjectManager);
  //std::cout << "Client::_remoteFind rpcSerializer->deserialize "<< std::endl;
  DStruct* dstruct = rpcSerializer->deserialize(stream);

  if (dstruct)
  {
    Destruct::Destruct& destruct = Destruct::Destruct::instance();
    destruct.registerDStruct(dstruct);
    //this->__print(dstruct); 
  } 
  else
    std::cout << "Struct " << name << " is NULL can't show content " << std::endl;
  //std::cout << "Client::_remoteFind return Struct " << std::endl;
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

  Destruct::DSerializers::to("Text")->serialize(*outStream, dobject);
  return (true);
}

int32_t Client::connectionSocket(void) const
{
  return (this->__connectionSocket);
}

ObjectManager<DObject*>&  Client::objectManager(void)
{
  return (this->__objectManager);
}

ObjectManager<ServerFunctionObject*>&  Client::functionObjectManager(void)
{
  return (this->__functionObjectManager);
}
