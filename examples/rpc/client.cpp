#include<sys/socket.h>
#include<arpa/inet.h>

#include "destruct.hpp"

#include "client.hpp"
#include "clientobject.hpp"
#include "serializerpc.hpp"
#include "networkstream.hpp"

Client::Client(std::string const& addr, uint32_t port) : __networkStream(NULL), __serializeRPC(NULL)
{
  this->__connect(addr, port);
  this->__networkStream = new NetworkStream(NULL, RealValue<DInt32>(this->__connectionSocket));
  this->__serializeRPC = new DSerializeRPC(*this->__networkStream, this->__objectManager, this->__functionObjectManager);
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

Destruct::DStruct* Client::remoteFind(const std::string name)
{
  //std::cout << "Client::_remoteFind(stream, " << name << ")" << std::endl;
  this->__networkStream->write("findDStruct" );
  this->__networkStream->write(name);
  this->__networkStream->flush();

  DStruct* dstruct = this->__serializeRPC->deserialize(*this->__networkStream);

  if (dstruct)
  {
    Destruct::Destruct& destruct = Destruct::Destruct::instance();
    destruct.registerDStruct(dstruct);
    //this->__print(dstruct); 
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
  outStream->destroy();
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

  Destruct::DSerializers::to("Text")->serialize(*outStream, dobject);
  outStream->destroy();
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

NetworkStream*  Client::networkStream(void) const
{
  return (this->__networkStream);
}

DSerialize*     Client::serializeRPC(void) const
{
  return (this->__serializeRPC);
}

