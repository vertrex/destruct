#include "serverobject.hpp"
#include "serializerpc.hpp"
#include "serverfunctionobject.hpp"
#include "objectmanager.hpp"

using namespace Destruct;

/* 
 *  ServerObject
 */
ServerObject::ServerObject(void* socket, void * context): __networkStream(NULL), __serializer(NULL), __deserializer(NULL), __objectManager(NULL)
{
  this->__objectManager = static_cast<ObjectManager*>(DStructs::instance().find("ObjectManager")->newObject());
  this->__networkStream = static_cast<NetworkStream*>(DStructs::instance().generate("NetworkStream"));
  this->__networkStream->__context = context;
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

void    ServerObject::find(void)
{
  DUnicodeString name = this->__deserializer->dDUnicodeString(); 
  Destruct::DStructs& destruct = Destruct::DStructs::instance();
  DStruct* dstruct = destruct.find(name);
 
  this->__networkStream->reply();
  this->__serializer->sDStruct(RealValue<DStruct*>(dstruct));
  this->__networkStream->flushWrite();
}

void    ServerObject::generate(void)
{
  DUnicodeString name = this->__deserializer->dDUnicodeString(); 
  Destruct::DStructs& destruct = Destruct::DStructs::instance();
  DStruct* dstruct = destruct.find(name);

  DObject* object = dstruct->newObject(); 
  RealValue<DUInt64> objectId = this->__objectManager->call("registerObject", RealValue<DObject*>(object));

  this->__networkStream->reply();
  this->__serializer->sDUInt64(objectId);
  this->__networkStream->flushWrite();
}

void    ServerObject::generateArg(void)
{
  DUnicodeString name = this->__deserializer->dDUnicodeString(); 
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

  this->__networkStream->reply();
  this->__serializer->sDUInt64(objectId);
  this->__networkStream->flushWrite();
}


void    ServerObject::setValue(void)
{
  DUInt64 id = this->__deserializer->dDUInt64();
  DUnicodeString name = this->__deserializer->dDUnicodeString();

  DObject* object = this->__objectManager->object(RealValue<DUInt64>(id));

  DValue value = this->__deserializer->call(object->instanceOf()->attribute(name).type().name());
  object->setValue(name, value);
  this->__networkStream->reply();
}

void    ServerObject::getValue(void)
{
  DUInt64 id = this->__deserializer->dDUInt64();
  DUnicodeString name = this->__deserializer->dDUnicodeString();
  DObject* object = this->__objectManager->object(RealValue<DUInt64>(id));
  
  DValue value = object->getValue(name);

  DType type = object->instanceOf()->attribute(name).type();
  if (type.getType() == DType::DMethodType)
  {
    DUInt64 id = this->__objectManager->registerFunctionObject(type, value);

    this->__networkStream->reply();
    this->__serializer->sDUInt64(RealValue<DUInt64>(id));
    this->__networkStream->flushWrite();
  }
  else
  {
    this->__networkStream->reply(); 
    this->__serializer->call(type.name(), value);
    this->__networkStream->flushWrite();
  }
}

void    ServerObject::call(void)
{
  DUInt64 id = this->__deserializer->dDUInt64();
  DObject* object = this->__objectManager->object(RealValue<DUInt64>(id));
  DUnicodeString name = this->__deserializer->dDUnicodeString();
  DType type = object->instanceOf()->attribute(name).type();
  DValue args = this->__deserializer->call(type.argumentName());

  DValue value = object->call(name, args);

  this->__networkStream->reply();
  this->__serializer->call(type.returnName(), value);
  this->__networkStream->flushWrite();
}

void    ServerObject::call0(void)
{
  DUInt64 id = this->__deserializer->dDUInt64();
  DObject* object = this->__objectManager->object(RealValue<DUInt64>(id));
  DUnicodeString name = this->__deserializer->dDUnicodeString();
  DType type = object->instanceOf()->attribute(name).type();

  DValue value = object->call(name); 
  
  this->__networkStream->reply(); 
  this->__serializer->call(type.returnName(), value);
  this->__networkStream->flushWrite();
}

void    ServerObject::functionCall(void)
{
  DUInt64 id = this->__deserializer->dDUInt64();
  ServerFunctionObject* object = static_cast<ServerFunctionObject*>(this->__objectManager->object(RealValue<DUInt64>(id)));
  DValue args = this->__deserializer->call(DType((DType::Type_t)(DUInt64)object->argumentType).name()); //XXX get name directly ? 

  DValue value = ((DFunctionObject*)object->functionObject)->call(args);

  this->__networkStream->reply();
  this->__serializer->call(DType((DType::Type_t)(DUInt64)object->returnType).name(), value); 
  this->__networkStream->flushWrite();
}

void    ServerObject::functionCall0(void)
{
  DUInt64 id = this->__deserializer->dDUInt64();
  ServerFunctionObject* object = static_cast<ServerFunctionObject*>(this->__objectManager->object(RealValue<DUInt64>(id)));

  DValue value = ((DFunctionObject*)object->functionObject)->call();

  this->__networkStream->reply();
  this->__serializer->call(DType((DType::Type_t)(DUInt64)object->returnType).name(), value); 
  this->__networkStream->flushWrite();
}
 
void    ServerObject::dispatch(void)
{
  DUInt8 msg = this->__deserializer->dDUInt8();
  try
  {
    //XXX use a dobject it's already a call table :) faster tahn if /else if /else 
    if (msg == CMD_FIND)
      this->find();
    else if (msg == CMD_GENERATE)
      this->generate();
    else if (msg == CMD_GENERATE_ARG)
      this->generateArg();
    else if(msg == CMD_SETVALUE)
      this->setValue();
    else if(msg == CMD_GETVALUE)
      this->getValue();
    else if(msg == CMD_CALL)
      this->call();
    else if(msg == CMD_CALL0)
      this->call0();
    else if(msg == CMD_FUNCTIONCALL)
      this->functionCall();
    else if(msg == CMD_FUNCTIONCALL0)
      this->functionCall0();
    else
      std::cout << "Unknown msg received " << msg << std::endl;
  }
  catch (DException const& exception)
  {
    this->__networkStream->call("replyError", RealValue<DUnicodeString>(exception.error()));
  }
}
