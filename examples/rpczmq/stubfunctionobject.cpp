#include "stubfunctionobject.hpp"
#include "serverobject.hpp"
#include "serializerpc.hpp"

/**
 *  StubFunctionObject
 */
StubFunctionObject::StubFunctionObject(DObject* stream, DObject* serialize, DObject* deserialize, uint64_t id, DType::Type_t argumentType, DType::Type_t returnType) : DFunctionObject(), __id(id), __networkStream(static_cast<NetworkStream*>(stream)), __serializer(static_cast<SerializeRPC*>(serialize)), __deserializer(static_cast<DeserializeRPC*>(deserialize)), __argumentType(argumentType), __returnType(returnType)
{
}

StubFunctionObject::~StubFunctionObject()
{
}

DValue StubFunctionObject::call(DValue const& args) const
{
  if (args.asUnicodeString() == "None *") // XXX must be able to serialize None object 
    return (this->call());

  zmsg_t* msg = (zmsg_t*)this->__serializer->sDUInt8(RealValue<DUInt8>(CMD_FUNCTIONCALL)); 
  zmsg_t* submsg = (zmsg_t*)this->__serializer->sDUInt64(RealValue<DUInt64>(this->__id));
  zmsg_addmsg(msg, &submsg);

  ///* Send argument (object is not compatible) */
  submsg = (zmsg_t*)this->__serializer->call(DType(this->__argumentType).name(), args).get<DOpaque>();
;
  zmsg_addmsg(msg, &submsg); 

  this->__networkStream->send(RealValue<DOpaque>(msg));
  zmsg_t* reply = (zmsg_t*)this->__networkStream->recv(); 
  DValue value = (this->__deserializer->call(DType(this->__returnType).name(), RealValue<DOpaque>(reply)));
  return (value);
}

DValue StubFunctionObject::call(void) const
{
  zmsg_t* msg = (zmsg_t*)this->__serializer->sDUInt8(RealValue<DUInt8>(CMD_FUNCTIONCALL0)); 
  zmsg_t* submsg = (zmsg_t*)this->__serializer->sDUInt64(RealValue<DUInt64>(this->__id));
  zmsg_addmsg(msg, &submsg);

  this->__networkStream->send(RealValue<DOpaque>(msg));
  zmsg_t* reply = (zmsg_t*)this->__networkStream->recv();
  DValue value = this->__deserializer->call(DType(this->__returnType).name(), RealValue<DOpaque>(reply));

  return (value);
}
