#include "serverobject.hpp"
#include "serializerpc.hpp"
#include "serverfunctionobject.hpp"

using namespace Destruct;

/* 
 *  ServerObject
 */
ServerObject::ServerObject(DObject* networkStream, DObject* serializer, DObject* deserializer)  : __networkStream(networkStream), __serializer(serializer), __deserializer(deserializer), __object(NULL), __id(-1)
{
  this->__objectManager = DStructs::instance().find("ObjectManager")->newObject();
}

ServerObject::~ServerObject()
{
  //if (this->__networkStream)
  //this->__networkStream->destroy();
  //if (this->__serializer)
  //this->__serializer->destroy();
  //if (this->__deserializer)
  //this->__deserializer->destroy();
  if (this->__objectManager)
    this->__objectManager->destroy();
  if (this->__object)
    this->__object->destroy();
}

void    ServerObject::setValue(void)
{
  this->__id = this->__deserializer->call("DUInt64").get<DUInt64>();
  DUnicodeString name = this->__deserializer->call("DUnicodeString").get<DUnicodeString>();

  DObject* object = this->__objectManager->call("object", this->__id).get<DObject*>();

  DValue value = this->__deserializer->call(object->instanceOf()->attribute(name).type().name());
  object->setValue(name, value);
}

void    ServerObject::getValue(void)
{
  this->__id = this->__deserializer->call("DUInt64").get<DUInt64>();
  DObject* object = this->__objectManager->call("object", this->__id).get<DObject*>();
  DUnicodeString name = this->__deserializer->call("DUnicodeString").get<DUnicodeString>();
  
  Destruct::DValue value = object->getValue(name);

  DType type = object->instanceOf()->attribute(name).type();
  //new serial
  if (type.getType() == DType::DMethodType) //XXX
  {
    ServerFunctionObject* serverFunctionObject = static_cast<ServerFunctionObject*>(DStructs::instance().find("ServerFunctionObject")->newObject()); //DStruct get ?
    serverFunctionObject->argumentType = type.getArgumentType();
    serverFunctionObject->returnType = type.getReturnType();
    serverFunctionObject->functionObject = value.get<DFunctionObject*>();

    DValue id = this->__objectManager->call("registerObject", RealValue<DObject*>(serverFunctionObject));
    this->__serializer->call("DUInt64", id);
  }
  else
    this->__serializer->call(type.name(), value);
  this->__networkStream->call("flush"); 
}

void    ServerObject::call(void)
{

  this->__id = this->__deserializer->call("DUInt64").get<DUInt64>();
  DObject* object = this->__objectManager->call("object", this->__id).get<DObject*>();
  DUnicodeString name = this->__deserializer->call("DUnicodeString").get<DUnicodeString>();
  DType type = object->instanceOf()->attribute(name).type();

  DValue args = this->__deserializer->call(type.argumentName());
  DValue value = object->call(name, args);

  this->__serializer->call(type.returnName(), value);
  this->__networkStream->call("flush");
}

void    ServerObject::call0(void)
{
  this->__id = this->__deserializer->call("DUInt64").get<DUInt64>();
  DObject* object = this->__objectManager->call("object", this->__id).get<DObject*>();
  DUnicodeString name = this->__deserializer->call("DUnicodeString").get<DUnicodeString>();

  Destruct::DValue value = object->call(name); 
  
  DType type = object->instanceOf()->attribute(name).type();
  this->__serializer->call(type.returnName(), value);
  this->__networkStream->call("flush");
}

void    ServerObject::functionCall(void)
{
  this->__id = this->__deserializer->call("DUInt64").get<DUInt64>();

  ServerFunctionObject* object = static_cast<ServerFunctionObject*>(this->__objectManager->call("object", this->__id).get<DObject*>());
  DValue args = this->__deserializer->call(DType((DType::Type_t)(DUInt64)object->argumentType).name()); //XXX get name directly ? 
  DValue value = ((DFunctionObject*)object->functionObject)->call(args);
  this->__serializer->call(DType((DType::Type_t)(DUInt64)object->returnType).name(), value); 

  this->__networkStream->call("flush");
}

void    ServerObject::functionCall0(void)
{
  this->__id = this->__deserializer->call("DUInt64").get<DUInt64>();

  ServerFunctionObject* object = static_cast<ServerFunctionObject*>(this->__objectManager->call("object", this->__id).get<DObject*>());

  DValue value = ((DFunctionObject*)object->functionObject)->call();
  this->__serializer->call(DType((DType::Type_t)(DUInt64)object->returnType).name(), value); 

  this->__networkStream->call("flush");
}

DObject*    ServerObject::networkStream(void)
{
  return (this->__networkStream);
}
