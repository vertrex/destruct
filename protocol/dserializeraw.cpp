#include "dserializeraw.hpp"

namespace Destruct
{

/**
 *  Raw serialization (on disk/memory serialization without meta info on type 
 */
DSerializeRaw::DSerializeRaw()
{
}

const DUnicodeString DSerializeRaw::name(void)
{
  return ("Raw");
}

DSerializeRaw*   DSerializeRaw::create(void)
{
  return (new DSerializeRaw);
}

/**
 *  Serialization
 */
bool DSerializeRaw::serialize(DStream& output, DFunctionObject* value, DType::Type_t argumentType, DType::Type_t returnType)
{
  DException("Not implemented");
  return (false);
}

bool DSerializeRaw::serialize(DStream& output, DObject*  dobject)
{
  int x = 0;
  DStruct const* dstruct = dobject->instanceOf(); 

  for (DStruct::DAttributeIterator i = dstruct->attributeBegin(); i != dstruct->attributeEnd(); ++i, ++x)
  {
    if (i->type().getType() == DType::DObjectType)
    {
      DObject* subDObject = dobject->getValue(x).get<DObject*>();

      if (subDObject != NULL)
      {
        this->serialize(output, subDObject);
        subDObject->destroy();
      }
    }
    else
    {
      DValue value = dobject->getValue(x);
      this->serialize(output, value, i->type().getType());

      //output << value; //write value -> this->serialize(value, type)
    }    
  }

  return (true);
}

bool DSerializeRaw::serialize(DStream& output, DValue value, DType::Type_t type)
{
  //XXX
      //output << value; //write value -> this->serialize(value, type)
  return (false);
}

bool DSerializeRaw::serialize(DStream& output, DStruct& dstruct)
{
//throw notImplemented !
  return (true);
}

/**
 *  Deserilization
 */
bool DSerializeRaw::deserialize(DStream& input, DObject* dobject) 
{
  int x = 0;
  DStruct const* dstruct = dobject->instanceOf(); 

  int32_t index = dobject->instanceOf()->findAttribute("deserializeRaw");
  if (index != -1)
    return (dobject->call(index, (RealValue<DObject*>(&input))).get<DUInt8>() != 0);

  for (DStruct::DAttributeIterator i = dstruct->attributeBegin(); i != dstruct->attributeEnd(); ++i, ++x)
  {
    DType::Type_t type = i->type().getType(); 
    if (type == DType::DObjectType)
    {
      //set object before ?
      DObject* subDObject = dobject->getValue(x).get<DObject*>();

      if (subDObject != NULL)
      {
        this->deserialize(input, subDObject);
        subDObject->destroy();
        subDObject->destroy();
      }
    }
    else if (type == DType::DNoneType)
      continue;
    else if (type == DType::DMethodType)
      continue;
    else if (type == DType::DUnknownType)
      continue;
    else
    {
      DValue value = this->deserialize(input, type);
      //DValue value = dobject->getValue(x);
      //input >> value;
      //

      dobject->setValue(x, value); 
    }    
  }

  return (true);
}

DValue DSerializeRaw::deserialize(DStream& input, DType::Type_t type)
{
  //DValue value;
  DValue  value = DValue(DType(type).newValue());


  return value;
}

DValue DSerializeRaw::deserialize(DStream& input, DType::Type_t argumentType, DType::Type_t returnType)
{
  throw DException("DMethod deserialization not implemented.");
}

DStruct* DSerializeRaw::deserialize(DStream& output)
{
  return (NULL);
}

}
