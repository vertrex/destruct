#include "dserializexml.hpp"

#include "dmutableobject.hpp"

namespace Destruct
{

/**
 *   XML serialization
 */
DSerializeXML::DSerializeXML(DStruct* dstruct, DValue const& args)// : DCppObject<DSerializeXML>(dstruct, args)
{
 // this->init();
}

DSerializeXML::DSerializeXML(const DSerializeXML& copy) //: DCppObject<DSerializeXML>(copy) 
{
 // this->init();
}
 
const DUnicodeString DSerializeXML::name(void)
{
  return ("XML");
}

DSerializeXML*   DSerializeXML::create(void)
{
  return (new DSerializeXML(NULL, RealValue<DObject*>(DNone)));
}

/**
 *  Serialization
 */
bool DSerializeXML::serialize(DStream& output, DObject* dobject)
{
  return (this->serialize(output, dobject, 1));
}
 
bool DSerializeXML::serialize(DStream& output, DFunctionObject* value, DType::Type_t argumentType, DType::Type_t returnType)
{
  DException("Not implemented");
  return (false);
}

bool DSerializeXML::serialize(DStream& output, DObject* dobject, int depth)
{
  int x = 0;
  DStruct const* dstruct = dobject->instanceOf();
  XMLTag tag = XMLTag(output, dstruct->name(), "\n", depth++, true);

  try 
  { 
    //if dobject.getValue("serialize") :
    DMutableObject* arguments = static_cast<DMutableObject*>(DStructs::instance().find("DMutable")->newObject());
    arguments->setValueAttribute(DType::DObjectType, "stream", RealValue<DObject*>(&output));
    arguments->setValueAttribute(DType::DUnicodeStringType, "type", RealValue<DUnicodeString>("XML"));
    //arguments->setAttributeValue(DType::DObjectType, "serializer", this);
    DValue output = dobject->call("serialize", RealValue<DObject*>(arguments));
    arguments->destroy();
  }
  catch (DException const& exception)
  {
    std::cout << "Can't find specific dserializer for " <<  dobject->instanceOf()->name() << " : " << exception.error() << std::endl;
  }

  for (DStruct::DAttributeIterator i = dstruct->attributeBegin(); i != dstruct->attributeEnd(); ++i, ++x)
  {
    if (i->type().getType() == DType::DObjectType)
    {
      DObject* subObject = dobject->getValue(x).get<DObject* >();

      if (subObject != NULL)
      {
        XMLTag xmlObj  = XMLTag(output, i->name(), "\n", depth, true);
        this->serialize(output, subObject, depth + 1);
        subObject->destroy();
      }
    }
    //else if(i->type().getType() == DType::DMethodType)
    //{
      //const std::string tag = i->type().returnName() + " " + i->type().name() + "(" + i->type().argumentName() + ") *"; 
      //XMLTag(output, i->name(), tag, depth);
    //}
    else    
      //Dserialize(value) //sauf dobject 
      XMLTag(output, i->name(), dobject->getValue(x).asUnicodeString(), depth);
  }   
 
  return (true);
}

bool DSerializeXML::serialize(DStream& output, DValue value, DType::Type_t type)
{
  return (false);
}

bool DSerializeXML::serialize(DStream& output, DStruct& dstruct)
{
  XMLTag ref = XMLTag(output, dstruct.name(), "\n", 1, true);

  for (DStruct::DAttributeIterator i = dstruct.attributeBegin(); i != dstruct.attributeEnd(); ++i)
  {
    XMLTag(output, (*i).name(), (*i).type().name(), 2); 
  } 

  return (true);
}

/**
 *  Deserialization
 */
DValue DSerializeXML::deserialize(DStream& input, DType::Type_t type)
{
  DValue value;
  return value;
}

DValue DSerializeXML::deserialize(DStream& input, DType::Type_t argumentType, DType::Type_t returnType)
{
  throw DException("DMethod deserialization not implemented.");
}

bool DSerializeXML::deserialize(DStream& input, DObject* dobject)
{
  return (false);
}

DStruct* DSerializeXML::deserialize(DStream& output)
{
  return (NULL);
}

}
