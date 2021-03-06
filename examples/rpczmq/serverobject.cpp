#include "serverobject.hpp"
#include "serializerpc.hpp"
#include "serverfunctionobject.hpp"
#include "objectmanager.hpp"
#include "czmq.h"

using namespace Destruct;

/* 
 *  ServerObject
 */
ServerObject::ServerObject(void* socket, void * context): __networkStream(NULL), __serializer(NULL), __deserializer(NULL), __objectManager(NULL)
{
  this->__objectManager = static_cast<ObjectManager*>(DStructs::instance().find("ObjectManager")->newObject());
  this->__networkStream = static_cast<NetworkStream*>(DStructs::instance().generate("NetworkStream"));
  this->__networkStream->__socket = socket;

  this->__serializer = static_cast<SerializeRPC*>(DStructs::instance().generate("SerializeRPC", RealValue<DObject*>(this->__networkStream)));
  this->__deserializer = static_cast<DeserializeRPC*>(DStructs::instance().generate("DeserializeRPC", RealValue<DObject*>(this->__networkStream)));
}

ServerObject::~ServerObject()
{
  this->__objectManager->destroy();
  this->__serializer->destroy();
  this->__deserializer->destroy();
  this->__networkStream->destroy();
}

void    ServerObject::find(zmsg_t* msg)
{
  DUnicodeString name = this->__deserializer->dDUnicodeString(zmsg_popmsg(msg)); 
  Destruct::DStructs& destruct = Destruct::DStructs::instance();
  DStruct* dstruct = destruct.find(name);
 
  zmsg_t* reply = (zmsg_t*)this->__serializer->sDStruct(RealValue<DStruct*>(dstruct));
  this->__networkStream->send(RealValue<DOpaque>(reply));
}

void    ServerObject::generate(zmsg_t* msg)
{
  DUnicodeString name = this->__deserializer->dDUnicodeString(zmsg_popmsg(msg)); 

  Destruct::DStructs& destruct = Destruct::DStructs::instance();
  DStruct* dstruct = destruct.find(name);

  DObject* object = dstruct->newObject(); 
  RealValue<DUInt64> objectId = this->__objectManager->call("registerObject", RealValue<DObject*>(object));

  zmsg_t* reply = (zmsg_t*)this->__serializer->sDUInt64(objectId);
  this->__networkStream->send(RealValue<DOpaque>(reply));
}

void    ServerObject::generateArg(zmsg_t* msg)
{
  DUnicodeString name = this->__deserializer->dDUnicodeString(zmsg_popmsg(msg)); 
  //get type before XXX 
  //DObject*      arg = this->__deserializer->dDObject(); 
  //DUInt64 argObjectId = this->__deserializer->dDUInt64();
  //
  //DObject* arg = this->__objectManager->object(RealValue<DUInt64>(argObjectId));

  Destruct::DStructs& destruct = Destruct::DStructs::instance();
  DStruct* dstruct = destruct.find(name);
  if (!dstruct) ///XXX must send exception to client !
    throw DException("Server::generate DStruct " + name + " not found"); 
  //DObject* object = dstruct->newObject(RealValue<DObject*>(arg)); 
  DObject* object = dstruct->newObject(RealValue<DObject*>()); 
  RealValue<DUInt64> objectId = this->__objectManager->call("registerObject", RealValue<DObject*>(object));

  zmsg_t* reply = (zmsg_t*)this->__serializer->sDUInt64(objectId);
  this->__networkStream->send(RealValue<DOpaque>(reply));
}

void    ServerObject::setValue(zmsg_t* msg)
{
  DUInt64 id = this->__deserializer->dDUInt64(msg);
  DUnicodeString name = this->__deserializer->dDUnicodeString(zmsg_popmsg(msg));

  DObject* object = this->__objectManager->object(RealValue<DUInt64>(id));

  DValue value = this->__deserializer->call(object->instanceOf()->attribute(name).type().name(), RealValue<DOpaque>(zmsg_popmsg(msg)));
  object->setValue(name, value);
  zmsg_t* reply = zmsg_new();
  this->__networkStream->send(RealValue<DOpaque>(reply));
}

void    ServerObject::getValue(zmsg_t* msg)
{
  DUInt64 id = this->__deserializer->dDUInt64(zmsg_popmsg(msg));
  DUnicodeString name = this->__deserializer->dDUnicodeString(zmsg_popmsg(msg));
  DObject* object = this->__objectManager->object(RealValue<DUInt64>(id));
  
  DValue value = object->getValue(name); 

  DType type = object->instanceOf()->attribute(name).type();
  if (type.getType() == DType::DMethodType)
  {
    DUInt64 id = this->__objectManager->registerFunctionObject(type, value);

    zmsg_t* reply = (zmsg_t*)this->__serializer->sDUInt64(RealValue<DUInt64>(id));
    this->__networkStream->send(RealValue<DOpaque>(reply));
  }
  else
  {
    zmsg_t* reply = (zmsg_t*)this->__serializer->call(type.name(), value).get<DOpaque>();
    this->__networkStream->send(RealValue<DOpaque>(reply));
  }
}

void    ServerObject::call(zmsg_t* msg)
{
  DUInt64 id = this->__deserializer->dDUInt64(zmsg_popmsg(msg));
  DObject* object = this->__objectManager->object(RealValue<DUInt64>(id));
  DUnicodeString name = this->__deserializer->dDUnicodeString(zmsg_popmsg(msg));
  DType type = object->instanceOf()->attribute(name).type();
  DValue args = this->__deserializer->call(type.argumentName(), RealValue<DOpaque>(zmsg_popmsg(msg)));

  DValue value = object->call(name, args);

  zmsg_t* reply = (zmsg_t*)this->__serializer->call(type.returnName(), value).get<DOpaque>();
  this->__networkStream->send(RealValue<DOpaque>(reply));
}

void    ServerObject::call0(zmsg_t* msg)
{
  DUInt64 id = this->__deserializer->dDUInt64(zmsg_popmsg(msg));
  DObject* object = this->__objectManager->object(RealValue<DUInt64>(id));
  DUnicodeString name = this->__deserializer->dDUnicodeString(zmsg_popmsg(msg));
  DType type = object->instanceOf()->attribute(name).type();

  DValue value = object->call(name); 
  
  zmsg_t* reply = (zmsg_t*)this->__serializer->call(type.returnName(), value).get<DOpaque>();
  this->__networkStream->send(RealValue<DOpaque>(reply));
}

void    ServerObject::functionCall(zmsg_t* msg)
{
  DUInt64 id = this->__deserializer->dDUInt64(zmsg_popmsg(msg));
  ServerFunctionObject* object = static_cast<ServerFunctionObject*>(this->__objectManager->object(RealValue<DUInt64>(id)));
  DValue args = this->__deserializer->call(DType((DType::Type_t)(DUInt64)object->argumentType).name(), RealValue<DOpaque>(zmsg_popmsg(msg))); //XXX get name directly ? 

  DValue value = ((DFunctionObject*)object->functionObject)->call(args);

  zmsg_t* reply = (zmsg_t*)this->__serializer->call(DType((DType::Type_t)(DUInt64)object->returnType).name(), value).get<DOpaque>(); 
  this->__networkStream->send(RealValue<DOpaque>(reply));
}

void    ServerObject::functionCall0(zmsg_t* msg)
{
  DUInt64 id = this->__deserializer->dDUInt64(zmsg_popmsg(msg));
  ServerFunctionObject* object = static_cast<ServerFunctionObject*>(this->__objectManager->object(RealValue<DUInt64>(id)));

  DValue value = ((DFunctionObject*)object->functionObject)->call();

  zmsg_t* reply = (zmsg_t*)this->__serializer->call(DType((DType::Type_t)(DUInt64)object->returnType).name(), value).get<DOpaque>();
  this->__networkStream->send(RealValue<DOpaque>(reply));
}
 
void    ServerObject::dispatch(void)
{
  zmsg_t* zmsg = (zmsg_t*)this->__networkStream->recv();
  DUInt8 cmd = this->__deserializer->dDUInt8(zmsg); 
  try
  {
    //XXX use a dobject it's already a call table :) faster tahn if /else if /else 
    if (cmd == CMD_FIND)
      this->find(zmsg);
    else if (cmd == CMD_GENERATE)
      this->generate(zmsg);
    else if (cmd == CMD_GENERATE_ARG)
      this->generateArg(zmsg);
    else if(cmd == CMD_SETVALUE)
      this->setValue(zmsg);
    else if(cmd == CMD_GETVALUE)
      this->getValue(zmsg);
    else if(cmd == CMD_CALL)
      this->call(zmsg);
    else if(cmd == CMD_CALL0)
      this->call0(zmsg);
    else if(cmd == CMD_FUNCTIONCALL)
      this->functionCall(zmsg);
    else if(cmd == CMD_FUNCTIONCALL0)
      this->functionCall0(zmsg);
    else
      std::cout << "Unknown command received " << cmd << std::endl;
    zmsg_destroy(&zmsg);
    //result = functionCall0
    //zmq_send(msg) 
  }
  catch (DException const& exception)
  {
    zmsg_destroy(&zmsg);
    //XXX XXX mem leak if cmd try to push in a zmsg to send reply !
    this->__networkStream->sendError(RealValue<DUnicodeString>(exception.error()));
  }
}
