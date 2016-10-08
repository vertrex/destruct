#include "dstructs.hpp"

#include "client.hpp"
#include "clientobject.hpp"
#include "serializerpc.hpp"
#include "networkstream.hpp"

#include "dsimpleobject.hpp"
#include "server.hpp"
#include "objectmanager.hpp"
#include "serverobject.hpp"
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
  this->__networkStream = static_cast<NetworkStream*>(DStructs::instance().find("NetworkStream")->newObject());

  this->__networkStream->__context = this->__context;
  this->__networkStream->__socket = this->__socket;

  this->__serialize = static_cast<SerializeRPC*>(DStructs::instance().find("SerializeRPC")->newObject(RealValue<DObject*>(this->__networkStream)));
  this->__deserialize = static_cast<DeserializeRPC*>(DStructs::instance().find("DeserializeRPC")->newObject(RealValue<DObject*>(this->__networkStream)));
}

Client::Client(DStruct* dstruct, DValue const& value) : DCppObject<Client>(dstruct, value), __networkStream(NULL), __serialize(NULL), __deserialize(NULL)
{
  this->init();
  DObject* args = value;

  DUnicodeString addr = args->getValue("address");
  uint32_t port = args->getValue("port").get<DUInt32>();
  this->__connect(addr, port);
  this->__networkStream = static_cast<NetworkStream*>(DStructs::instance().find("NetworkStream")->newObject());

  this->__networkStream->__context = this->__context;
  this->__networkStream->__socket = this->__socket;

  this->__serialize = static_cast<SerializeRPC*>(DStructs::instance().find("SerializeRPC")->newObject(RealValue<DObject*>(this->__networkStream)));
  this->__deserialize = static_cast<DeserializeRPC*>(DStructs::instance().find("DeserializeRPC")->newObject(RealValue<DObject*>(this->__networkStream)));
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
  this->__socket = zmq_socket(this->__context, ZMQ_REQ);

  std::stringstream address;
  address << "tcp://" + std::string(addr) << ":" << port;
  zmq_connect(this->__socket, address.str().c_str()); 
}

void    Client::__close(void)
{
  zmq_close(this->__socket);
  zmq_ctx_destroy(this->__context); 
}

                                    //(DUnicodeString const& name ? work au lieu de dvalue ?
Destruct::DObject* Client::generate(DValue const& args)
{
  Destruct::DStructs& destruct = Destruct::DStructs::instance();
  this->__serialize->sDUInt8(RealValue<DUInt8>(CMD_GENERATE));
  this->__serialize->sDUnicodeString(args);

  this->__networkStream->request();
  DUInt64 objectId = this->__deserialize->dDUInt64();
  this->__networkStream->flushRead();

  //hanle argument name + value ... XXX
  //if struct not found in local deserialize it !
  DStruct* objectStruct = destruct.find(args.get<DUnicodeString>());

  ClientObject* root = new ClientObject(RealValue<DObject*>(this->__networkStream), RealValue<DObject*>(this->__serialize), RealValue<DObject*>(this->__deserialize), objectId, objectStruct); 

  return (RealValue<DObject*>(root));

}

Destruct::DStruct* Client::find(DValue const& name)
{
  //if not in dstruct or use namespace XXX  
  this->__serialize->sDUInt8(RealValue<DUInt8>(CMD_FIND));
  this->__serialize->sDUnicodeString(name);
  this->__networkStream->request();

  DStruct* dstruct = this->__deserialize->call("DStruct"); //XXX pass client everywhere to avoir creating  serialize / serializer / deserialzier everytime all is in client so ...
  this->__networkStream->flushRead();
  return (dstruct);
}

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
