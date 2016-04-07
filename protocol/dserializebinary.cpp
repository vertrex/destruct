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
}

void    SerializeBinary::sDObject(DValue const& args)
{
  DObject* dobject = args;
  //if (dobject == NULL)
  //return (this->sDNone());

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
    DObject* iterator = dobject->call("iterator");
    DType returnType = DType(iterator->instanceOf()->attribute("currentItem").type().getReturnType());

    DValue count = dobject->call("size");   
    this->sDUInt64(count);
 
    for ( ;iterator->call("isDone").get<DInt8>() != 1; iterator->call("nextItem"))
    {
      DValue value = iterator->call("currentItem");
      this->call(returnType.name(), value);
    }
    iterator->destroy(); 
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
}

void    SerializeBinary::sDStruct(DValue const& args)
{
  DStruct* dstruct = args;
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

  DUnicodeString str = args;
  RealValue<DInt64>  dsize((int64_t)str.size());

  stream->call("write", RealValue<DBuffer>(dsize.asDBuffer()));
  stream->call("write", RealValue<DBuffer>(args.asDBuffer()));
}

void    SerializeBinary::sDBuffer(DValue const& args)
{
  //XXX implement me
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
}

DObject*        DeserializeBinary::dDObject(DValue const& value)
{
  DStruct* dstruct = NULL;
  DObject* dobject = value;

  if (dobject == DNone)
  {
    DStructs& dstructs = DStructs::instance();
    DUnicodeString structName = this->call("DUnicodeString");

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
    DUInt64 count = this->call("DUInt64");
   
    if (hasNewItem != -1)
    {
      for (DUInt64 index = 0; index < count; index++)
      {
       DUnicodeString structName = this->call("DUnicodeString"); //deserialize type
       DObject* item = dobject->call("newItem"); //usefull ??? if registered why not used registered type because it'swritten
       this->call("DObject", RealValue<DObject*>(item));
       dobject->call("setItem", RealValue<DObject*>(item));
       item->destroy();
      }
    }
    else
    {
      for (DUInt64 index = 0; index < count; index++) 
      {
         DValue value = this->call(DType(returnType).name());
         if (DType(returnType).getType() == DType::DObjectType)
         {
            DObject* ovalue = value;
            dobject->call("push", value);
            ovalue->destroy();
         }
         else
           dobject->call("push", value);
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
    //if type == dobject destroy ?
    else if (type.getType() == DType::DObjectType)
    {
      DValue value = this->call(type.name());
      DObject* ovalue = value;
      dobject->setValue((*attribute).name(), value);
      ovalue->destroy();
    }
    else
    {
      DValue value = this->call(type.name());
      dobject->setValue((*attribute).name(), value);
    }
  }
  return (dobject);
}

DStruct*        DeserializeBinary::dDStruct(void)
{
  DStruct* dstruct = NULL; 

  DUnicodeString structName = this->call("DUnicodeString");
  DUInt32 attributeCount = this->call("DUInt32");

  if ((dstruct = new DStruct(0, structName, DSimpleObject::newObject)) == NULL)//inheritance ? 
    return (NULL);
  for (size_t i = 0; i < attributeCount; i++) 
  {
     DUnicodeString name = this->call("DUnicodeString");
     
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

  DInt64  size = this->call("DInt64"); 
  DBuffer buffer = stream->call("read", RealValue<DInt64>(size));

  std::string str((const char*)buffer.data(), buffer.size()); //DUnicodeString constructor XXX
  //DUnicodeString string(dbuffer.data(), size);
  //delete DBuffer->data;

  return (str);
}

DBuffer         DeserializeBinary::dDBuffer(void)
{
  //XXX implement me
  //DObject* stream = this->__stream;
  //stream->call("write", args);
  //read buffer size
  //read buffer data

  return DBuffer();
}

DInt8           DeserializeBinary::dDInt8(void)
{
  DObject* stream = this->__stream;
  DBuffer buffer = stream->call("read", RealValue<DInt64>(sizeof(DInt8)));

  return (*((DInt8*)buffer.data()));
}

DInt16          DeserializeBinary::dDInt16(void)
{
  DObject* stream = this->__stream;
  DBuffer buffer = stream->call("read", RealValue<DInt64>(sizeof(DInt16)));

  return (*((DInt16*)buffer.data()));
}

DInt32          DeserializeBinary::dDInt32(void)
{
  DObject* stream = this->__stream;
  DBuffer buffer = stream->call("read", RealValue<DInt64>(sizeof(DInt32)));

  return (*((DInt32*)buffer.data()));
}

DInt64          DeserializeBinary::dDInt64(void)
{
  DObject* stream = this->__stream;
  DBuffer buffer = stream->call("read", RealValue<DInt64>(sizeof(DInt64)));

  return (*((DInt64*)buffer.data()));
}

DUInt8          DeserializeBinary::dDUInt8(void)
{
  DObject* stream = this->__stream;
  DBuffer buffer = stream->call("read", RealValue<DInt64>(sizeof(DUInt8)));

  return (*((DUInt8*)buffer.data()));
}

DUInt16         DeserializeBinary::dDUInt16(void)
{
  DObject* stream = this->__stream;
  DBuffer buffer = stream->call("read", RealValue<DInt64>(sizeof(DUInt16)));

  return (*((DUInt16*)buffer.data()));
}

DUInt32         DeserializeBinary::dDUInt32(void)
{
  DObject* stream = this->__stream;
  DBuffer buffer = stream->call("read", RealValue<DInt64>(sizeof(DUInt32)));

  return (*((DUInt32*)buffer.data()));
}

DUInt64         DeserializeBinary::dDUInt64(void)
{
  DObject* stream = this->__stream;
  DBuffer buffer = stream->call("read", RealValue<DInt64>(sizeof(DUInt64)));
  
  return (*((DUInt64*)buffer.data()));
}

}
