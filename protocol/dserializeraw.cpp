#include "dserializeraw.hpp"

namespace Destruct
{
/**
 *  Raw serialization (on disk/memory serialization without meta info on type 
 */


SerializeRaw::SerializeRaw(DStruct* dstruct, DValue const& args) : DCppObject<SerializeRaw>(dstruct, args), __stream(args)
{
  this->init(); 
}

SerializeRaw::SerializeRaw(SerializeRaw const& copy) : DCppObject<SerializeRaw>(copy), __stream(copy.__stream)
{
  this->init();
}

SerializeRaw::~SerializeRaw()
{
  //((DObject*)__stream)->destroy();
}

void    SerializeRaw::sDObject(DValue const& args)
{
  DObject* dobject = args.get<DObject*>();
  if (dobject == NULL)
    return ; 
  
  DStruct const* dstruct = dobject->instanceOf(); 
  if (dstruct == NULL)
  {
    dobject->destroy();
    throw DException("SerializeRaw::sDObject(DValue) object instance is NULL");
  }

  /*
   *  We follow special serialization and iterable method first 
   */
  //XXX write serialization for iterator protocol ?
  //XXX call serializeRaw method for special object serialization ?

  int i = 0;
  for (DStruct::DAttributeIterator attribute = dstruct->attributeBegin(); attribute != dstruct->attributeEnd(); ++attribute, ++i)
  {
    DType type = attribute->type();
    DType::Type_t type_t = type.getType();
    if (type_t == DType::DMethodType || type_t == DType::DUnknownType || type_t == DType::DStructType)
      continue;
    else
    {
      DValue value = dobject->getValue(i);
      this->call(type.name(), value);
    }
  }
  dobject->destroy();
}

void    SerializeRaw::sDStruct(DValue const& args)
{
  //No DStruct serialization
}

void    SerializeRaw::sDNone(void)
{
  //Serialize NULL ?
}

void    SerializeRaw::sDMethod(DValue const& args)
{
  //No method serialization
}

void    SerializeRaw::sDUnicodeString(DValue const& args)
{
  //Write string directly
  DObject* stream = this->__stream;

  DUnicodeString str = args.get<DUnicodeString>();
  stream->call("write", RealValue<DBuffer>(args.asDBuffer()));
}

void    SerializeRaw::sDBuffer(DValue const& args)
{
  DObject* stream = this->__stream;
  //write size
  //write buffer data ? (for other only write data or do it for each so it self contained ??) 
  stream->call("write", args);
}

void    SerializeRaw::sDInt8(DValue const& args)
{
  DObject* stream = this->__stream;
  stream->call("write", RealValue<DBuffer>(args.asDBuffer()));
}

void    SerializeRaw::sDInt16(DValue const& args)
{
  DObject* stream = this->__stream;
  stream->call("write", RealValue<DBuffer>(args.asDBuffer()));
}

void    SerializeRaw::sDInt32(DValue const& args)
{
  DObject* stream = this->__stream;
  stream->call("write", RealValue<DBuffer>(args.asDBuffer()));
}

void    SerializeRaw::sDInt64(DValue const& args)
{
  DObject* stream = this->__stream;
  stream->call("write", RealValue<DBuffer>(args.asDBuffer()));
}

void    SerializeRaw::sDUInt8(DValue const& args)
{
  DObject* stream = this->__stream;
  stream->call("write", RealValue<DBuffer>(args.asDBuffer()));
}

void    SerializeRaw::sDUInt16(DValue const& args)
{
  DObject* stream = this->__stream;
  stream->call("write", RealValue<DBuffer>(args.asDBuffer()));
}

void    SerializeRaw::sDUInt32(DValue const& args)
{
  DObject* stream = this->__stream;
  stream->call("write", RealValue<DBuffer>(args.asDBuffer()));
}

void    SerializeRaw::sDUInt64(DValue const& args)
{
  DObject* stream = this->__stream;
  stream->call("write", RealValue<DBuffer>(args.asDBuffer()));
}

/**
 *  DeserializeRaw
 */
DeserializeRaw::DeserializeRaw(DStruct* dstruct, DValue const& args) : DCppObject<DeserializeRaw>(dstruct, args), __stream(args)
{
  this->init(); 
}

DeserializeRaw::DeserializeRaw(DeserializeRaw const& copy) : DCppObject<DeserializeRaw>(copy), __stream(copy.__stream)
{
  this->init();
}

DeserializeRaw::~DeserializeRaw()
{
 //((DObject*)__stream)->destroy();
}

DObject*        DeserializeRaw::dDObject(DValue const& value)
{
  int x = 0;
  DObject* dobject = value.get<DObject*>();
  DStruct const* dstruct = dobject->instanceOf(); 

  int32_t index = dobject->instanceOf()->findAttribute("deserializeRaw");
  if (index != -1)
  {
    return (dobject->call(index, RealValue<DObject*>(this)).get<DObject*>()); //XXX refcount 
   //dobject->destroy
  }

  for (DStruct::DAttributeIterator i = dstruct->attributeBegin(); i != dstruct->attributeEnd(); ++i, ++x)
  {
    DType type = i->type();
    DType::Type_t type_t = type.getType();
    if (type_t == DType::DNoneType || type_t == DType::DMethodType || type_t == DType::DUnknownType)
      continue;
    else if (type_t == DType::DObjectType)
    {
      this->call(type.name(), dobject->getValue(x));
    }
    else
    {
      DValue value = this->call(type.name());
      dobject->setValue(x, value); 
    }    
  }
  //dobject->destroy();

  return (dobject);//XXX get it and destroy it or return DValue !
}

DStruct*        DeserializeRaw::dDStruct(void)
{
  //No DStruct serialization 
  return (NULL);
}

DObject*        DeserializeRaw::dDNone(void)
{
  //Deserialize NULL ?
  return (DNone);
}

DFunctionObject* DeserializeRaw::dDMethod(void)
{
  //No Method deserialization
  return (NULL);
}

DUnicodeString  DeserializeRaw::dDUnicodeString(void)
{
  //read until \\0 ?
 //XXX
 //DObject* stream = this->__stream;

 //DInt64  size = this->call("DInt64").get<DInt64>(); 
 //DBuffer buffer = stream->call("read", RealValue<DInt64>(size)).get<DBuffer>();
 //
 //return (str);
  return (DUnicodeString());
}

DBuffer         DeserializeRaw::dDBuffer(void)
{
  //read buffer until size ?
  return DBuffer();
}

DInt8           DeserializeRaw::dDInt8(void)
{
  DObject* stream = this->__stream;
  DBuffer buffer = stream->call("read", RealValue<DInt64>(sizeof(DInt8))).get<DBuffer>();

  return (*((DInt8*)buffer.data()));
}

DInt16          DeserializeRaw::dDInt16(void)
{
  DObject* stream = this->__stream;
  DBuffer buffer = stream->call("read", RealValue<DInt64>(sizeof(DInt16))).get<DBuffer>();

  return (*((DInt16*)buffer.data()));
}

DInt32          DeserializeRaw::dDInt32(void)
{
  DObject* stream = this->__stream;
  DBuffer buffer = stream->call("read", RealValue<DInt64>(sizeof(DInt32))).get<DBuffer>();

  return (*((DInt32*)buffer.data()));
}

DInt64          DeserializeRaw::dDInt64(void)
{
  DObject* stream = this->__stream;
  DBuffer buffer = stream->call("read", RealValue<DInt64>(sizeof(DInt64))).get<DBuffer>();

  return (*((DInt64*)buffer.data()));
}

DUInt8          DeserializeRaw::dDUInt8(void)
{
  DObject* stream = this->__stream;
  DBuffer buffer = stream->call("read", RealValue<DInt64>(sizeof(DUInt8))).get<DBuffer>();

  return (*((DUInt8*)buffer.data()));
}

DUInt16         DeserializeRaw::dDUInt16(void)
{
  DObject* stream = this->__stream;
  DBuffer buffer = stream->call("read", RealValue<DInt64>(sizeof(DUInt16))).get<DBuffer>();

  return (*((DUInt16*)buffer.data()));
}

DUInt32         DeserializeRaw::dDUInt32(void)
{
  DObject* stream = this->__stream;
  DBuffer buffer = stream->call("read", RealValue<DInt64>(sizeof(DUInt32))).get<DBuffer>();

  return (*((DUInt32*)buffer.data()));
}

DUInt64         DeserializeRaw::dDUInt64(void)
{
  DObject* stream = this->__stream;
  DBuffer buffer = stream->call("read", RealValue<DInt64>(sizeof(DUInt64))).get<DBuffer>();
  
  return (*((DUInt64*)buffer.data()));
}

}
