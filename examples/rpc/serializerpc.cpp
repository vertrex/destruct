#include "serializerpc.hpp"
#include "networkstream.hpp"
#include "dsimpleobject.hpp"
#include "destruct.hpp"
#include "rpcobject.hpp"
#include "rpcserver.hpp"

/*
 *   DSerializeRPC 
 */
namespace Destruct
{

DSerializeRPC::DSerializeRPC(NetworkStream& stream, ObjectManager & objects) : __networkStream(stream), __objects(objects), __streamString(DStreamString(Destruct::instance().find("DStreamString"), RealValue<DObject*>(DNone)))
{
}

DSerializeRPC*   DSerializeRPC::create(void)
{
  return (new DSerializeRPC(this->__networkStream, this->__objects));
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
  if (type == DType::DObjectType)
  {
    DObject* dobject = value.get<DObject*>();
    this->serialize(this->__networkStream, *dobject);
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

bool DSerializeRPC::serialize(DStream& networkStream, DObject&  dobject) 
{
  RealValue<DUnicodeString> objectName(dobject.instanceOf()->name());
  RealValue<DUInt64> id(this->__objects.registerObject(&dobject));

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
      std::cout << "Can't deserialize object not find in base must get struct named :  " << objectName << std::endl;
      return RealValue<DObject*>(DNone);
    } 
    return RealValue<DObject*>(new RPCObject(this->__networkStream, id, dstruct, this->__objects));
  }
  DValue value(DType(type).newValue());
  this->__streamString >> value;
  this->__streamString.clear();

  return (value);
}

bool DSerializeRPC::deserialize(DStream& input, DObject& dobject) //UNUSED //XXX must return a DOBject can't construct it before !
{

}
}
