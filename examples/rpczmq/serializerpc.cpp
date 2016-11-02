#include "serializerpc.hpp"
#include "networkstream.hpp"
#include "stubobject.hpp"
#include "serverobject.hpp"
#include "stubfunctionobject.hpp"
#include "stubobject.hpp"
#include "stubstruct.hpp"

#include "czmq.h"
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

DOpaque   SerializeRPC::sDObject(DValue const& args)
{
  DObject* dobject = args;

  RealValue<DUInt64> id = this->__objectManager->call("registerObject", args);  
 
  zmsg_t* msg = (zmsg_t*)this->sDUInt64(id);
  zmsg_t* submsg = (zmsg_t*)this->sDUnicodeString(RealValue<DUnicodeString>(dobject->instanceOf()->name()));
  zmsg_addmsg(msg, &submsg);
  return (msg);
}


DOpaque    SerializeRPC::sDStruct(DValue const& args)
{
  DStruct* dstruct = args; //server check for NULL in find & generate but not for direct call XXX
  zmsg_t* msg = (zmsg_t*)this->sDUnicodeString(RealValue<DUnicodeString>(dstruct->name())); 

  size_t attributeCount = dstruct->attributeCount();
  zmsg_t* submsg = (zmsg_t*)this->sDUInt32(RealValue<DUInt32>((DInt32)attributeCount)); //XXX size t

  zmsg_addmsg(msg, &submsg);
  for (DStruct::DAttributeIterator i = dstruct->attributeBegin(); i != dstruct->attributeEnd(); ++i)
  {
    submsg = (zmsg_t*)this->sDUnicodeString(RealValue<DUnicodeString>((*i).name()));
    zmsg_addmsg(msg, &submsg);
    DType::Type_t type = (*i).type().getType();
    submsg = (zmsg_t*)this->sDUInt8(RealValue<DUInt8>(type));
    zmsg_addmsg(msg, &submsg);
    if (type == DType::DMethodType)
    {
       DType::Type_t argumentType = (*i).type().getArgumentType();
       submsg = (zmsg_t*)this->sDUInt8(RealValue<DUInt8>(argumentType));
       zmsg_addmsg(msg, &submsg);
       DType::Type_t returnType = (*i).type().getReturnType();
       submsg = (zmsg_t*)this->sDUInt8(RealValue<DUInt8>(returnType));
       zmsg_addmsg(msg, &submsg);
    }
  } 
  return (msg);
}

DOpaque    SerializeRPC::sDNone(DValue const& args)
{
  zmsg_t* msg = zmsg_new();
  return (msg);
}

DOpaque    SerializeRPC::sDMethod(DValue const& args)
{
  //to implement
  zmsg_t* msg = zmsg_new();
  return (msg);
}

DOpaque    SerializeRPC::sDUnicodeString(DValue const& args)
{
  DUnicodeString str = args;
  zmsg_t* msg = zmsg_new();
  zframe_t* frame = zframe_new(str.c_str(), str.size());
  zmsg_append(msg, &frame);

  return (msg);
}

DOpaque    SerializeRPC::sDBuffer(DValue const& args)
{
  DBuffer dbuffer = args;
  zmsg_t* msg = zmsg_new();
  zframe_t* frame = zframe_new(dbuffer.data(), dbuffer.size());
  zmsg_append(msg, &frame);

  return (msg);
}

DOpaque    SerializeRPC::sDInt8(DValue const& args)
{
  DInt8 value = args;
  zmsg_t* msg = zmsg_new();
  zframe_t* frame = zframe_new((uint8_t*)&value, sizeof(value));
  zmsg_append(msg, &frame);

  return (msg);
}

DOpaque    SerializeRPC::sDInt16(DValue const& args)
{
  DInt16 value = args;
  zmsg_t* msg = zmsg_new();
  zframe_t* frame = zframe_new((uint8_t*)&value, sizeof(value));
  zmsg_append(msg, &frame);

  return (msg);
}

DOpaque    SerializeRPC::sDInt32(DValue const& args)
{
  DInt32 value = args;
  zmsg_t* msg = zmsg_new();
  zframe_t* frame = zframe_new((uint8_t*)&value, sizeof(value));
  zmsg_append(msg, &frame);

  return (msg);
}

DOpaque    SerializeRPC::sDInt64(DValue const& args)
{
  DInt64 value = args;
  zmsg_t* msg = zmsg_new();
  zframe_t* frame = zframe_new((uint8_t*)&value, sizeof(value));
  zmsg_append(msg, &frame);

  return (msg);
}

DOpaque    SerializeRPC::sDUInt8(DValue const& args)
{
  DUInt8 value = args;
  zmsg_t* msg = zmsg_new();
  zframe_t* frame = zframe_new((uint8_t*)&value, sizeof(value));
  zmsg_append(msg, &frame);

  return (msg);
}

DOpaque    SerializeRPC::sDUInt16(DValue const& args)
{
  DUInt16 value = args;
  zmsg_t* msg = zmsg_new();
  zframe_t* frame = zframe_new((uint8_t*)&value, sizeof(value));
  zmsg_append(msg, &frame);

  return (msg);
}

DOpaque    SerializeRPC::sDUInt32(DValue const& args)
{
  DUInt32 value = args;
  zmsg_t* msg = zmsg_new();
  zframe_t* frame = zframe_new((uint8_t*)&value, sizeof(value));
  zmsg_append(msg, &frame);

  return (msg);
}

DOpaque    SerializeRPC::sDUInt64(DValue const& args)
{
  DUInt64 value = args;
  zmsg_t* msg = zmsg_new();
  zframe_t* frame = zframe_new((uint8_t*)&value, sizeof(value));
  zmsg_append(msg, &frame);

  return (msg);
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

DObject*        DeserializeRPC::dDObject(DOpaque msg)
{
  DUInt64 id = this->dDUInt64(msg);
  DUnicodeString objectName = this->dDUnicodeString(zmsg_popmsg((zmsg_t*)msg));
  DStruct* dstruct = NULL; 
  SerializeRPC* serializer = static_cast<SerializeRPC*>(DStructs::instance().find("SerializeRPC")->newObject(RealValue<DObject*>(this->__stream))); //XXX new object each time  -> slow reuse parent one client or server 
  try
  {
     dstruct = Destruct::DStructs::instance().find(objectName);
  }
  catch (DException const& exception)
  {
    zmsg_t* rep1 = (zmsg_t*)serializer->sDUInt8(RealValue<DUInt8>(CMD_FIND)); 
    zmsg_t* rep2 = (zmsg_t*)serializer->sDUnicodeString(RealValue<DUnicodeString>(objectName));
    zmsg_addmsg(rep1, &rep2);
    this->__networkStream->send(RealValue<DOpaque>(rep1));

    zmsg_t* req = (zmsg_t*)this->__networkStream->recv();
    dstruct = this->dDStruct(req); //register done in dDStruct 
  }
  
  DObject* clientObject = new StubObject(RealValue<DObject*>(this->__stream), RealValue<DObject*>(serializer), RealValue<DObject*>(this), id, dstruct);
  serializer->destroy(); 

  return (clientObject);
}

DStruct*        DeserializeRPC::dDStruct(DOpaque msg)
{
  DStruct* dstruct = NULL; 

  DUnicodeString structName = this->dDUnicodeString((zmsg_t*)msg);
  DUInt32 attributeCount = this->dDUInt32(zmsg_popmsg((zmsg_t*)msg));

                                  
  if ((dstruct = new StubStruct(0, structName, StubObject::newObject, this->__networkStream)) == NULL)//inheritance ? 
    return (NULL);
  for (size_t i = 0; i < attributeCount; i++) 
  {
     DUnicodeString name = this->dDUnicodeString(zmsg_popmsg((zmsg_t*)msg));
     
     DType::Type_t type = (DType::Type_t)this->dDUInt8(zmsg_popmsg((zmsg_t*)msg)); 
     if (type == DType::DMethodType)
     {
       DType::Type_t argumentType = (DType::Type_t)this->dDUInt8(zmsg_popmsg((zmsg_t*)msg));
       DType::Type_t returnType = (DType::Type_t)this->dDUInt8(zmsg_popmsg((zmsg_t*)msg));
       dstruct->addAttribute(DAttribute(returnType, name, argumentType, type));
     }
     else
       dstruct->addAttribute(DAttribute(type, name));
  }
  DStructs::instance().registerDStruct(dstruct); //register here ? 
  return (dstruct); 
}

DObject*        DeserializeRPC::dDNone(DOpaque msg)
{
  zmsg_pop((zmsg_t*)msg);
  return (DNone);
}

DFunctionObject* DeserializeRPC::dDMethod(DOpaque msg)
{
  //Implemented in StubObject
  zmsg_pop((zmsg_t*)msg); 
  return (NULL);
}

DUnicodeString  DeserializeRPC::dDUnicodeString(DOpaque msg)
{
  zframe_t* frame = zmsg_pop((zmsg_t*)msg);
  const char* data = (const char*)zframe_data(frame);
  size_t size = zframe_size(frame);

  std::string res = std::string(data, size);
  zframe_destroy(&frame);

  return (res);
}

DBuffer         DeserializeRPC::dDBuffer(DOpaque msg)
{
  zframe_t* frame = zmsg_pop((zmsg_t*)msg);
  uint8_t* data = (uint8_t*)zframe_data(frame);
  size_t size = zframe_size(frame);
 
  DBuffer res = DBuffer(data, size); 
  zframe_destroy(&frame);

  return (res);
}

DInt8           DeserializeRPC::dDInt8(DOpaque msg)
{
  zframe_t* frame = zmsg_pop((zmsg_t*)msg);
  DInt8 res = *((DInt8*)zframe_data(frame));

  zframe_destroy(&frame);
  return (res);
}

DInt16          DeserializeRPC::dDInt16(DOpaque msg)
{
  zframe_t* frame = zmsg_pop((zmsg_t*)msg);
  DInt16 res = *((DInt16*)zframe_data(frame));
 
  zframe_destroy(&frame);
  return (res);
}

DInt32          DeserializeRPC::dDInt32(DOpaque msg)
{
  zframe_t* frame = zmsg_pop((zmsg_t*)msg);
  DInt32 res = *((DInt32*)zframe_data(frame));
 
  zframe_destroy(&frame);
  return (res);
}

DInt64          DeserializeRPC::dDInt64(DOpaque msg)
{
  zframe_t* frame = zmsg_pop((zmsg_t*)msg);
  DInt64 res = *((DInt64*)zframe_data(frame));
 
  zframe_destroy(&frame);
  return (res);

}

DUInt8          DeserializeRPC::dDUInt8(DOpaque msg)
{
  zframe_t* frame = zmsg_pop((zmsg_t*)msg);
  DUInt8 res = *((DUInt8*)zframe_data(frame));
 
  zframe_destroy(&frame);
  return (res);
}

DUInt16         DeserializeRPC::dDUInt16(DOpaque msg)
{
  zframe_t* frame = zmsg_pop((zmsg_t*)msg);
  DUInt16 res = *((DUInt16*)zframe_data(frame));
 
  zframe_destroy(&frame);
  return (res);
}

DUInt32         DeserializeRPC::dDUInt32(DOpaque msg)
{
  zframe_t* frame = zmsg_pop((zmsg_t*)msg);
  DUInt32 res = *((DInt32*)zframe_data(frame));
 
  zframe_destroy(&frame);
  return (res);
}

DUInt64         DeserializeRPC::dDUInt64(DOpaque msg)
{
  zframe_t* frame = zmsg_pop((zmsg_t*)msg);
  DUInt64 res = *((DUInt64*)zframe_data(frame));
 
  zframe_destroy(&frame);
  return (res);
}
