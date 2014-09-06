#include "rpcserver.hpp"

#include "serializerpc.hpp"
#include "destruct.hpp"

using namespace Destruct;

/* 
 *  RPCServer
 */
RPCServer::RPCServer(NetworkStream networkStream, ObjectManager & objectManager) : __networkStream(networkStream), __serializer(new DSerializeRPC(networkStream, objectManager))
{
}

void    RPCServer::findDStruct(void)
{
  std::string name;
//  std::cout << "RPCServer::findDStruct read name of struct " << std::endl;
  this->__networkStream.read(name);
//  std::cout << "RPCServer get  struct to find :  " << name << std::endl;
  Destruct::Destruct& destruct = Destruct::Destruct::instance();
  DStruct* dstruct = destruct.find(name);
  if (!dstruct)
   throw std::string("DStruct not found");

  std::cout << " serializing networkStream for struct " << name << std::endl;
  this->__serializer->serialize(this->__networkStream, *dstruct);
}

void    RPCServer::setValue(DObject* object)
{
  std::string name;
  this->__networkStream.read(name);

  Destruct::DValue value = this->__serializer->deserialize(this->__networkStream, object->instanceOf()->attribute(name).type().getType());

  object->setValue(name, value);
}

void    RPCServer::getValue(DObject* object)
{
  std::string name;
  this->__networkStream.read(name);

  Destruct::DValue value = object->getValue(name);

  DType type = object->instanceOf()->attribute(name).type();
  this->__serializer->serialize(this->__networkStream, value, type.getType());
}

void    RPCServer::call(DObject* object)
{
  std::string name;
  this->__networkStream.read(name);

  Destruct::DValue args = this->__serializer->deserialize(this->__networkStream, object->instanceOf()->attribute(name).type().getArgumentType());

  Destruct::DValue value = object->call(name, args); 

  DType type = object->instanceOf()->attribute(name).type();
  this->__serializer->serialize(this->__networkStream, value, type.getReturnType());
}

void    RPCServer::call0(DObject* object)
{
  std::string name;
  this->__networkStream.read(name);

  Destruct::DValue value = object->call(name); 
  
  DType type = object->instanceOf()->attribute(name).type();
  this->__serializer->serialize(this->__networkStream, value, type.getReturnType());
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
/*
 *  ObjectManager
 */
ObjectManager::ObjectManager() : __currentID(0)
{
}

uint64_t        ObjectManager::registerObject(DObject* object)
{
  std::map<uint64_t, DObject*>::const_iterator i= this->__objectsID.begin();
  for (; i != this->__objectsID.end(); ++i)
    if (i->second == object)
      return i->first;

  uint64_t id = this->__currentID;
  this->__objectsID[id] = object;
  this->__currentID++;
  return (id);
}

DObject*        ObjectManager::object(uint64_t id) const
{
  std::map<uint64_t, DObject*>::const_iterator object = this->__objectsID.find(id);
  if (object != this->__objectsID.end())
    return (object->second);
  return RealValue<DObject*>(DNone); 
}
