#include "dserializetext.hpp"

#include "dunicodestring.hpp"

/** 
 *  Text serialization
 */

namespace Destruct
{

DSerializeText::DSerializeText()
{
}

const DUnicodeString DSerializeText::name(void)
{
  return ("Text");
}

DSerializeText*   DSerializeText::create(void)
{
  return (new DSerializeText);
}

/**
 * Serialization
 */
bool DSerializeText::serialize(DStream& output, DObject*  dobject)
{
//if dstruct == null
  output << dobject->instanceOf()->name() << std::endl << "{" << std::endl;
  bool result = this->serialize(output, dobject, 1);
  output << "};" << std::endl; 
 
  return (result);
}
 
bool DSerializeText::serialize(DStream& output, DFunctionObject* value, DType::Type_t argumentType, DType::Type_t returnType)
{
  DException("Not implemented");
  return (false);
}

bool DSerializeText::serialize(DStream& output, DObject* dobject, int depth)
{
  int x = 0;
  DStruct const* dstruct = dobject->instanceOf();

  if (dstruct == NULL)
    throw DException("DSerializeText::serialize(DStream& output, DObject* dobject) object instance is NULL");

  int32_t index = dobject->instanceOf()->findAttribute("iterator");
  if (index != -1)
  {
    DObject* iterator = dobject->call("iterator").get<DObject*>();
    DType::Type_t   returnType = iterator->instanceOf()->attribute("currentItem").type().getReturnType();

    DValue count = dobject->call("size");

  //  output << count.asUnicdeString(); // affiche XXX (count) ? 
    for ( ; iterator->call("isDone").get<DInt8>() != 1; iterator->call("nextItem"))
    {
       DValue value = iterator->call("currentItem");
       if (returnType == DType::DObjectType)
       {
         DObject* subDObject = value.get<DObject*>();
         if (subDObject != NULL)
         {
           output << std::string(2 * depth, ' ') << subDObject->instanceOf()->name() << " " <<  std::endl << std::string(2 * depth, ' ') << "{" <<  std::endl;
           //output << std::string(2 * depth, ' ') << subDObject->instanceOf()->name() << " " << count or index << " = " <<  std::endl << std::string(2 * depth, ' ') << "{" <<  std::endl;
           this->serialize(output, subDObject, depth + 1);
           output << std::string(2 * depth, ' ') << "};" << std::endl;
           subDObject->destroy(); //one for the get
           subDObject->destroy(); //one for the object :) 
         }
         //else
         //this->serialize(output, DNone);// afficher DNone ? 
       }
       else
         output << std::string(2 * depth, ' ') << value.asUnicodeString() << "," << std::endl;
    }
    iterator->destroy();
    iterator->destroy();
    return (true);
  }
  //else
  for (DStruct::DAttributeIterator i = dstruct->attributeBegin(); i != dstruct->attributeEnd(); ++i, ++x)
  {
    if (i->type().getType() == DType::DObjectType)
    {
      DObject* subDObject = dobject->getValue(x).get<DObject*>();
      if (subDObject != NULL)
      {
        output << std::string(2 * depth, ' ') << subDObject->instanceOf()->name() << " " << i->name() << " = " <<  std::endl << std::string(2 * depth, ' ') << "{" <<  std::endl;
        this->serialize(output, subDObject, depth + 1);
        subDObject->destroy();
        output << std::string(2 * depth, ' ') << "};" << std::endl; 
      }
      //else serialize dnone
    }
    else 
    {
      output << std::string(2 * depth, ' ') << i->type().name() << " " << i->name() << " = " << dobject->getValue(x).asUnicodeString() << ";" << std::endl;
    }
  }
 
  return (true);
}

bool DSerializeText::serialize(DStream& output, DValue value, DType::Type_t type)
{
  return (false);
}

bool DSerializeText::serialize(DStream& output, DStruct& dstruct)
{
  output << dstruct.name() << std::endl;
  output << "{" << std::endl;

  for (DStruct::DAttributeIterator i = dstruct.attributeBegin(); i != dstruct.attributeEnd(); ++i)
  {
    output << "  " << (*i).type().name() << " " << (*i).name() << ";" << std::endl;
  } 
  output << "}" << std::endl;

  return (true);
}

/**
 *  Deserialization
 */
DValue DSerializeText::deserialize(DStream& input, DType::Type_t type)
{
  DValue value;
  return value;
}

DValue DSerializeText::deserialize(DStream& input, DType::Type_t argumentType, DType::Type_t returnType)
{
  throw DException("DMethod deserialization not implemented.");
}

bool DSerializeText::deserialize(DStream& input, DObject* dobject)
{
  return (false);
}

DStruct* DSerializeText::deserialize(DStream& output)
{
  return (NULL);
}

}
