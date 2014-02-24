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

/*
 *   XML serialization
 */ 

const std::string DSerializeXML::name(void)
{
  return ("XML");
}

DSerializeXML*   DSerializeXML::create(void)
{
  return (new DSerializeXML);
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

  //special serialization
  //if dobject.serialize:
  //{
  //dobject.serialize(DStream& output, this);
  //return ;
  //} 
  //default serialization

//try catch ? mettre ds iterator / dcontainer ? 
  
///

  DMutableObject* arguments = static_cast<DMutableObject*>(Destruct::instance().find("DMutable")->newObject());
  //arguments->setValueAttribute(DType::DObjectType, "stream", stream);
  //arguments->setAttributeValue(DType::DObjectType, "serializer", serializer);
  arguments->setValueAttribute(DType::DUnicodeStringType, "serializerType", RealValue<DUnicodeString>("XML"));

  //output = dobject.call("serialize", "XML", stream, serializer);

  DObject* iterator =  dobject.call("iterator").get<DObject*>();
  if (iterator != DNone)
  {
   std::cout << "serializating an iterable " << std::endl;
   output << "<list>" << std::endl;
   while(!(iterator->call("isDone").get<DInt8>()))
   {
     DValue item = iterator->call("currentItem");
     iterator->call("nextItem");
     output  << item.asUnicodeString() << ",";
     //XMLTag(output, ",", item.asUnicodeString(), depth);
   }
   output << std::endl << "</list>" << std::endl;
  }
///


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
      //const std::string tag = i->type().returnName() + " " + i->type().name() + "(" + i->type().argumentName() + ") *"; //test pour voir... can't get value so affiche le type ?  ou juste DMethod* ?  plutot utile ds dserialize DStruct ?
      //XMLTag(output, i->name(), tag, depth);
    //}
    else    
      //Dserialize(value) //sauf dobject 
      XMLTag(output, i->name(), dobject.getValue(x).asUnicodeString(), depth);
  }   
 
  return (true);
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

/* 
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
  }
 
  output << std::string(2*depth - 2, ' ') << "};" << std::endl;
  return (true);
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

/*
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

bool DSerializeBinary::serialize(DStream& output, DObject&  dobject)
{
  return (true);
}

bool DSerializeBinary::deserialize(DStream& input, DObject& dobject)
{
  return (false);
}

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

     dstruct->addAttribute(DAttribute(type, name));
  }
  return (dstruct); 
}


/*
 *  Raw serialization 
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
  this->registerSerializer(new DSerializeXML());
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
