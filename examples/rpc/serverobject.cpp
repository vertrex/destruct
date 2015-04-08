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
  this->__networkStream.read(&this->__id); 
  DUnicodeString name;
  this->__networkStream.read(name);

  DObject* object = this->__objectManager.object(this->__id);
  Destruct::DValue value = this->__serializer->deserialize(this->__networkStream, object->instanceOf()->attribute(name).type().getType());

  object->setValue(name, value);
}

void    ServerObject::getValue(void)
{
  this->__networkStream.read(&this->__id); 
  DObject* object = this->__objectManager.object(this->__id);
  DUnicodeString  name;
  this->__networkStream.read(name);
  
  Destruct::DValue value = object->getValue(name);

  DType type = object->instanceOf()->attribute(name).type();
  if (type.getType() == DType::DMethodType)
    this->__serializer->serialize(this->__networkStream, value.get<DFunctionObject*>(), type.getArgumentType(), type.getReturnType());
  else
    this->__serializer->serialize(this->__networkStream, value, type.getType());
  this->__networkStream.flush(); 
}

void    ServerObject::call(void)
{
  this->__networkStream.read(&this->__id); 
  DObject* object = this->__objectManager.object(this->__id);
  DUnicodeString name;
  this->__networkStream.read(name);

  Destruct::DValue args = this->__serializer->deserialize(this->__networkStream, object->instanceOf()->attribute(name).type().getArgumentType());
  Destruct::DValue value = object->call(name, args); 

  DType type = object->instanceOf()->attribute(name).type();
  this->__serializer->serialize(this->__networkStream, value, type.getReturnType());
  this->__networkStream.flush(); 
}

void    ServerObject::call0(void)
{
  this->__networkStream.read(&this->__id); 
  DObject* object = this->__objectManager.object(this->__id);
  DUnicodeString name;
  this->__networkStream.read(name);

  Destruct::DValue value = object->call(name); 
  
  DType type = object->instanceOf()->attribute(name).type();
  this->__serializer->serialize(this->__networkStream, value, type.getReturnType());
  this->__networkStream.flush(); 
}

void    ServerObject::functionCall(void)
{
  this->__networkStream.read(&this->__id); 
  ServerFunctionObject* object = this->__functionObjectManager.object(this->__id);

  Destruct::DValue args = this->__serializer->deserialize(this->__networkStream, object->argumentType());
  Destruct::DValue value = object->functionObject()->call(args); 
        
  this->__serializer->serialize(this->__networkStream, value, object->returnType());
  this->__networkStream.flush(); 
}

void    ServerObject::functionCall0(void)
{
  this->__networkStream.read(&this->__id); 
  ServerFunctionObject* object = this->__functionObjectManager.object(this->__id);

  Destruct::DValue value = object->functionObject()->call(); 
         
  this->__serializer->serialize(this->__networkStream, value, object->returnType());
  this->__networkStream.flush(); 
}

NetworkStream&    ServerObject::networkStream(void)
{
  return (this->__networkStream);
}
