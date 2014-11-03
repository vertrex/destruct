#include "rpcserver.hpp"

#include "serializerpc.hpp"
#include "destruct.hpp"
#include "rpcobject.hpp"
using namespace Destruct;

/* 
 *  RPCServer
 */
RPCServer::RPCServer(NetworkStream networkStream, ObjectManager<DObject*>& objectManager, ObjectManager<ServerFunctionObject*>& functionObjectManager) : __networkStream(networkStream), __serializer(new DSerializeRPC(networkStream, objectManager, functionObjectManager))
{
}

void    RPCServer::findDStruct(void)
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

void    RPCServer::setValue(DObject* object)
{
  std::string name;
  this->__networkStream.read(name);


  Destruct::DValue value = this->__serializer->deserialize(this->__networkStream, object->instanceOf()->attribute(name).type().getType());

  std::cout << object->instanceOf()->name() << ".setValue(\"" << name  <<  "\", " << value.asUnicodeString() << ")" << std::endl;
  object->setValue(name, value);
}

void    RPCServer::getValue(DObject* object)
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

void    RPCServer::call(DObject* object)
{
  std::string name;
  this->__networkStream.read(name);

  Destruct::DValue args = this->__serializer->deserialize(this->__networkStream, object->instanceOf()->attribute(name).type().getArgumentType());

  Destruct::DValue value = object->call(name, args); 
  std::cout << object->instanceOf()->name() << ".call(\"" << name << "\", " << args.asUnicodeString() << ") => " << value.asUnicodeString() << std::endl;

  DType type = object->instanceOf()->attribute(name).type();
  this->__serializer->serialize(this->__networkStream, value, type.getReturnType());
}

void    RPCServer::call0(DObject* object)
{
  std::string name;
  this->__networkStream.read(name);

  Destruct::DValue value = object->call(name); 
  std::cout << object->instanceOf()->name() << ".call(\"" << name << "\") => " << value.asUnicodeString() << std::endl;
  
  DType type = object->instanceOf()->attribute(name).type();
  this->__serializer->serialize(this->__networkStream, value, type.getReturnType());
}

void    RPCServer::functionCall(ServerFunctionObject* object)
{
  Destruct::DValue args = this->__serializer->deserialize(this->__networkStream, object->argumentType());
  Destruct::DValue value = object->functionObject()->call(args); 
        
  std::cout << "functionCall(" << args.asUnicodeString() << ") => " << value.asUnicodeString() << std::endl;
  this->__serializer->serialize(this->__networkStream, value, object->returnType());
}

void    RPCServer::functionCall0(ServerFunctionObject* object)
{
  Destruct::DValue value = object->functionObject()->call(); 
  std::cout << "functionCall() => " << value.asUnicodeString() << std::endl;
         
  this->__serializer->serialize(this->__networkStream, value, object->returnType());
}

void    RPCServer::unknown(const std::string& cmd)
{
  std::cout << "Receive unknown command : " << cmd << std::endl;
  this->__networkStream.write("Unknown command : " + cmd);
}

NetworkStream&    RPCServer::networkStream(void)
{
  return (this->__networkStream);
}

/**
 * Object Manager specialization
 */
template<>
DObject* ObjectManager<DObject* >::object(uint64_t id) const
{
  std::map<uint64_t, DObject* >::const_iterator object = this->__objectsID.find(id);
  if (object != this->__objectsID.end())
    return (object->second);
  return RealValue<DObject*>(DNone); 
}
