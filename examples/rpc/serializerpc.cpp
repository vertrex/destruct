#include "serializerpc.hpp"
#include "networkstream.hpp"
#include "clientobject.hpp"
#include "serverobject.hpp"
#include "clientfunctionobject.hpp"

/*
 *   DSerializeRPC 
 */
namespace Destruct
{

DSerializeRPC::DSerializeRPC(NetworkStream& stream, ObjectManager<DObject*>& objects, ObjectManager<ServerFunctionObject*>& functionObjects) : __networkStream(stream), __objects(objects), __functionObjects(functionObjects), __streamString(DStreamString(Destruct::instance().find("DStreamString"), RealValue<DObject*>(DNone)))
{
}

DSerializeRPC*   DSerializeRPC::create(void)
{
  return (new DSerializeRPC(this->__networkStream, this->__objects, this->__functionObjects));
}

const std::string DSerializeRPC::name(void)
{
  return ("RPC");
}

/*
 *  Serialization
 */ 
bool DSerializeRPC::serialize(DStream& networkStream, DStruct& dstruct)
{
  DSerialize* binarySerializer = DSerializers::to("Binary");
  binarySerializer->serialize(this->__streamString, dstruct);
  this->__networkStream << this->__streamString;

  return (true);
}

bool DSerializeRPC::serialize(DStream& networkStream, DValue value, DType::Type_t type)
{
  if (type == DType::DNoneType)
    return (true);
  else if (type == DType::DObjectType)
  {
    DObject* dobject = value.get<DObject*>();
    this->serialize(this->__networkStream, dobject);
  }
  else if (type == DType::DMethodType)
  {
    DException("DFunctionType serialization not implemented");
  }
  else
  {
    this->__streamString.clear();
    this->__streamString << value;
    this->__networkStream << this->__streamString;
  } 
  this->__streamString.clear();

  return (true);
}

bool DSerializeRPC::serialize(DStream& networkStream, DFunctionObject* dfunctionObject, DType::Type_t argumentType, DType::Type_t returnType)
{
  ServerFunctionObject* serverFunctionObject = new ServerFunctionObject(dfunctionObject, argumentType, returnType);
  RealValue<DUInt64> id(this->__functionObjects.registerObject(serverFunctionObject));
  
  id.serialize(this->__streamString);
  this->__networkStream << this->__streamString;

  return (true);
}

bool DSerializeRPC::serialize(DStream& networkStream, DObject*  dobject) 
{
  RealValue<DUnicodeString> objectName(dobject->instanceOf()->name());
  RealValue<DUInt64> id(this->__objects.registerObject(dobject));

  objectName.serialize(this->__streamString);
  id.serialize(this->__streamString);
  this->__networkStream << this->__streamString;

  return (true);
}

/*
 *  Deserialization
 */
DStruct* DSerializeRPC::deserialize(DStream& input)
{ 
  this->__networkStream >> this->__streamString;
  DSerialize* binarySerializer = DSerializers::to("Binary");
  DStruct* dstruct = binarySerializer->deserialize(this->__streamString);

  return (dstruct);
}

DValue DSerializeRPC::deserialize(DStream& networkStream, DType::Type_t type)
{
  if (type == DType::DNoneType)
    return (RealValue<DObject*>(DNone));

  this->__networkStream >> this->__streamString;
  if (type == DType::DObjectType)
  {
    RealValue<DUnicodeString> objectName("");
    RealValue<DUInt64> id;
  
    objectName.unserialize(this->__streamString);
    id.unserialize(this->__streamString);

    DStruct* dstruct = Destruct::Destruct::instance().find(objectName);
    if (dstruct == NULL)
    {
      std::cout << "DSerializeRPC Can't deserialize object not find in base must get struct named :  " << objectName << std::endl;
      return RealValue<DObject*>(DNone);
    }

    return RealValue<DObject*>(new ClientObject(this->__networkStream, this, id, dstruct));
  }

  DValue value(DType(type).newValue());
  this->__streamString >> value;
  this->__streamString.clear();
  return (value);
}

DValue DSerializeRPC::deserialize(DStream& input, DType::Type_t argumentType, DType::Type_t returnType)
{
  this->__networkStream >> this->__streamString;

  RealValue<DUInt64> id;
  id.unserialize(this->__streamString);
  
  return (RealValue<DFunctionObject*>(new ClientFunctionObject(this->__networkStream, this, id, argumentType, returnType)));
}

bool DSerializeRPC::deserialize(DStream& input, DObject* dobject) //UNUSED //XXX must return a DObject can't construct it before !
{
 return (false);
}
}
