#include "dserializebinary.hpp"

#include "dsimpleobject.hpp"

namespace Destruct
{

/**
 *  Binary serialization // marshaling
 *  This is the default serialization format for destruct 
 *  All object & protocol must be serializable and unserializable trough it's interface 
 */

DSerializeBinary::DSerializeBinary()
{
}

const DUnicodeString DSerializeBinary::name(void)
{
  return (DUnicodeString("Binary"));
}

DSerializeBinary*   DSerializeBinary::create(void)
{
  return (new DSerializeBinary);
}

/**
 *  Serialization
 */ 
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
       else if (returnType == DType::DStructType)
       {
         DStruct* subStruct = value.get<DStruct*>();
         if (subStruct != NULL)
           this->serialize(output, *subStruct);
         //else
          //XXX?
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
    else if (type == DType::DStructType)
    {
      DValue value = dobject->getValue((*attribute).name());
      DStruct* subStruct = value.get<DStruct*>();
      this->serialize(output, *subStruct);
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

/**
 * Deserialization
 */
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
  DStructs::instance().registerDStruct(dstruct); //needed for complex object that register struct and need it later to deserialize rest of file 
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
  if (dtype == DType::DStructType)
  {
    DStruct* dstruct = this->deserialize(input);
    return RealValue<DStruct*>(dstruct);
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
    else if (type == DType::DStructType)
    {
      DStruct* subStruct = this->deserialize(input, DType::DStructType).get<DStruct*>();
        
      if (subStruct)
        dobject->setValue((*attribute).name(), RealValue<DStruct*>(subStruct));
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



}
