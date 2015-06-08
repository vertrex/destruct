#include "serverobject.hpp"
#include "serializerpc.hpp"
#include "serverfunctionobject.hpp"

using namespace Destruct;

/* 
 *  ServerObject
 */
ServerObject::ServerObject(DValue const& args): __networkStream(NULL), __serializer(NULL), __deserializer(NULL), __objectManager(NULL)
{
  this->__objectManager = DStructs::instance().find("ObjectManager")->newObject();
  this->__networkStream = DStructs::instance().generate("NetworkStream", args);
  this->__serializer = DStructs::instance().generate("SerializeRPC", RealValue<DObject*>(this->__networkStream));
  this->__deserializer = DStructs::instance().generate("DeserializeRPC", RealValue<DObject*>(this->__networkStream));
}

ServerObject::~ServerObject()
{
  this->__objectManager->destroy();
  this->__serializer->destroy();
  this->__deserializer->destroy();
  this->__networkStream->destroy();
}

void    ServerObject::setValue(void)
{
  DValue id = this->__deserializer->call("DUInt64");
  DUnicodeString name = this->__deserializer->call("DUnicodeString").get<DUnicodeString>();

  DObject* object = this->__objectManager->call("object", id).get<DObject*>();

  DValue value = this->__deserializer->call(object->instanceOf()->attribute(name).type().name());
  object->setValue(name, value);
  object->destroy();
}

void    ServerObject::getValue(void)
{
  DValue id = this->__deserializer->call("DUInt64");
  DUnicodeString name = this->__deserializer->call("DUnicodeString").get<DUnicodeString>();
  DObject* object = this->__objectManager->call("object", id).get<DObject*>();
  
  DValue value = object->getValue(name);

  DType type = object->instanceOf()->attribute(name).type();
  if (type.getType() == DType::DMethodType)
  {
    ServerFunctionObject* serverFunctionObject = static_cast<ServerFunctionObject*>(DStructs::instance().find("ServerFunctionObject")->newObject()); //DStruct get ?
    serverFunctionObject->argumentType = type.getArgumentType();
    serverFunctionObject->returnType = type.getReturnType();
    serverFunctionObject->functionObject = value.get<DFunctionObject*>();

    DValue id = this->__objectManager->call("registerObject", RealValue<DObject*>(serverFunctionObject));
    this->__serializer->call("DUInt64", id);
    serverFunctionObject->destroy();
  }
  else
    this->__serializer->call(type.name(), value);
  this->__networkStream->call("flush");
  object->destroy(); 
}

void    ServerObject::call(void)
{
  DValue id = this->__deserializer->call("DUInt64");
  DObject* object = this->__objectManager->call("object", id).get<DObject*>();
  DUnicodeString name = this->__deserializer->call("DUnicodeString").get<DUnicodeString>();
  DType type = object->instanceOf()->attribute(name).type();

  DValue args = this->__deserializer->call(type.argumentName());
  DValue value = object->call(name, args);

  this->__serializer->call(type.returnName(), value);
  this->__networkStream->call("flush");
  object->destroy();
}

void    ServerObject::call0(void)
{
  DValue id = this->__deserializer->call("DUInt64");
  DObject* object = this->__objectManager->call("object", id).get<DObject*>();
  DUnicodeString name = this->__deserializer->call("DUnicodeString").get<DUnicodeString>();

  DValue value = object->call(name); 
  
  DType type = object->instanceOf()->attribute(name).type();
  this->__serializer->call(type.returnName(), value);
  this->__networkStream->call("flush");
  object->destroy();
}

void    ServerObject::functionCall(void)
{
  DValue id= this->__deserializer->call("DUInt64");

  ServerFunctionObject* object = static_cast<ServerFunctionObject*>(this->__objectManager->call("object", id).get<DObject*>());
  DValue args = this->__deserializer->call(DType((DType::Type_t)(DUInt64)object->argumentType).name()); //XXX get name directly ? 
  DValue value = ((DFunctionObject*)object->functionObject)->call(args);
  this->__serializer->call(DType((DType::Type_t)(DUInt64)object->returnType).name(), value); 

  this->__networkStream->call("flush");
  object->destroy();
}

void    ServerObject::functionCall0(void)
{
  DValue id = this->__deserializer->call("DUInt64");

  ServerFunctionObject* object = static_cast<ServerFunctionObject*>(this->__objectManager->call("object", id).get<DObject*>());

  DValue value = ((DFunctionObject*)object->functionObject)->call();
  this->__serializer->call(DType((DType::Type_t)(DUInt64)object->returnType).name(), value); 

  this->__networkStream->call("flush");
  object->destroy();
}

void    ServerObject::findDStruct(void)
{
  DUnicodeString name = this->__deserializer->call("DUnicodeString").get<DUnicodeString>(); 
  std::cout << "Send DStruct " << name << std::endl;
  Destruct::DStructs& destruct = Destruct::DStructs::instance();
  DStruct* dstruct = destruct.find(name);
  if (!dstruct)
   throw DException("Server::findDStruct DStruct not found");

  this->__serializer->call("DStruct", RealValue<DStruct*>(dstruct));
  this->__networkStream->call("flush");
}
 
void    ServerObject::unknown(DUnicodeString const& cmd)
{
  std::cout << "Receive unknown command : " << cmd << std::endl;

  this->__serializer->call("DUnicodeString", RealValue<DUnicodeString>("Unknown command : " + cmd));
  this->__networkStream->call("flush");
}

DUnicodeString  ServerObject::cmd(void)
{
  return (this->__deserializer->call("DUnicodeString").get<DUnicodeString>());
}
