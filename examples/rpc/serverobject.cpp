#include "serverobject.hpp"
#include "serializerpc.hpp"

using namespace Destruct;

/* 
 *  ServerObject
 */
//ServerObject::ServerObject(NetworkStream& networkStream, DObject* serializer, ObjectManager<DObject*>& objectManager, ObjectManager<ServerFunctionObject*>& functionObjectManager) : __networkStream(networkStream), __serializer(serializer),__objectManager(objectManager), __functionObjectManager(functionObjectManager), __object(NULL), __id(-1)
ServerObject::ServerObject(DObject* networkStream, DObject* serializer, ObjectManager<DObject*>& objectManager, ObjectManager<ServerFunctionObject*>& functionObjectManager) : __networkStream(networkStream), __serializer(serializer),__objectManager(objectManager), __functionObjectManager(functionObjectManager), __object(NULL), __id(-1)
{
}

void    ServerObject::setValue(void)
{
  this->__id = this->__deserializer->call("DUInt64").get<DUInt64>();
  DUnicodeString name = this->__deserializer->call("DUnicodeString").get<DUnicodeString>();

  DObject* object = this->__objectManager.object(this->__id);

  DValue value = this->__deserializer->call(object->instanceOf()->attribute(name).type().name());
  object->setValue(name, value);
}

void    ServerObject::getValue(void)
{
  this->__id = this->__deserializer->call("DUInt64").get<DUInt64>();
  DObject* object = this->__objectManager.object(this->__id);
  DUnicodeString name = this->__deserializer->call("DUnicodeString").get<DUnicodeString>();
   
  Destruct::DValue value = object->getValue(name);
  DType type = object->instanceOf()->attribute(name).type();
  //new serial
  //if (type.getType() == DType::DMethodType) //XXX
  //this->__serializer->serialize(this->__networkStream, value.get<DFunctionObject*>(), type.getArgumentType(), type.getReturnType());
  //else
    //this->__serializer->serialize(this->__networkStream, value, type.getType());
    this->__serializer->call(type.name(), value);

  this->__networkStream->call("flush"); 
}

void    ServerObject::call(void)
{
  this->__id = this->__deserializer->call("DUInt64").get<DUInt64>();
  DObject* object = this->__objectManager.object(this->__id);
  DUnicodeString name = this->__deserializer->call("DUnicodeString").get<DUnicodeString>();

  DValue args = this->__deserializer->call(object->instanceOf()->attribute(name).type().name());
  DValue value = object->call(name, args);

  DType type = object->instanceOf()->attribute(name).type();
  this->__serializer->call(type.returnName(), value);
  this->__networkStream->call("flush");
}

void    ServerObject::call0(void)
{
  this->__id = this->__deserializer->call("DUInt64").get<DUInt64>();
  DObject* object = this->__objectManager.object(this->__id);
  DUnicodeString name = this->__deserializer->call("DUnicodeString").get<DUnicodeString>();

  Destruct::DValue value = object->call(name); 
  
  DType type = object->instanceOf()->attribute(name).type();
  this->__serializer->call(type.returnName(), value);
  this->__networkStream->call("flush");
}

void    ServerObject::functionCall(void)
{
  this->__id = this->__deserializer->call("DUInt64").get<DUInt64>();
  ServerFunctionObject* object = this->__functionObjectManager.object(this->__id);

  DValue args = this->__deserializer->call(DType(object->argumentType()).name()); //XXX create function argumentName() ! on server object
  DValue value = object->functionObject()->call(args);

  this->__serializer->call(DType(object->returnType()).name(), value); //XXX create function returnName on ServerObject

  this->__networkStream->call("flush");
}

void    ServerObject::functionCall0(void)
{
  this->__id = this->__deserializer->call("DUInt64").get<DUInt64>();
  ServerFunctionObject* object = this->__functionObjectManager.object(this->__id);

  Destruct::DValue value = object->functionObject()->call(); 
  
  this->__serializer->call(DType(object->returnType()).name());       
  this->__networkStream->call("flush");
}

DObject*    ServerObject::networkStream(void)
{
  return (this->__networkStream);
}
