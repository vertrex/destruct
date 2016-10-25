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

  this->__serializer->sDUInt8(RealValue<DUInt8>(CMD_FUNCTIONCALL)); 
  this->__serializer->sDUInt64(RealValue<DUInt64>(this->__id));

  ///* Send argument (object is not compatible) */
  this->__serializer->call(DType(this->__argumentType).name(), args);
;
  /* get return value */
  //return (this->__deserializer->call(DType(this->__returnType).name()));

  this->__networkStream->request();
  DValue value = (this->__deserializer->call(DType(this->__returnType).name()));
  this->__networkStream->flushRead();
  return (value);
}

DValue StubFunctionObject::call(void) const
{
  this->__serializer->sDUInt8(RealValue<DUInt8>(CMD_FUNCTIONCALL0)); 
  this->__serializer->sDUInt64(RealValue<DUInt64>(this->__id));
  //this->__networkStream->call("flush");
  this->__networkStream->request();
  DValue value = this->__deserializer->call(DType(this->__returnType).name());
  this->__networkStream->flushRead();

  return (value);
}
