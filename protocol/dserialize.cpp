#include "dserialize.hpp"
#include "dstruct.hpp"
#include "dstructs.hpp"
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
const DUnicodeString DSerializeBinary::name(void)
{
  return (DUnicodeString("Binary"));
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

bool DSerializeBinary::serialize(DStream& output, DFunctionObject* value, DType::Type_t argumentType, DType::Type_t returnType)
{
  DException("Not implemented");
  return (false);
}

bool DSerializeBinary::serialize(DStream& output, DValue value, DType::Type_t type) //XXX type is not needed here 
{
  throw std::string("UNUSED UNUSED UNUSED UNUSED Serialize(output, value, type)");
  /*if (type == DType::DObjectType)
  {
    DObject* object = value.get<DObject*>();
    this->serialize(output, object);
    object->destroy();
  }
  //else if (type == DType::DMethodType)
  //continue ;
  else if (type == DType::DNoneType)
    this->serialize(output, DNone);  
  //continue; //XXX
  //else (type == DType::DUnknownType)
  //continue;
  else
    output << value; 

  return (true);*/
}
                                                //const ref or pointer and const func ? 
bool DSerializeBinary::serialize(DStream& output, DObject*  dobject)
{
  DStruct const* dstruct = dobject->instanceOf(); 
  if (dstruct == NULL)
    throw DException("DSerializeBinary::serialize(DStream& output, DObject* dobject) object instance is NULL");
  this->serialize(output, dstruct->name()); //Serialize Struct name

  /*
   *  We follow special serialization and iterable method first 
   */
  int32_t index = dstruct->findAttribute("iterator");
  if (index != -1)
  {
    DObject* iterator = dobject->call("iterator").get<DObject*>();
    DType::Type_t   returnType = iterator->instanceOf()->attribute("currentItem").type().getReturnType();

    DValue count = dobject->call("size");   
    output << count; 
 
    for ( ; iterator->call("isDone").get<DInt8>() != 1; iterator->call("nextItem"))
    {
       DValue value = iterator->call("currentItem");
       if (returnType == DType::DObjectType)
       {
         DObject* subDObject = value.get<DObject*>();
         if (subDObject != NULL)
         {
           this->serialize(output, subDObject);
           subDObject->destroy(); //one for the get
           subDObject->destroy(); //one for the object :) 
         }
         else
           this->serialize(output, DNone);//avoid writting 0 arg etc... XXX
       }
       else
       {
         output << value;
       }
    }
    iterator->destroy(); //DETRUIT L' INSTANCE Cree par 'call'
    iterator->destroy(); //DETRUIT la ref retourner par 'get<DObject*>'
    return (true);
  }
  
  for (DStruct::DAttributeIterator attribute = dstruct->attributeBegin(); attribute != dstruct->attributeEnd(); ++attribute)
  {
    DType::Type_t type = attribute->type().getType();
    if (type == DType::DObjectType)
    {
      DValue value = dobject->getValue((*attribute).name());
      DObject* subDObject = value.get<DObject*>();

      if (subDObject != NULL)
      {
        this->serialize(output, subDObject); //XXX ? like that ?
        subDObject->destroy(); //XXX ? Simple ? 
      }
      else
        this->serialize(output, DNone);
    }
    else if (type == DType::DNoneType)
      this->serialize(output, DNone);
    else if (type == DType::DMethodType)
      continue;
    else if (type == DType::DUnknownType)
      continue;
    else
    {
      DValue value = dobject->getValue(attribute->name());
      output << value;
    }    
  }

  //std::cout << dobject->instanceOf()->name() << " serialized" << std::endl;
  return (true);
}
//king of specialization for string type 

bool DSerializeBinary::serialize(DStream& output, const DUnicodeString& str)
{
 size_t ssize = str.size();
  //utilisase 8 bytes pour une size ca fait bcp ... 
  if (output.write((char*)&ssize, sizeof(ssize)).fail())
    return (false);
  if (output.write(str.c_str(), (uint32_t)ssize).fail())
    return (false);

  return (true);
}

//Deserialization 
DStruct* DSerializeBinary::deserialize(DStream& input)
{
  DUnicodeString name;
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
     DUnicodeString name;

     if (this->deserialize(input, name) == false)
     {
       delete dstruct;
       return (NULL);
     }
     if (input.read((char*)&type, sizeof(type)).fail())
     {
       delete dstruct;
       return (NULL); //strange error checking !
     }
     if (type == DType::DMethodType)
     {
       DType::Type_t argumentType;
       DType::Type_t returnType;
       if (input.read((char*)&argumentType, sizeof(type)).fail())
       {
         delete dstruct;
         return (NULL); //s
       }
       if (input.read((char*)&returnType, sizeof(type)).fail())
       {
         delete dstruct;
         return (NULL); //s
       }
       dstruct->addAttribute(DAttribute(returnType, name, argumentType, type));
     }
     else
       dstruct->addAttribute(DAttribute(type, name));
  }
  return (dstruct); 
}

// Return a value that must be destroyed by caller
//must twice destroyed if use get<DObject*> who increment val
DValue DSerializeBinary::deserialize(DStream& input, DType::Type_t dtype)
{
  if (dtype == DType::DObjectType)
  {
    DStructs& destruct = DStructs::instance(); //mettre ds this car la ca va etre lent. 
    DUnicodeString structName;
    this->deserialize(input, structName);

    if (structName == "None")
      return (RealValue<DObject*>(DNone));
    DStruct* dstruct = destruct.find(structName);
    if (dstruct == NULL)
      throw DException("Can't find struct : " + structName + " in destruct database.");
   
    DObject* dobject = dstruct->newObject();
    this->deserialize(input, dobject);
    //dobject->destroy(); //must not destroy caller must destroy 
    return (RealValue<DObject*>(dobject));
  }
  
  DValue value(DType(dtype).newValue());
  input >> value;

  return (value);
}

DValue DSerializeBinary::deserialize(DStream& input, DType::Type_t argumentType, DType::Type_t returnType)
{
  throw DException("DMethod deserialization not implemented.");
}

bool DSerializeBinary::deserialize(DStream& input, DObject* dobject) //not implemtenable by ref ...
{
  DStruct const* dstruct = dobject->instanceOf();
  int32_t index = dstruct->findAttribute("iterator");
  int32_t hasNewItem = dstruct->findAttribute("newItem");
  if (index != -1)
  {
    DType::Type_t   returnType = dstruct->attribute("get").type().getReturnType();
  
    DValue vcount(RealValue<DUInt64>(0)); 
    input >> vcount; 
    DUInt64 count = vcount.get<DUInt64>();
   
    if (hasNewItem != -1)
    {
      for (DUInt64 index = 0; index < count; index++) 
      {
         DUnicodeString structName;
         this->deserialize(input, structName);
        
         DObject* item = dobject->call("newItem").get<DObject*>();
         //if object has newItem use newItem (for map ???) 
         this->deserialize(input, item);
         dobject->call("setItem", RealValue<DObject*>(item));
         item->destroy();
         item->destroy();
      }
    }
    else
    {
      for (DUInt64 index = 0; index < count; index++) 
      {
         DValue value = this->deserialize(input, returnType);
         dobject->call("push", value);
         if (returnType == DType::DObjectType) //XXX do for DFunction ! 
         {
           DObject* obj = value.get<DObject*>();
           obj->destroy();
           obj->destroy();
         }
      }
    }
    return (true);
  }

  for (DStruct::DAttributeIterator attribute = dstruct->attributeBegin(); attribute != dstruct->attributeEnd(); ++attribute)
  {
    DType::Type_t type = attribute->type().getType();
    if (type == DType::DObjectType)
    {
      //DValue value = this->deserialize(input, DType::DObjectType);
      //dobject->setValue((*attribute).name(), value);
      DObject* subObject = this->deserialize(input, DType::DObjectType).get<DObject*>();
      
      dobject->setValue((*attribute).name(), RealValue<DObject*>(subObject));
      subObject->destroy(); //ref return by deserialize
      subObject->destroy(); //ref returned by get<>
    }
    else if (type == DType::DNoneType)
      dobject->setValue((*attribute).name(), RealValue<DObject*>(DNone)); 
    else if (type == DType::DMethodType)
    {
      //std::cout << "deserializing method type not implem" << std::endl;
      continue;
    }
    else if (type == DType::DUnknownType)
    {
      //std::cout << "deserializing unknown type not implem" << std::endl;
      continue;
    }
    else
    {
      DValue value(attribute->type().newValue());
      input >> value;
      dobject->setValue((*attribute).name(), value); 
    }    
  }
  return (true);
}

bool DSerializeBinary::deserialize(DStream& input, DUnicodeString& str)
{
  size_t ssize;
  if (input.read((char*)&ssize, sizeof(size_t)).fail())
    return (false);

  char* buff = new char[ssize];

  if (buff == NULL)
    return (false);
  if (input.read(buff, (uint32_t)ssize).fail())
    return (false);

  str = std::string(buff, ssize);
  delete[] buff;  

  return (true);
}

/**
 *   XML serialization
 */ 
const DUnicodeString DSerializeXML::name(void)
{
  return ("XML");
}

DSerializeXML*   DSerializeXML::create(void)
{
  return (new DSerializeXML(NULL, RealValue<DObject*>(DNone)));
}

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
const DUnicodeString DSerializeText::name(void)
{
  return ("Text");
}

DSerializeText*   DSerializeText::create(void)
{
  return (new DSerializeText);
}

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
const DUnicodeString DSerializeRaw::name(void)
{
  return ("Raw");
}

DSerializeRaw*   DSerializeRaw::create(void)
{
  return (new DSerializeRaw);
}

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
      output << value;
    }    
  }

  return (true);
}

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
      DValue value = dobject->getValue(x);
      input >> value;
      dobject->setValue(x, value); 
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

DValue DSerializeRaw::deserialize(DStream& input, DType::Type_t argumentType, DType::Type_t returnType)
{
  throw DException("DMethod deserialization not implemented.");
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

DSerialize* DSerializers::to(const DUnicodeString type)
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
