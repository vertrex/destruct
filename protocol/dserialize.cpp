#include "dserialize.hpp"
#include "dstruct.hpp"
#include "destruct.hpp"
#include "dobject.hpp"
#include "dsimpleobject.hpp"
#include "dnullobject.hpp"
#include "dmutableobject.hpp"
#include "drealvalue.hpp"

#include <iostream>

namespace Destruct
{

/**
 *  Binary serialization // marshaling
 *  This is the default serialization format for destruct 
 *  All object & protocol must be serializable and unserializable trough it's interface 
 */
const std::string DSerializeBinary::name(void)
{
  return ("Binary");
}

DSerializeBinary*   DSerializeBinary::create(void)
{
  return (new DSerializeBinary);
}

//throw rather than many check // ? use ./open read rather than slow stream?
bool DSerializeBinary::serialize(DStream& output, DStruct& dstruct)
{
  if (this->serialize(output, dstruct.name()) == false)
    return (false);

  size_t attributeCount = dstruct.attributeCount(); 
  output.write((char *)&attributeCount, sizeof(attributeCount));

  for (DStruct::DAttributeIterator i = dstruct.attributeBegin(); i != dstruct.attributeEnd(); ++i)
  {
    if (this->serialize(output, (*i).name()) == false)
      return (false);
 
    DType::Type_t type = (*i).type().getType();
    if (output.write((char*)&type, sizeof(type)).fail())
      return (false);
    if (type == DType::DMethodType)
    {
       DType::Type_t argumentType = (*i).type().getArgumentType();
       DType::Type_t returnType = (*i).type().getReturnType();
       if (output.write((char*)&argumentType, sizeof(type)).fail())
         return (false);
       if (output.write((char*)&returnType, sizeof(type)).fail())
         return (false);
    }
  } 

  return (true);
}

DStruct* DSerializeBinary::deserialize(DStream& input)
{ 
  std::string name;
  size_t attributeCount;
  DStruct* dstruct = NULL; 

  if (this->deserialize(input, name) == false)
    return (NULL);
  if (input.read((char*)&attributeCount, sizeof(size_t)).fail())
    return (NULL);
 
  if ((dstruct = new DStruct(0, name, DSimpleObject::newObject)) == NULL) //XXX parent etc !!
    return (NULL);
  for (size_t i = 0; i < attributeCount; i++) 
  {
     DType::Type_t type;
     std::string   name;

     if (this->deserialize(input, name) == false)
       return (NULL);
     if (input.read((char*)&type, sizeof(type)).fail())
       return (NULL); //strange error checking !
     if (type == DType::DMethodType)
     {
       DType::Type_t argumentType;
       DType::Type_t returnType;
       if (input.read((char*)&argumentType, sizeof(type)).fail())
         return (NULL); //s
       if (input.read((char*)&returnType, sizeof(type)).fail())
         return (NULL); //s
       dstruct->addAttribute(DAttribute(returnType, name, argumentType, type));
     }
     else
       dstruct->addAttribute(DAttribute(type, name));
  }
  return (dstruct); 
}

bool DSerializeBinary::serialize(DStream& output, DValue value, DType::Type_t type)
{
  output << value;

  return (false);
}

DValue DSerializeBinary::deserialize(DStream& input, DType::Type_t dtype)
{
  DValue value(DType(dtype).newValue());
  input >> value;

  return (value);
}


bool DSerializeBinary::serialize(DStream& output, DObject&  dobject)
{
  return (true);
}

bool DSerializeBinary::deserialize(DStream& input, DObject& dobject)
{
  return (false);
}

//king of specialization for string type 

bool DSerializeBinary::serialize(DStream& output, const std::string& str)
{
 size_t ssize = str.size();
  
  if (output.write((char*)&ssize, sizeof(ssize)).fail())
    return (false);
  if (output.write(str.c_str(), ssize).fail())
    return (false);

  return (true);
}

bool DSerializeBinary::deserialize(DStream& input, std::string& str)
{
  size_t ssize;
  if (input.read((char*)&ssize, sizeof(size_t)).fail())
    return (false);

  char* buff = new char[ssize];

  if (buff == NULL)
    return (false);
  if (input.read(buff, ssize).fail())
    return (false);

  str = std::string(buff, ssize);
  delete[] buff;  

  return (true);
}



/**
 *   XML serialization
 */ 
const std::string DSerializeXML::name(void)
{
  return ("XML");
}

DSerializeXML*   DSerializeXML::create(void)
{
  return (new DSerializeXML(NULL, RealValue<DObject*>(DNone)));
}

bool DSerializeXML::serialize(DStream& output, DObject& dobject)
{
  return (this->serialize(output, dobject, 1));
}
 
bool DSerializeXML::serialize(DStream& output, DObject& dobject, int depth)
{
  int x = 0;
  DStruct const* dstruct = dobject.instanceOf();
  XMLTag tag = XMLTag(output, dstruct->name(), "\n", depth++, true);

  try 
  { 
    //if dobject.getValue("serialize") :
    DMutableObject* arguments = static_cast<DMutableObject*>(Destruct::instance().find("DMutable")->newObject());
    arguments->setValueAttribute(DType::DObjectType, "stream", RealValue<DObject*>(&output));
    arguments->setValueAttribute(DType::DUnicodeStringType, "type", RealValue<DUnicodeString>("XML"));
    //arguments->setAttributeValue(DType::DObjectType, "serializer", this);
    DValue output = dobject.call("serialize", RealValue<DObject*>(arguments));
    arguments->destroy();
  }
  catch (DException const& exception)
  {
    std::cout << "Can't find specific dserializer for " <<  dobject.instanceOf()->name() << " : " << exception.error() << std::endl;
  }

  for (DStruct::DAttributeIterator i = dstruct->attributeBegin(); i != dstruct->attributeEnd(); ++i, ++x)
  {
    if (i->type().getType() == DType::DObjectType)
    {
      DObject* subObject = dobject.getValue(x).get<DObject* >();

      if (subObject != NULL)
      {
        XMLTag xmlObj  = XMLTag(output, i->name(), "\n", depth, true);
        this->serialize(output, *subObject, depth + 1);
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
      XMLTag(output, i->name(), dobject.getValue(x).asUnicodeString(), depth);
  }   
 
  return (true);
}

bool DSerializeXML::serialize(DStream& output, DValue value, DType::Type_t type)
{
  return (false);
}

DValue DSerializeXML::deserialize(DStream& input, DType::Type_t type)
{
  DValue value;
  return value;
}

bool DSerializeXML::deserialize(DStream& input, DObject& dobject)
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

DStruct* DSerializeXML::deserialize(DStream& output)
{
  return (NULL);
}

/** 
 *  Text serialization
 */
const std::string DSerializeText::name(void)
{
  return ("Text");
}

DSerializeText*   DSerializeText::create(void)
{
  return (new DSerializeText);
}

bool DSerializeText::serialize(DStream& output, DObject&  dobject)
{
  return (this->serialize(output, dobject, 0));
}
 
bool DSerializeText::serialize(DStream& output, DObject& dobject, int depth)
{
  int x = 0;
  DStruct const* dstruct = dobject.instanceOf();

  output << std::string(2*depth, ' ') << dstruct->name() << std::endl;
  output << std::string(2*depth++, ' ') <<  "{" << std::endl;

  for (DStruct::DAttributeIterator i = dstruct->attributeBegin(); i != dstruct->attributeEnd(); ++i, ++x)
  {
    if (i->type().getType() == DType::DObjectType)
    {
      DObject* subDObject = dobject.getValue(x).get<DObject*>();
      output << std::string(2*depth, ' ') << i->type().name() << " " << i->name() << " = " <<  std::endl;
      if (subDObject != NULL)
      {
        this->serialize(output, *subDObject, depth + 1);
        subDObject->destroy();
      }
    }
    else
      output << std::string(2*depth, ' ') << i->type().name() << " " << i->name() << " = " << dobject.getValue(x).asUnicodeString() << ";" << std::endl;

   if (i->name() == "serializeText")
   {
     DMutableObject* arguments = static_cast<DMutableObject*>(Destruct::Destruct::instance().generate("DMutable"));

     if (arguments == NULL)
        std::cout << "Arguments is null " << std::endl;
     if ((DObject*)arguments == DNone)
        std::cout << "arguments id dnone " << std::endl;
     arguments->setValueAttribute(DType::DObjectType, "stream", RealValue<DObject*>(&output));
     arguments->setValueAttribute(DType::DInt32Type, "depth", RealValue<DInt32>(depth));
     DValue output = dobject.call("serializeText", RealValue<DObject*>(arguments));
     arguments->destroy();
   }
  }
 
  output << std::string(2*depth - 2, ' ') << "};" << std::endl;
  return (true);
}


bool DSerializeText::serialize(DStream& output, DValue value, DType::Type_t type)
{
  return (false);
}

DValue DSerializeText::deserialize(DStream& input, DType::Type_t type)
{
  DValue value;
  return value;
}

bool DSerializeText::deserialize(DStream& input, DObject& dobject)
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

DStruct* DSerializeText::deserialize(DStream& output)
{
  return (NULL);
}


/**
 *  Raw serialization (on disk/memory serialization without meta info on type 
 */
const std::string DSerializeRaw::name(void)
{
  return ("Raw");
}

DSerializeRaw*   DSerializeRaw::create(void)
{
  return (new DSerializeRaw);
}

bool DSerializeRaw::serialize(DStream& output, DObject&  dobject)
{
  int x = 0;
  DStruct const* dstruct = dobject.instanceOf(); 

  for (DStruct::DAttributeIterator i = dstruct->attributeBegin(); i != dstruct->attributeEnd(); ++i, ++x)
  {
    if (i->type().getType() == DType::DObjectType)
    {
      DObject* subDObject = dobject.getValue(x).get<DObject*>();

      if (subDObject != NULL)
      {
        this->serialize(output, *subDObject);
        subDObject->destroy();
      }
    }
    else
    {
      DValue value = dobject.getValue(x);
      output << value;
    }    
  }

  return (true);
}

bool DSerializeRaw::deserialize(DStream& input, DObject& dobject) 
{
  int x = 0;
  DStruct const* dstruct = dobject.instanceOf(); 

  for (DStruct::DAttributeIterator i = dstruct->attributeBegin(); i != dstruct->attributeEnd(); ++i, ++x)
  {
    if (i->type().getType() == DType::DObjectType)
    {
      DObject* subDObject = dobject.getValue(x).get<DObject*>();

      if (subDObject != NULL)
      {
        this->deserialize(input, *subDObject);
        subDObject->destroy();
      }
    }
    else
    {
      DValue value = dobject.getValue(x);
      input >> value;
      dobject.setValue(x, value); 
    }    
  }

  return (true);
}

bool DSerializeRaw::serialize(DStream& output, DValue value, DType::Type_t type)
{
  return (false);
}

DValue DSerializeRaw::deserialize(DStream& input, DType::Type_t type)
{
  DValue value;
  return value;
}

bool DSerializeRaw::serialize(DStream& output, DStruct& dstruct)
{
//throw notImplemented !
  return (true);
}

DStruct* DSerializeRaw::deserialize(DStream& output)
{
  return (NULL);
}

/* 
 *  Serializers factory 
 */ 
std::vector<DSerialize* > DSerializers::__serializers;
static DSerializers dserializers = DSerializers();

DSerializers::DSerializers()
{
  //type
  //Encoders ? != serializer ? 
  this->registerSerializer(new DSerializeXML(NULL, RealValue<DObject*>(DNone)));
  this->registerSerializer(new DSerializeText());
  this->registerSerializer(new DSerializeBinary());
  this->registerSerializer(new DSerializeRaw());
}

DSerializers::~DSerializers()
{
  for (std::vector<DSerialize* >::iterator i = __serializers.begin(); i != __serializers.end(); ++i)
  {
    //delete (*i);
  }
}

size_t DSerializers::count()
{
  return (__serializers.size());
}

DSerialize* DSerializers::to(size_t id)
{
  if (id >= __serializers.size())
    return (NULL); 
  return (__serializers[id]);
}

DSerialize* DSerializers::to(const std::string type)
{
  std::vector<DSerialize* >::iterator i;

  for (i = __serializers.begin() ; i != __serializers.end(); ++i)
  {
    if (type == (*i)->name())
      return ((*i)->create()); 
  }
  return (NULL); 
}

bool DSerializers::registerSerializer(DSerialize* serializer)
{
  __serializers.push_back(serializer);
  return (true);
}

}
