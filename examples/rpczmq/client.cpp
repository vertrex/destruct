#include "dstructs.hpp"

#include "client.hpp"
#include "clientobject.hpp"
#include "serializerpc.hpp"
#include "networkstream.hpp"

#include "dsimpleobject.hpp"
#include "server.hpp"
#include "objectmanager.hpp"
#include "serverfunctionobject.hpp"

#include "zmq.h"
/**
 *  Client & Server Destruct Declaration
 */
extern "C"
{
  EXPORT void DestructExport()
  {
    Client::declare();
  }
}

void    Client::declare(void)
{
  Destruct::DStructs& destruct = Destruct::DStructs::instance();
  destruct.registerDStruct(makeNewDCpp<NetworkStream>("NetworkStream"));
  destruct.registerDStruct(makeNewDCpp<SerializeRPC>("SerializeRPC"));
  destruct.registerDStruct(makeNewDCpp<DeserializeRPC>("DeserializeRPC"));

  destruct.registerDStruct(makeNewDCppSingleton<ObjectManager >("ObjectManager"));
  destruct.registerDStruct(makeNewDCpp<ServerFunctionObject>("ServerFunctionObject"));


  DStruct* dstruct = makeNewDCpp<Server>("Server"); 
  destruct.registerDStruct(dstruct);

  dstruct = makeNewDCpp<Client>("Client");
  destruct.registerDStruct(dstruct);

  dstruct = new DStruct(NULL, "ClientArgument", DSimpleObject::newObject);
  dstruct->addAttribute(DAttribute(DType::DUInt32Type, "port"));
  dstruct->addAttribute(DAttribute(DType::DUnicodeStringType, "address"));
  destruct.registerDStruct(dstruct); 
}

/**
 *  Client
 */
Client::Client(DUnicodeString const& addr, uint32_t port) : DCppObject<Client>(NULL, RealValue<DObject*>(DNone)), __networkStream(NULL), __serialize(NULL), __deserialize(NULL)
{
  this->init();
  this->__connect(addr, port);
  this->__networkStream = DStructs::instance().find("NetworkStream")->newObject();

//XXX NOT REALLY CLEAN
  NetworkStream* ns = static_cast<NetworkStream*>(this->__networkStream);
  ns->__context = this->__context;
  ns->__socket = this->__socket;

  this->__serialize = DStructs::instance().find("SerializeRPC")->newObject(RealValue<DObject*>(this->__networkStream));
  this->__deserialize = DStructs::instance().find("DeserializeRPC")->newObject(RealValue<DObject*>(this->__networkStream));
}

Client::Client(DStruct* dstruct, DValue const& value) : DCppObject<Client>(dstruct, value), __networkStream(NULL), __serialize(NULL), __deserialize(NULL)
{
  this->init();
  DObject* args = value;

  DUnicodeString addr = args->getValue("address");
  uint32_t port = args->getValue("port").get<DUInt32>();
  this->__connect(addr, port);
  this->__networkStream = DStructs::instance().find("NetworkStream")->newObject();

//NOT REALLY CLEAN
  NetworkStream* ns = static_cast<NetworkStream*>(this->__networkStream);
  ns->__context = this->__context;
  ns->__socket = this->__socket;

  this->__serialize = DStructs::instance().find("SerializeRPC")->newObject(RealValue<DObject*>(this->__networkStream));
  this->__deserialize = DStructs::instance().find("DeserializeRPC")->newObject(RealValue<DObject*>(this->__networkStream));
}

Client::~Client()
{
  std::cout << "~Client" << std::endl;
  this->__networkStream->destroy();
  this->__serialize->destroy();
  this->__deserialize->destroy();
  this->__close();
}

void    Client::__connect(DUnicodeString const& addr, uint32_t port)
{
  this->__context = zmq_ctx_new();
  this->__socket = zmq_socket(this->__context, ZMQ_PAIR);//ZMQ_REQ
 
  std::string faddr =  "tcp://" + std::string(addr) + ":" + "3583";
  int rc = zmq_connect(this->__socket, faddr.c_str()); //XXX port to string !
}

void    Client::__close(void)
{
  zmq_close(this->__socket);
  zmq_ctx_destroy(this->__context); 
}

DObject*   Client::start(void)
{
  throw DException("Client::start Not implemented.");
}

DValue     Client::findObject(void) //getRoot XXX ?
{
  Destruct::DStructs& destruct = Destruct::DStructs::instance();

  DStruct* registryS = destruct.find("Registry"); //replaced bu create root !

  ClientObject* root = new ClientObject(RealValue<DObject*>(this->__networkStream), RealValue<DObject*>(this->__serialize), RealValue<DObject*>(this->__deserialize), 0, registryS); 

  return (RealValue<DObject*>(root));
}

DValue  Client::createRoot(DUnicodeString objectName)
{
  Destruct::DStructs& destruct = Destruct::DStructs::instance();

  DStruct* registryS = destruct.find(objectName);

  ClientObject* root = new ClientObject(RealValue<DObject*>(this->__networkStream), RealValue<DObject*>(this->__serialize), RealValue<DObject*>(this->__deserialize), 0, registryS); 

  return (RealValue<DObject*>(root));

}

Destruct::DStruct* Client::remoteFind(const DUnicodeString name)
{
  this->__serialize->call("DUnicodeString", RealValue<DUnicodeString>("findDStruct"));
  this->__serialize->call("DUnicodeString", RealValue<DUnicodeString>(name));
  //this->__networkStream->call("flush");
 
  DStruct* dstruct = this->__deserialize->call("DStruct");
  if (dstruct)
  {
    Destruct::DStructs& destruct = Destruct::DStructs::instance();
    if (destruct.find(dstruct->name()) == NULL) //XXX don't need to find again
      destruct.registerDStruct(dstruct);
    else
      std::cout << dstruct->name() << " already registered" << std::endl;
    //this->print(dstruct); 
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

//int32_t Client::connectionSocket(void) const
//{
  //return (this->__connectionSocket);
//}

DObject*     Client::networkStream(void) const
{
  return (this->__networkStream);
}

DObject*     Client::serializeRPC(void) const
{
  return (this->__serialize);
}

DObject*     Client::deserializeRPC(void) const
{
  return (this->__deserialize);
}
