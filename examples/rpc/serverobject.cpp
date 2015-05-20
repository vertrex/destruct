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
  //this->__functionObjectManager = DStruct().find("FunctionObjectManager").newObject();
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
    ServerFunctionObject* serverFunctionObject = static_cast<ServerFunctionObject*>(DStructs::instance().find("ServerFunctionObject")->newObject());
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
  std::cout << "FUNCTION CALL XXX" << std::endl;
  this->__id = this->__deserializer->call("DUInt64").get<DUInt64>();
  //ServerFunctionObject* object = this->__functionObjectManager->call("object", this->__id).get<DFunctionObject*>(); //XXX XXX XXX

  //DValue args = this->__deserializer->call(DType(object->argumentType()).name()); //XXX create function argumentName() ! on server object
  //DValue value = object->functionObject()->call(args);
  //
  //this->__serializer->call(DType(object->returnType()).name(), value); //XXX create function returnName on ServerObject
  //
  //this->__networkStream->call("flush");
  std::cout << "HE WRITE DFUNCTION OBJECT MANAGER " << std::endl;
}

void    ServerObject::functionCall0(void)
{
  std::cout << "FUNCTION CALL 0 XXX" << std::endl;
  this->__id = this->__deserializer->call("DUInt64").get<DUInt64>();
  //ServerFunctionObject* object = this->__functionObjectManager->call("object", this->__id).get<DFunctionObject*>(); //XXX XXX XXX

  //Destruct::DValue value = object->functionObject()->call(); 
  // 
  //this->__serializer->call(DType(object->returnType()).name());       
  //this->__networkStream->call("flush");
  std::cout << "HE WRITE DFUNCTION OBJECT MANAGER " << std::endl;
}

DObject*    ServerObject::networkStream(void)
{
  return (this->__networkStream);
}
