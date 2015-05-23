#include "dserializebinary.hpp"

#include "dstruct.hpp"
#include "dstructs.hpp"
#include "dobject.hpp"
#include "dsimpleobject.hpp"
#include "dnullobject.hpp"
#include "dmutableobject.hpp"
#include "drealvalue.hpp"
//#include "dserializeraw.hpp"
//#include "dserializexml.hpp"

#include <iostream>

namespace Destruct
{

SerializeBinary::SerializeBinary(DStruct* dstruct, DValue const& args) : DCppObject<SerializeBinary>(dstruct, args), __stream(args.get<DObject*>())
{
  this->init(); 
}

SerializeBinary::SerializeBinary(SerializeBinary const& copy) : DCppObject<SerializeBinary>(copy), __stream(copy.__stream)
{
  this->init();
}

SerializeBinary::~SerializeBinary()
{
  ((DObject*)__stream)->destroy();
}

void    SerializeBinary::sDObject(DValue const& args)
{
  DObject* dobject = args.get<DObject*>();
  if (dobject == NULL) //XXX XXX ct fait avant mais ca faisait quoi ? :)
    return (this->sDNone());
  

  DStruct const* dstruct = dobject->instanceOf(); 
  if (dstruct == NULL)
    throw DException("SerializeBinary::sDObject(DValue) object instance is NULL");
  this->sDUnicodeString(RealValue<DUnicodeString>(dstruct->name()));

  /*
   *  We follow special serialization and iterable method first 
   */
  int32_t index = dstruct->findAttribute("iterator");
  if (index != -1)
  {
    DObject* iterator = dobject->call("iterator").get<DObject*>();
    DType returnType = DType(iterator->instanceOf()->attribute("currentItem").type().getReturnType());

    DValue count = dobject->call("size");   
    this->sDUInt64(count);
 
    for ( ;iterator->call("isDone").get<DInt8>() != 1; iterator->call("nextItem"))
    {
      DValue value = iterator->call("currentItem");
      this->call(returnType.name(), value);
    }
    //iterator->destroy(); //destroy get<DObject*> instance
    //iterator->destroy(); //Destroy "call" instance
    //dobject->destroy();
    return ;
  }
  
  for (DStruct::DAttributeIterator attribute = dstruct->attributeBegin(); attribute != dstruct->attributeEnd(); ++attribute)
  {
    DType type = attribute->type();
    DType::Type_t type_t = type.getType();
    if (type_t == DType::DMethodType || type_t == DType::DNoneType || type_t == DType::DUnknownType)
      continue;
    else
    {
      DValue value = dobject->getValue((*attribute).name());
      this->call(type.name(), value);
    }
  }
  //dobject->destroy();
}

void    SerializeBinary::sDStruct(DValue const& args)
{
  DStruct* dstruct = args.get<DStruct*>();
//  if (dstruct == NULL)
//     throw DException

  this->sDUnicodeString(RealValue<DUnicodeString>(dstruct->name())); 

  size_t attributeCount = dstruct->attributeCount();
  this->sDUInt32(RealValue<DUInt32>((DUInt32)attributeCount)); //XXX size t

  for (DStruct::DAttributeIterator i = dstruct->attributeBegin(); i != dstruct->attributeEnd(); ++i)
  {
    this->sDUnicodeString(RealValue<DUnicodeString>((*i).name()));
    DType::Type_t type = (*i).type().getType();
    this->sDUInt8(RealValue<DUInt8>(type)); //DUInt8?
    if (type == DType::DMethodType)
    {
       DType::Type_t argumentType = (*i).type().getArgumentType();
       this->sDUInt8(RealValue<DUInt8>(argumentType));
       DType::Type_t returnType = (*i).type().getReturnType();
       this->sDUInt8(RealValue<DUInt8>(returnType));
    }
  } 
}

void    SerializeBinary::sDNone(void)
{
  this->sDObject(RealValue<DObject*>(DNone)); 
}

void    SerializeBinary::sDMethod(DValue const& args)
{
  //pass or throw ?
}

void    SerializeBinary::sDUnicodeString(DValue const& args)
{
  DObject* stream = this->__stream;

  DUnicodeString str = args.get<DUnicodeString>();
  RealValue<DInt64>  dsize((int64_t)str.size());

  stream->call("write", RealValue<DBuffer>(dsize.asDBuffer()));
  stream->call("write", RealValue<DBuffer>(args.asDBuffer()));
}

void    SerializeBinary::sDBuffer(DValue const& args)
{
  DObject* stream = this->__stream;
  //write size
  //write buffer data ? (for other only write data or do it for each so it self contained ??) 
  stream->call("write", args);
}

void    SerializeBinary::sDInt8(DValue const& args)
{
  DObject* stream = this->__stream;
  stream->call("write", RealValue<DBuffer>(args.asDBuffer()));
}

void    SerializeBinary::sDInt16(DValue const& args)
{
  DObject* stream = this->__stream;
  stream->call("write", RealValue<DBuffer>(args.asDBuffer()));
}

void    SerializeBinary::sDInt32(DValue const& args)
{
  DObject* stream = this->__stream;
  stream->call("write", RealValue<DBuffer>(args.asDBuffer()));
}

void    SerializeBinary::sDInt64(DValue const& args)
{
  DObject* stream = this->__stream;
  stream->call("write", RealValue<DBuffer>(args.asDBuffer()));
}

void    SerializeBinary::sDUInt8(DValue const& args)
{
  DObject* stream = this->__stream;
  stream->call("write", RealValue<DBuffer>(args.asDBuffer()));
}

void    SerializeBinary::sDUInt16(DValue const& args)
{
  DObject* stream = this->__stream;
  stream->call("write", RealValue<DBuffer>(args.asDBuffer()));
}

void    SerializeBinary::sDUInt32(DValue const& args)
{
  DObject* stream = this->__stream;
  stream->call("write", RealValue<DBuffer>(args.asDBuffer()));
}

void    SerializeBinary::sDUInt64(DValue const& args)
{
  DObject* stream = this->__stream;
  stream->call("write", RealValue<DBuffer>(args.asDBuffer()));
}

/**
 *  DeserializeBinary
 */
DeserializeBinary::DeserializeBinary(DStruct* dstruct, DValue const& args) : DCppObject<DeserializeBinary>(dstruct, args), __stream(args.get<DObject*>())
{
  this->init(); 
}

DeserializeBinary::DeserializeBinary(DeserializeBinary const& copy) : DCppObject<DeserializeBinary>(copy), __stream(copy.__stream)
{
  this->init();
}

DeserializeBinary::~DeserializeBinary()
{
  ((DObject*)__stream)->destroy();
}

DObject*        DeserializeBinary::dDObject(DValue const& value)
{
  DStruct* dstruct = NULL;
  DObject* dobject = value.get<DObject*>();
 
  if (dobject == DNone)
  {
    DStructs& dstructs = DStructs::instance();
    DUnicodeString structName = this->call("DUnicodeString").get<DUnicodeString>();

    if (structName == "None")
      return (RealValue<DObject*>(DNone));

    dstruct = dstructs.find(structName);
    if (dstruct == NULL)
      throw DException("Can't find struct : '" + structName + "' in destruct database.");
    dobject = dstruct->newObject();
  }
  else
    dstruct = dobject->instanceOf();
   
 
  int32_t index = dstruct->findAttribute("iterator");
  int32_t hasNewItem = dstruct->findAttribute("newItem");
  if (index != -1)
  {
    DType::Type_t   returnType = dstruct->attribute("get").type().getReturnType();
    DUInt64 count = this->call("DUInt64").get<DUInt64>();
   
    if (hasNewItem != -1)
    {
      for (DUInt64 index = 0; index < count; index++)
      {
         DUnicodeString structName = this->call("DUnicodeString").get<DUnicodeString>(); 
         DObject* item = dobject->call("newItem").get<DObject*>();
         this->call("DObject", RealValue<DObject*>(item));
         dobject->call("setItem", RealValue<DObject*>(item));
         //item->destroy();
         //item->destroy();
      }
    }
    else
    {
      for (DUInt64 index = 0; index < count; index++) 
      {
         
         DValue value = this->call(DType(returnType).name());
         dobject->call("push", value);
         //if (returnType == DType::DObjectType)
         //{
         //DObject* obj = value.get<DObject*>();
         //obj->destroy();
         //obj->destroy();
         //}
      }
    }
    return (dobject);
  }

  for (DStruct::DAttributeIterator attribute = dstruct->attributeBegin(); attribute != dstruct->attributeEnd(); ++attribute)
  {
    DType type = attribute->type();
    if (type.getType() == DType::DMethodType)
      continue;
    else if (type.getType() == DType::DUnknownType)
      continue;
    else
    {
      DValue value = this->call(type.name());
      dobject->setValue((*attribute).name(), value);
    }
    //if type un dobject
    //obj->destroy() * 2 ?
  }

  return (dobject);
}

DStruct*        DeserializeBinary::dDStruct(void)
{
  DStruct* dstruct = NULL; 

  DUnicodeString structName = this->call("DUnicodeString").get<DUnicodeString>();
  DUInt32 attributeCount = this->call("DUInt32").get<DUInt32>(); //32 ?

  if ((dstruct = new DStruct(0, structName, DSimpleObject::newObject)) == NULL)//inheritance ? 
    return (NULL);
  for (size_t i = 0; i < attributeCount; i++) 
  {
     DUnicodeString name = this->call("DUnicodeString").get<DUnicodeString>();
     
     DType::Type_t type = (DType::Type_t)this->call("DUInt8").get<DUInt8>(); //XXX type serialization ?
     if (type == DType::DMethodType)
     {
       DType::Type_t argumentType = (DType::Type_t)this->call("DUInt8").get<DUInt8>();
       DType::Type_t returnType = (DType::Type_t)this->call("DUInt8").get<DUInt8>();
       dstruct->addAttribute(DAttribute(returnType, name, argumentType, type));
     }
     else
       dstruct->addAttribute(DAttribute(type, name));
  }
  DStructs::instance().registerDStruct(dstruct); //needed for complex object that register struct and need it later to deserialize rest of file 
  return (dstruct); 
}

DObject*        DeserializeBinary::dDNone(void)
{
  return (DNone);
}

DFunctionObject* DeserializeBinary::dDMethod(void)
{
  return (NULL);
}

DUnicodeString  DeserializeBinary::dDUnicodeString(void)
{
  DObject* stream = this->__stream;

  DInt64  size = this->call("DInt64").get<DInt64>(); 
  DBuffer buffer = stream->call("read", RealValue<DInt64>(size)).get<DBuffer>();

  //char* data = new char[size + 1];
  //is.read(data, size);
  //data[size] = 0;
  //delete[] data;

  std::string str((const char*)buffer.data(), buffer.size()); //DUnicodeString constructor XXX
  //DUnicodeString string(dbuffer.data(), size);
  //delete DBuffer->data;

  return (str);
}

DBuffer         DeserializeBinary::dDBuffer(void)
{
  //DObject* stream = this->__stream;
  //stream->call("write", args);
  //read buffer size
  //read buffer data

  return DBuffer();
}

DInt8           DeserializeBinary::dDInt8(void)
{
  DObject* stream = this->__stream;
  DBuffer buffer = stream->call("read", RealValue<DInt64>(sizeof(DInt8))).get<DBuffer>();

  return (*((DInt8*)buffer.data()));
}

DInt16          DeserializeBinary::dDInt16(void)
{
  DObject* stream = this->__stream;
  DBuffer buffer = stream->call("read", RealValue<DInt64>(sizeof(DInt16))).get<DBuffer>();

  return (*((DInt16*)buffer.data()));
}

DInt32          DeserializeBinary::dDInt32(void)
{
  DObject* stream = this->__stream;
  DBuffer buffer = stream->call("read", RealValue<DInt64>(sizeof(DInt32))).get<DBuffer>();

  return (*((DInt32*)buffer.data()));
}

DInt64          DeserializeBinary::dDInt64(void)
{
  DObject* stream = this->__stream;
  DBuffer buffer = stream->call("read", RealValue<DInt64>(sizeof(DInt64))).get<DBuffer>();

  return (*((DInt64*)buffer.data()));
}

DUInt8          DeserializeBinary::dDUInt8(void)
{
  DObject* stream = this->__stream;
  DBuffer buffer = stream->call("read", RealValue<DInt64>(sizeof(DUInt8))).get<DBuffer>();

  return (*((DUInt8*)buffer.data()));
}

DUInt16         DeserializeBinary::dDUInt16(void)
{
  DObject* stream = this->__stream;
  DBuffer buffer = stream->call("read", RealValue<DInt64>(sizeof(DUInt16))).get<DBuffer>();

  return (*((DUInt16*)buffer.data()));
}

DUInt32         DeserializeBinary::dDUInt32(void)
{
  DObject* stream = this->__stream;
  DBuffer buffer = stream->call("read", RealValue<DInt64>(sizeof(DUInt32))).get<DBuffer>();

  return (*((DUInt32*)buffer.data()));
}

DUInt64         DeserializeBinary::dDUInt64(void)
{
  DObject* stream = this->__stream;
  DBuffer buffer = stream->call("read", RealValue<DInt64>(sizeof(DUInt64))).get<DBuffer>();
  
  return (*((DUInt64*)buffer.data()));
}

}
