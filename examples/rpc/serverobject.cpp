#include "serverobject.hpp"
#include "serializerpc.hpp"

using namespace Destruct;

/* 
 *  ServerObject
 */
ServerObject::ServerObject(NetworkStream networkStream, ObjectManager<DObject*>& objectManager, ObjectManager<ServerFunctionObject*>& functionObjectManager) : __networkStream(networkStream), __serializer(new DSerializeRPC(networkStream, objectManager, functionObjectManager))
{
}

void    ServerObject::findDStruct(void)
{
  std::string name;
  this->__networkStream.read(name);

  std::cout << "Send DStruct " << name << std::endl;
  Destruct::Destruct& destruct = Destruct::Destruct::instance();
  DStruct* dstruct = destruct.find(name);
  if (!dstruct)
   throw std::string("DStruct not found");

  this->__serializer->serialize(this->__networkStream, *dstruct);
}

void    ServerObject::setValue(DObject* object)
{
  std::string name;
  this->__networkStream.read(name);


  Destruct::DValue value = this->__serializer->deserialize(this->__networkStream, object->instanceOf()->attribute(name).type().getType());

  std::cout << object->instanceOf()->name() << ".setValue(\"" << name  <<  "\", " << value.asUnicodeString() << ")" << std::endl;
  object->setValue(name, value);
}

void    ServerObject::getValue(DObject* object)
{
  std::string name;
  this->__networkStream.read(name);
  
  Destruct::DValue value = object->getValue(name);
  std::cout << object->instanceOf()->name() << ".getValue(\"" << name << "\") => " << value.asUnicodeString() << std::endl;

  DType type = object->instanceOf()->attribute(name).type();
  if (type.getType() == DType::DMethodType)
    this->__serializer->serialize(this->__networkStream, value.get<DFunctionObject*>(), type.getArgumentType(), type.getReturnType());
  else
    this->__serializer->serialize(this->__networkStream, value, type.getType());
}

void    ServerObject::call(DObject* object)
{
  std::string name;
  this->__networkStream.read(name);

  Destruct::DValue args = this->__serializer->deserialize(this->__networkStream, object->instanceOf()->attribute(name).type().getArgumentType());

  Destruct::DValue value = object->call(name, args); 
  std::cout << object->instanceOf()->name() << ".call(\"" << name << "\", " << args.asUnicodeString() << ") => " << value.asUnicodeString() << std::endl;

  DType type = object->instanceOf()->attribute(name).type();
  this->__serializer->serialize(this->__networkStream, value, type.getReturnType());
}

void    ServerObject::call0(DObject* object)
{
  std::string name;
  this->__networkStream.read(name);

  Destruct::DValue value = object->call(name); 
  std::cout << object->instanceOf()->name() << ".call(\"" << name << "\") => " << value.asUnicodeString() << std::endl;
  
  DType type = object->instanceOf()->attribute(name).type();
  this->__serializer->serialize(this->__networkStream, value, type.getReturnType());
}

void    ServerObject::functionCall(ServerFunctionObject* object)
{
  Destruct::DValue args = this->__serializer->deserialize(this->__networkStream, object->argumentType());
  Destruct::DValue value = object->functionObject()->call(args); 
        
  std::cout << "functionCall(" << args.asUnicodeString() << ") => " << value.asUnicodeString() << std::endl;
  this->__serializer->serialize(this->__networkStream, value, object->returnType());
}

void    ServerObject::functionCall0(ServerFunctionObject* object)
{
  Destruct::DValue value = object->functionObject()->call(); 
  std::cout << "functionCall() => " << value.asUnicodeString() << std::endl;
         
  this->__serializer->serialize(this->__networkStream, value, object->returnType());
}

void    ServerObject::unknown(const std::string& cmd)
{
  std::cout << "Receive unknown command : " << cmd << std::endl;
  this->__networkStream.write("Unknown command : " + cmd);
}

NetworkStream&    ServerObject::networkStream(void)
{
  return (this->__networkStream);
}


