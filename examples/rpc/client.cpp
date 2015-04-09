
#ifndef WIN32
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#endif


#include "dstructs.hpp"

#include "client.hpp"
#include "clientobject.hpp"
#include "serializerpc.hpp"
#include "networkstream.hpp"

#include "dsimpleobject.hpp"
#include "server.hpp"
extern "C"
{
  void  declare(void)
  {
    Destruct::DStructs& destruct = Destruct::DStructs::instance();
   
    DStruct* dstruct = makeNewDCpp<Server>("Server"); 
    destruct.registerDStruct(dstruct);
    dstruct = makeNewDCpp<Client>("Client");
    destruct.registerDStruct(dstruct);

    dstruct = new DStruct(NULL, "ClientArgument", DSimpleObject::newObject);
    dstruct->addAttribute(DAttribute(DType::DUInt32Type, "port"));
    dstruct->addAttribute(DAttribute(DType::DUnicodeStringType, "address"));
    destruct.registerDStruct(dstruct); 
  }
}

Client::Client(DUnicodeString const& addr, uint32_t port) : DCppObject<Client>(NULL, RealValue<DObject*>(DNone)), __networkStream(NULL), __serializeRPC(NULL)
{
  this->__connect(addr, port);
  //this->__networkStream = new NetworkStream(NULL, RealValue<DInt32>(this->__connectionSocket));
  this->__networkStream = DStructs::instance().find("NetworkStream")->newObject(RealValue<DInt32>(this->__connectionSocket));
  //this->__serializeRPC = new DSerializeRPC(*this->__networkStream, this->__objectManager, this->__functionObjectManager);
  this->__serializeRPC = DStructs::instance().find("SerializeRPC")->newObject(RealValue<DObject*>(this->__networkStream));
}

Client::Client(DStruct* dstruct, DValue const& value) : DCppObject<Client>(dstruct, value), __networkStream(NULL), __serializeRPC(NULL)
{
  this->init();
  DObject* args = value.get<DObject*>();

  DUnicodeString addr = args->getValue("address").get<DUnicodeString>();
  uint32_t port = args->getValue("port").get<DUInt32>();
  this->__connect(addr, port);
  //this->__networkStream = new NetworkStream(NULL, RealValue<DInt32>(this->__connectionSocket));
  this->__networkStream = DStructs::instance().find("NetworkStream")->newObject(RealValue<DInt32>(this->__connectionSocket));
  //this->__serializeRPC = new DSerializeRPC(*this->__networkStream, this->__objectManager, this->__functionObjectManager);
  this->__serializeRPC = DStructs::instance().find("SerializeRPC")->newObject(RealValue<DObject*>(this->__networkStream));
}

Client::~Client()
{
  //delete this->__networkStream;
  //delete this->__serializeRPC;
  this->__networkStream->destroy();
  this->__serializeRPC->destroy();
  this->__close();
}

void    Client::__connect(DUnicodeString const& addr, uint32_t port)
{
  sockaddr_in server;

  this->__connectionSocket = socket(AF_INET , SOCK_STREAM , 0);
  if (this->__connectionSocket == -1)
    throw DException("Client::__connect Could not create socket");
     
  server.sin_addr.s_addr = inet_addr(addr.c_str());
  server.sin_family = AF_INET;
  server.sin_port = htons(port);
  memset(&server.sin_zero, 0, sizeof(server.sin_zero));

  if (connect(this->__connectionSocket, (sockaddr *)&server , sizeof(server)) < 0)
    throw DException("Client::__connect connect failed. Error");
}

void    Client::__close(void)
{
  close(this->__connectionSocket);
}

DObject*                Client::start(void)
{
  throw DException("Client::start Not implemented.");
}

DValue     Client::findObject(void) //getRoot
{
  Destruct::DStructs& destruct = Destruct::DStructs::instance();
//  this->__networkStream = new NetworkStream(NULL, RealValue<DInt32>(this->connectionSocket()));
  this->__networkStream = destruct.generate("NetworkStream", RealValue<DInt32>(this->connectionSocket())); 

  DStruct* registryS = destruct.find("Registry");

  //DSerialize* serializer = new DSerializeRPC(*this->__networkStream, this->objectManager(), this->functionObjectManager());
  DObject* serializer = DStructs::instance().find("SerializeRPC")->newObject(RealValue<DObject*>(this->__networkStream));
  ClientObject* root = new ClientObject(this->__networkStream, serializer, 0, registryS); 

  return (RealValue<DObject*>(root));
}

Destruct::DStruct* Client::remoteFind(const DUnicodeString name)
{
  //std::cout << "Client::_remoteFind(stream, " << name << ")" << std::endl;
  //this->__networkStream->write(DUnicodeString("findDStruct"));
  //this->__networkStream->write(name);
  //this->__networkStream->flush();

  //DStruct* dstruct = this->__serializeRPC->deserialize(*this->__networkStream);
  DStruct* dstruct; //XXX new serialization plus haut
  if (dstruct)
  {
    Destruct::DStructs& destruct = Destruct::DStructs::instance();
    destruct.registerDStruct(dstruct);
    //this->__print(dstruct); 
  } 
  else
    std::cout << "Struct " << name << " is NULL can't show content " << std::endl;
  return (dstruct);
}

bool    Client::print(DStruct* dstruct) const
{
  Destruct::DStructs& destruct = Destruct::DStructs::instance();
  DObject* stream = destruct.generate("DStreamCout");

  if (!dstruct)
    return (false);
 
  DObject* serializer = destruct.generate("SerializeText", RealValue<DObject*>(stream));
  serializer->call("DStruct", RealValue<DStruct*>(dstruct));

  serializer->destroy();
  stream->destroy();
  return (true);
}

bool    Client::print(DObject* dobject) const
{
  Destruct::DStructs& destruct = Destruct::DStructs::instance();
  DObject* stream = destruct.generate("DStreamCout");
  if (!dobject)
    return (false);

  DObject* serializer = destruct.generate("SerializeText", RealValue<DObject*>(stream));
  serializer->call("DObject", RealValue<DObject*>(dobject));
  
  serializer->destroy();
  stream->destroy();
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

DObject*     Client::networkStream(void) const
{
  return (this->__networkStream);
}

DObject*     Client::serializeRPC(void) const
{
  return (this->__serializeRPC);
}
