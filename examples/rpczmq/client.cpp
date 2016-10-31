#include "dstructs.hpp"

#include "client.hpp"
#include "stubobject.hpp"
#include "serializerpc.hpp"
#include "networkstream.hpp"

#include "dsimpleobject.hpp"
#include "server.hpp"
#include "objectmanager.hpp"
#include "serverobject.hpp"
#include "serverfunctionobject.hpp"


#include <zmq.h>
#include <czmq.h>
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
  dstruct->addAttribute(DAttribute(DType::DObjectType, "auth"));
  destruct.registerDStruct(dstruct); 

  dstruct = new DStruct(NULL, "RPCAuth", DSimpleObject::newObject);
  dstruct->addAttribute(DAttribute(DType::DUnicodeStringType, "cert")); //public and private key for client/server
  dstruct->addAttribute(DAttribute(DType::DUnicodeStringType, "certStore"));//path of accepted public key store for auth server
  dstruct->addAttribute(DAttribute(DType::DUnicodeStringType, "clientCert"));//accepted client cert 
  destruct.registerDStruct(dstruct); ;
}

/**
 *  Client
 */
Client::Client(DStruct* dstruct, DValue const& args) : DCppObject<Client>(dstruct, args), __networkStream(NULL), __serialize(NULL), __deserialize(NULL)
{
  this->init();

  this->__connect(args);
  this->__networkStream = static_cast<NetworkStream*>(DStructs::instance().find("NetworkStream")->newObject());

  this->__networkStream->__context = this->__context;
  this->__networkStream->__socket = this->__socket;

  this->__serialize = static_cast<SerializeRPC*>(DStructs::instance().find("SerializeRPC")->newObject(RealValue<DObject*>(this->__networkStream)));
  this->__deserialize = static_cast<DeserializeRPC*>(DStructs::instance().find("DeserializeRPC")->newObject(RealValue<DObject*>(this->__networkStream)));
}

Client::~Client()
{
  this->__networkStream->destroy();
  this->__serialize->destroy();
  this->__deserialize->destroy();
  this->__close();
  //zcert_destroy
}

/**
 *  Use private key to connect to server
 */
void    Client::__setAuth(DObject* rpcAuth)
{
  zauth_t* auth = zauth_new((zctx_t*)this->__context);
  if (auth == NULL)
    throw DException("Can't init authentication");
  zauth_set_verbose(auth, true); //XXX 
  zauth_configure_curve(auth, "*", rpcAuth->getValue("certStore").get<DUnicodeString>().c_str());

  zcert_t* server_cert = zcert_load(rpcAuth->getValue("cert").get<DUnicodeString>().c_str());
  if (server_cert == NULL)
    throw DException("Can't load server certificate");
  zcert_apply(server_cert, this->__socket);
  zsocket_set_curve_server(this->__socket, 1);
}

void    Client::__connect(DObject* args)
{
  this->__context = zctx_new();
  this->__socket = zsocket_new((zctx_t*)this->__context, ZMQ_REQ);

  DObject* auth = args->getValue("auth");
  if (auth != DNone)
    this->__setAuth(auth);

  std::stringstream address;
  address << "tcp://" + std::string(args->getValue("address").get<DUnicodeString>()) << ":" << args->getValue("port").get<DUInt32>();
  zmq_connect(this->__socket, address.str().c_str()); 
}

void    Client::__close(void)
{
  zmq_close(this->__socket);
  zmq_ctx_destroy(this->__context); 
}

Destruct::DObject* Client::generate(DValue const& args)
{ 
  Destruct::DStructs& destruct = Destruct::DStructs::instance();
  DStruct* objectStruct = NULL;
  DUnicodeString structName = args;
  try 
  {
    objectStruct = destruct.find(structName);
  }
  catch (DException const& exception)
  {
    objectStruct = this->find(args);
  }

  zmsg_t* msg = (zmsg_t*)this->__serialize->sDUInt8(RealValue<DUInt8>(CMD_GENERATE));
  zmsg_t* msg2 = (zmsg_t*)this->__serialize->sDUnicodeString(args);
  zmsg_addmsg(msg, &msg2);

  this->__networkStream->send(RealValue<DOpaque>(msg));
  zmsg_t* reply = (zmsg_t*)this->__networkStream->recv(); 
  DUInt64 objectId = this->__deserialize->dDUInt64(reply);

  //hanle argument name + value ... XXX
  StubObject* root = new StubObject(RealValue<DObject*>(this->__networkStream), RealValue<DObject*>(this->__serialize), RealValue<DObject*>(this->__deserialize), objectId, objectStruct); 

  return (RealValue<DObject*>(root));

}

Destruct::DStruct* Client::find(DValue const& name)
{
  //if not in dstruct or use namespace XXX  
  zmsg_t* msg = (zmsg_t*)this->__serialize->sDUInt8(RealValue<DUInt8>(CMD_FIND));
  zmsg_t* submsg = (zmsg_t*)this->__serialize->sDUnicodeString(name);
  zmsg_addmsg(msg, &submsg);

  this->__networkStream->send(RealValue<DOpaque>(msg));
  zmsg_t* reply = (zmsg_t*)this->__networkStream->recv();
  DStruct* dstruct = this->__deserialize->dDStruct(reply); //XXX pass client everywhere to avoir creating  serialize / serializer / deserialzier everytime all is in client so ...
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
