#include "serializerpc.hpp"
#include "networkstream.hpp"
#include "clientobject.hpp"
#include "serverobject.hpp"
#include "clientfunctionobject.hpp"

#include "dsimpleobject.hpp"
/*
 *   DSerializeRPC 
 */
namespace Destruct
{

SerializeRPC::SerializeRPC(DStruct* dstruct, DValue const& args) : DCppObject<SerializeRPC>(dstruct, args), __stream(args)
{
  this->init(); 
  this->__objectManager = DStructs::instance().find("ObjectManager")->newObject();
}

SerializeRPC::SerializeRPC(SerializeRPC const& copy) : DCppObject<SerializeRPC>(copy), __objectManager(copy.__objectManager), __stream(copy.__stream)
{
  this->init();
}

SerializeRPC::~SerializeRPC()
{
  this->__objectManager->destroy();
}

void    SerializeRPC::sDObject(DValue const& args)
{
  DObject* dobject = args.get<DObject*>();

  RealValue<DUInt64> id = this->__objectManager->call("registerObject", args);  
 
  this->sDUnicodeString(RealValue<DUnicodeString>(dobject->instanceOf()->name()));
  this->sDUInt64(id);
  dobject->destroy();
}


void    SerializeRPC::sDStruct(DValue const& args)
{
  DStruct* dstruct = args.get<DStruct*>();
//  if (dstruct == NULL)
//     throw DException

  this->sDUnicodeString(RealValue<DUnicodeString>(dstruct->name())); 

  size_t attributeCount = dstruct->attributeCount();
  this->sDUInt32(RealValue<DUInt32>((DInt32)attributeCount)); //XXX size t

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

void    SerializeRPC::sDNone(void)
{
  //Do nothing //it's ok for none method return type but for object return none could be necesseray to inform the client
}

void    SerializeRPC::sDMethod(DValue const& args)
{
  //to implement
}

void    SerializeRPC::sDUnicodeString(DValue const& args)
{
  DObject* stream = this->__stream;

  DUnicodeString str = args.get<DUnicodeString>();
  RealValue<DInt64>  dsize((int64_t)str.size());

  stream->call("write", RealValue<DBuffer>(dsize.asDBuffer()));
  if (dsize != 0)
    stream->call("write", RealValue<DBuffer>(args.asDBuffer()));
}

void    SerializeRPC::sDBuffer(DValue const& args)
{
  DObject* stream = this->__stream;
  DBuffer buffer = args.get<DBuffer>();
  RealValue<DInt64>  dsize((DInt64)buffer.size());

  stream->call("write", RealValue<DBuffer>(dsize.asDBuffer()));
  stream->call("write", args);
}

void    SerializeRPC::sDInt8(DValue const& args)
{
  DObject* stream = this->__stream;
  stream->call("write", RealValue<DBuffer>(args.asDBuffer()));
}

void    SerializeRPC::sDInt16(DValue const& args)
{
  DObject* stream = this->__stream;
  stream->call("write", RealValue<DBuffer>(args.asDBuffer()));
}

void    SerializeRPC::sDInt32(DValue const& args)
{
  DObject* stream = this->__stream;
  stream->call("write", RealValue<DBuffer>(args.asDBuffer()));
}

void    SerializeRPC::sDInt64(DValue const& args)
{
  DObject* stream = this->__stream;
  stream->call("write", RealValue<DBuffer>(args.asDBuffer()));
}

void    SerializeRPC::sDUInt8(DValue const& args)
{
  DObject* stream = this->__stream;
  stream->call("write", RealValue<DBuffer>(args.asDBuffer()));
}

void    SerializeRPC::sDUInt16(DValue const& args)
{
  DObject* stream = this->__stream;
  stream->call("write", RealValue<DBuffer>(args.asDBuffer()));
}

void    SerializeRPC::sDUInt32(DValue const& args)
{
  DObject* stream = this->__stream;
  stream->call("write", RealValue<DBuffer>(args.asDBuffer()));
}

void    SerializeRPC::sDUInt64(DValue const& args)
{
  DObject* stream = this->__stream;
  stream->call("write", RealValue<DBuffer>(args.asDBuffer()));
}

/**
 *  Deserialize RPC
 */

DeserializeRPC::DeserializeRPC(DStruct* dstruct, DValue const& args) : DCppObject<DeserializeRPC>(dstruct, args), __stream(args)
{
  this->init(); 
}

DeserializeRPC::DeserializeRPC(DeserializeRPC const& copy) : DCppObject<DeserializeRPC>(copy), __stream(copy.__stream)
{
  this->init();
}

DeserializeRPC::~DeserializeRPC()
{
}

DObject*        DeserializeRPC::dDObject(DValue const& value)
{
  DUnicodeString objectName = this->dDUnicodeString();
  DUInt64 id = this->dDUInt64();

  DStruct* dstruct = Destruct::DStructs::instance().find(objectName);
  if (dstruct == NULL)
  {
    if (objectName != "None")
      std::cout << "DSerializeRPC Can't deserialize object not find in base must get struct named :  " << objectName << std::endl;
    return (DNone);
  }
  
  DObject* serializer = DStructs::instance().find("SerializeRPC")->newObject(RealValue<DObject*>(this->__stream)); //XXX new object each time  -> slow 
  DObject* clientObject = new ClientObject(RealValue<DObject*>(this->__stream), RealValue<DObject*>(serializer), RealValue<DObject*>(this), id, dstruct);
  serializer->destroy(); 
 
  return (clientObject);
}

DStruct*        DeserializeRPC::dDStruct(void)
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

DObject*        DeserializeRPC::dDNone(void)
{
  //do nothing
  return (DNone);
}

DFunctionObject* DeserializeRPC::dDMethod(void)
{
  //Implemented in ClientObject
  return (NULL);
}

DUnicodeString  DeserializeRPC::dDUnicodeString(void)
{
  DObject* stream = this->__stream;

  DInt64  size = this->call("DInt64").get<DInt64>();
  if (size == 0)
    return ("");

  DBuffer buffer = stream->call("read", RealValue<DInt64>(size)).get<DBuffer>();
  return (std::string((const char*)buffer.data(), buffer.size()));
}

DBuffer         DeserializeRPC::dDBuffer(void)
{
  DObject* stream = this->__stream;

  DInt64  size = this->call("DInt64").get<DInt64>();
  return (stream->call("read", RealValue<DInt64>(size)).get<DBuffer>());
}

DInt8           DeserializeRPC::dDInt8(void)
{
  DObject* stream = this->__stream;
  DBuffer buffer = stream->call("read", RealValue<DInt64>(sizeof(DInt8))).get<DBuffer>();

  return (*((DInt8*)buffer.data()));
}

DInt16          DeserializeRPC::dDInt16(void)
{
  DObject* stream = this->__stream;
  DBuffer buffer = stream->call("read", RealValue<DInt64>(sizeof(DInt16))).get<DBuffer>();

  return (*((DInt16*)buffer.data()));
}

DInt32          DeserializeRPC::dDInt32(void)
{
  DObject* stream = this->__stream;
  DBuffer buffer = stream->call("read", RealValue<DInt64>(sizeof(DInt32))).get<DBuffer>();

  return (*((DInt32*)buffer.data()));
}

DInt64          DeserializeRPC::dDInt64(void)
{
  DObject* stream = this->__stream;
  DBuffer buffer = stream->call("read", RealValue<DInt64>(sizeof(DInt64))).get<DBuffer>();

  return (*((DInt64*)buffer.data()));
}

DUInt8          DeserializeRPC::dDUInt8(void)
{
  DObject* stream = this->__stream;
  DBuffer buffer = stream->call("read", RealValue<DInt64>(sizeof(DUInt8))).get<DBuffer>();

  return (*((DUInt8*)buffer.data()));
}

DUInt16         DeserializeRPC::dDUInt16(void)
{
  DObject* stream = this->__stream;
  DBuffer buffer = stream->call("read", RealValue<DInt64>(sizeof(DUInt16))).get<DBuffer>();

  return (*((DUInt16*)buffer.data()));
}

DUInt32         DeserializeRPC::dDUInt32(void)
{
  DObject* stream = this->__stream;
  DBuffer buffer = stream->call("read", RealValue<DInt64>(sizeof(DUInt32))).get<DBuffer>();

  return (*((DUInt32*)buffer.data()));
}

DUInt64         DeserializeRPC::dDUInt64(void)
{
  DObject* stream = this->__stream;
  DBuffer buffer = stream->call("read", RealValue<DInt64>(sizeof(DUInt64))).get<DBuffer>();

  return (*((DUInt64*)buffer.data()));
}

}
