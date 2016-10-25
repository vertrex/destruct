#include "serializerpc.hpp"
#include "networkstream.hpp"
#include "stubobject.hpp"
#include "serverobject.hpp"
#include "stubfunctionobject.hpp"
#include "stubobject.hpp"
#include "stubstruct.hpp"

//#include "dsimpleobject.hpp"
/*
 *   DSerializeRPC 
 */
SerializeRPC::SerializeRPC(DStruct* dstruct, DValue const& args) : DCppObject<SerializeRPC>(dstruct, args), __stream(args), __networkStream(static_cast<NetworkStream*>((DObject*)args))
{
  this->init(); 
  this->__objectManager = DStructs::instance().find("ObjectManager")->newObject();
}

SerializeRPC::SerializeRPC(SerializeRPC const& rhs) : DCppObject<SerializeRPC>(rhs), __objectManager(rhs.__objectManager), __stream(rhs.__stream), __networkStream(static_cast<NetworkStream*>((DObject*)rhs.__stream))
{
  this->copy(this, rhs);
}

SerializeRPC::~SerializeRPC()
{
  this->__objectManager->destroy();
}

void    SerializeRPC::sDObject(DValue const& args)
{
  DObject* dobject = args;

  RealValue<DUInt64> id = this->__objectManager->call("registerObject", args);  
 
  this->sDUInt64(id);
  this->sDUnicodeString(RealValue<DUnicodeString>(dobject->instanceOf()->name()));
}


void    SerializeRPC::sDStruct(DValue const& args)
{
  DStruct* dstruct = args; //server check for NULL in find & generate but not for direct call XXX
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
    this->__networkStream->write(RealValue<DBuffer>(args.asDBuffer()));
}

void    SerializeRPC::sDBuffer(DValue const& args)
{
  this->__networkStream->write(args);
}

void    SerializeRPC::sDInt8(DValue const& args)
{
  this->__networkStream->write(RealValue<DBuffer>(args.asDBuffer()));
}

void    SerializeRPC::sDInt16(DValue const& args)
{
  this->__networkStream->write(RealValue<DBuffer>(args.asDBuffer()));
}

void    SerializeRPC::sDInt32(DValue const& args)
{
  this->__networkStream->write(RealValue<DBuffer>(args.asDBuffer()));
}

void    SerializeRPC::sDInt64(DValue const& args)
{
  this->__networkStream->write(RealValue<DBuffer>(args.asDBuffer()));
}

void    SerializeRPC::sDUInt8(DValue const& args)
{
  this->__networkStream->write(RealValue<DBuffer>(args.asDBuffer()));
}

void    SerializeRPC::sDUInt16(DValue const& args)
{
  this->__networkStream->write(RealValue<DBuffer>(args.asDBuffer()));
}

void    SerializeRPC::sDUInt32(DValue const& args)
{
  this->__networkStream->write(RealValue<DBuffer>(args.asDBuffer()));
}

void    SerializeRPC::sDUInt64(DValue const& args)
{
  this->__networkStream->write(RealValue<DBuffer>(args.asDBuffer()));
}

/**
 *  Deserialize RPC
 */

DeserializeRPC::DeserializeRPC(DStruct* dstruct, DValue const& args) : DCppObject<DeserializeRPC>(dstruct, args), __stream(args), __networkStream(static_cast<NetworkStream*>((DObject*)args))
{
  this->init(); 
}

DeserializeRPC::DeserializeRPC(DeserializeRPC const& rhs) : DCppObject<DeserializeRPC>(rhs), __stream(rhs.__stream), __networkStream(static_cast<NetworkStream*>((DObject*)rhs.__stream))
{
  this->copy(this, rhs);
}

DeserializeRPC::~DeserializeRPC()
{
}

DObject*        DeserializeRPC::dDObject(void)
{
  DUInt64 id = this->dDUInt64();
  DUnicodeString objectName = this->dDUnicodeString();
  DStruct* dstruct = NULL; 
  SerializeRPC* serializer = static_cast<SerializeRPC*>(DStructs::instance().find("SerializeRPC")->newObject(RealValue<DObject*>(this->__stream))); //XXX new object each time  -> slow reuse parent one client or server 
  try
  {
     dstruct = Destruct::DStructs::instance().find(objectName);
  }
  catch (DException const& exception)
  {
    this->__networkStream->flushRead();
    serializer->sDUInt8(RealValue<DUInt8>(CMD_FIND));
    serializer->sDUnicodeString(RealValue<DUnicodeString>(objectName));
    this->__networkStream->request();

    dstruct = this->dDStruct(); //register done in dDStruct 
  }
  
  DObject* clientObject = new StubObject(RealValue<DObject*>(this->__stream), RealValue<DObject*>(serializer), RealValue<DObject*>(this), id, dstruct);
  serializer->destroy(); 

  return (clientObject);
}

DStruct*        DeserializeRPC::dDStruct(void)
{
  DStruct* dstruct = NULL; 

  DUnicodeString structName = this->dDUnicodeString();
  DUInt32 attributeCount = this->dDUInt32();

                                  
  if ((dstruct = new StubStruct(0, structName, StubObject::newObject, this->__networkStream)) == NULL)//inheritance ? 
    return (NULL);
  for (size_t i = 0; i < attributeCount; i++) 
  {
     DUnicodeString name = this->dDUnicodeString();
     
     DType::Type_t type = (DType::Type_t)this->dDUInt8(); 
     if (type == DType::DMethodType)
     {
       DType::Type_t argumentType = (DType::Type_t)this->dDUInt8();
       DType::Type_t returnType = (DType::Type_t)this->dDUInt8();
       dstruct->addAttribute(DAttribute(returnType, name, argumentType, type));
     }
     else
       dstruct->addAttribute(DAttribute(type, name));
  }
  DStructs::instance().registerDStruct(dstruct); //register here ? 
  return (dstruct); 
}

DObject*        DeserializeRPC::dDNone(void)
{
  //do nothing
  return (DNone);
}

DFunctionObject* DeserializeRPC::dDMethod(void)
{
  //Implemented in StubObject
  return (NULL);
}

DUnicodeString  DeserializeRPC::dDUnicodeString(void)
{
  DBuffer buffer = this->__networkStream->read();
  return (std::string((const char*)buffer.data(), buffer.size()));
}

DBuffer         DeserializeRPC::dDBuffer(void)
{
  return (this->__networkStream->read());
}

DInt8           DeserializeRPC::dDInt8(void)
{
  DBuffer buffer = this->__networkStream->read();
  return (*((DInt8*)buffer.data()));
}

DInt16          DeserializeRPC::dDInt16(void)
{
  DBuffer buffer = this->__networkStream->read();
  return (*((DInt16*)buffer.data()));
}

DInt32          DeserializeRPC::dDInt32(void)
{
  DBuffer buffer = this->__networkStream->read();
  return (*((DInt32*)buffer.data()));
}

DInt64          DeserializeRPC::dDInt64(void)
{
  DBuffer buffer = this->__networkStream->read();
  return (*((DInt64*)buffer.data()));
}

DUInt8          DeserializeRPC::dDUInt8(void)
{
  DBuffer buffer = this->__networkStream->read();
  return (*((DUInt8*)buffer.data()));
}

DUInt16         DeserializeRPC::dDUInt16(void)
{
  DBuffer buffer = this->__networkStream->read();
  return (*((DUInt16*)buffer.data()));
}

DUInt32         DeserializeRPC::dDUInt32(void)
{
  DBuffer buffer = this->__networkStream->read();
  return (*((DUInt32*)buffer.data()));
}

DUInt64         DeserializeRPC::dDUInt64(void)
{
  DBuffer buffer = this->__networkStream->read();
  return (*((DUInt64*)buffer.data()));
}
