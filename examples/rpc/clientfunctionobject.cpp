#include "clientfunctionobject.hpp"
#include "serializerpc.hpp"

/**
 *  ClientFunctionObject
 */
ClientFunctionObject::ClientFunctionObject(NetworkStream& stream, DSerialize* serialize, uint64_t id, DType::Type_t argumentType, DType::Type_t returnType) : DFunctionObject(), __id(id), __networkStream(stream), __serializer(serialize), __argumentType(argumentType), __returnType(returnType)
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

  this->__networkStream.write(std::string("functionCall"));
  this->__networkStream.write(this->__id);

  /* Send argument (object is not compatible) */
  this->__serializer->serialize(this->__networkStream, args, this->__argumentType);
  this->__networkStream.flush();
 
  /* get return value */
  return (this->__serializer->deserialize(this->__networkStream, this->__returnType));
}

DValue ClientFunctionObject::call(void) const
{
  this->__networkStream.write(std::string("functionCall0"));
  this->__networkStream.write(this->__id);
  this->__networkStream.flush();

  DValue value = this->__serializer->deserialize(this->__networkStream, this->__returnType);
  return (value);
}
