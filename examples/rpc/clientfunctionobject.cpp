#include "clientfunctionobject.hpp"
#include "serializerpc.hpp"

/**
 *  ClientFunctionObject
 */
ClientFunctionObject::ClientFunctionObject(DObject* stream, DObject* serialize, DObject* deserialize, uint64_t id, DType::Type_t argumentType, DType::Type_t returnType) : DFunctionObject(), __id(id), __networkStream(stream), __serializer(serialize), __deserializer(deserialize), __argumentType(argumentType), __returnType(returnType)
{

}

ClientFunctionObject::~ClientFunctionObject()
{
  //server delref // remove from manager
}

DValue ClientFunctionObject::call(DValue const& args) const
{
  if (args.asUnicodeString() == "None *") // XXX must be able to serialize None object 
    return (this->call());

  this->__serializer->call("DUnicodeString", RealValue<DUnicodeString>("functionCall"));
  this->__serializer->call("DUInt64", RealValue<DUInt64>(this->__id));

  ///* Send argument (object is not compatible) */
  this->__serializer->call(DType(this->__argumentType).name(), args);
  this->__networkStream->call("flush");
;
  /* get return value */
  return (this->__deserializer->call(DType(this->__returnType).name()));
}

DValue ClientFunctionObject::call(void) const
{
  this->__serializer->call("DUnicodeString", RealValue<DUnicodeString>("functionCall0"));
  this->__serializer->call("DUInt64", RealValue<DUInt64>(this->__id));
  this->__networkStream->call("flush");

  return (this->__deserializer->call(DType(this->__returnType).name()));
}
