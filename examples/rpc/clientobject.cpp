#include "dstructs.hpp"
//#include "protocol/dserialize.hpp"

#include "networkstream.hpp"
#include "serializerpc.hpp"
#include "clientobject.hpp"
#include "clientfunctionobject.hpp"

namespace Destruct {

/**
 *  ClientObject Proxy object that handle transparent remote communication and let you use your object as a local object
 */
ClientObject::ClientObject(DObject* stream, DObject* serializer, DObject* deserializer, uint64_t id, DStruct* dstruct) : DObject(dstruct, RealValue<DObject*>(DNone)), __id(id), __networkStream(stream), __serializer(serializer), __deserializer(deserializer)
{
  //this->init(this);
}

ClientObject::ClientObject(DStruct* dstruct, DValue const& args) : DObject(dstruct, args), __id(-1), __networkStream(DNone), __serializer(DNone), __deserializer(DNone)
{
  //this->init(this);
}

ClientObject::ClientObject(ClientObject const & rhs) : DObject(rhs), __id(rhs.__id),  __networkStream(rhs.__networkStream), __serializer(rhs.__serializer), __deserializer(rhs.__serializer)
{
  //this->copy(this, rhs);
}

ClientObject::~ClientObject()
{
}

DObject* ClientObject::newObject(DStruct* dstruct, DValue const& args)
{
  return (new ClientObject(dstruct, args)); //XXX copy network stream, handle connection & struct etc...
}

DValue ClientObject::getValue(DUnicodeString const& name) const
{
  this->__serializer->call("DUnicodeString", RealValue<DUnicodeString>("getValue")); 
  this->__serializer->call("DUInt64", RealValue<DUInt64>(this->__id)); 
  this->__serializer->call("DUnicodeString", RealValue<DUnicodeString>(name));
  this->__networkStream->call("flush");

  DType  dtype = this->instanceOf()->attribute(name).type();
 
  if (dtype.getType() == DType::DMethodType) //XXX XXX how to implem it ?
  {
    DUInt64 id = this->__deserializer->call("DUInt64").get<DUInt64>();

    return (RealValue<DFunctionObject*>(new ClientFunctionObject(this->__networkStream, this->__serializer, this->__deserializer, id, dtype.getArgumentType(), dtype.getReturnType())));
  } 

  return (this->__deserializer->call(dtype.name()));
}

void ClientObject::setValue(DUnicodeString const& name, DValue const &v)
{
  this->__serializer->call("DUnicodeString", RealValue<DUnicodeString>("setValue"));
  this->__serializer->call("DUInt64", RealValue<DUInt64>(this->__id));
  this->__serializer->call("DUnicodeString", RealValue<DUnicodeString>(name));
 
  //don't need get type if serialized in a streamstring or binary ? 
  //this->__serializer->serialize(this->__networkStream, v, this->instanceOf()->attribute(name).type().getType());
  this->__serializer->call(this->instanceOf()->attribute(name).type().name(), v);
  this->__networkStream->call("flush");
}
                                        
DValue ClientObject::call(DUnicodeString const& name, DValue const &args)
{
  DType  dtype = this->instanceOf()->attribute(name).type();
  //if (name == "serializeText")
  //{
  //..  newObject()
  //BaseValue *b = DObject::getBaseValue(this->__object, this->instanceOf()->findAttribute(name));
  //DValue v = b->getFinal().get<DFunctionObject* >()->call(args); 
  //return RealValue<DObject*>(DNone);
  //}

  this->__serializer->call("DUnicodeString", RealValue<DUnicodeString>("call"));
  this->__serializer->call("DUInt64", RealValue<DUInt64>(this->__id));
  this->__serializer->call("DUnicodeString", RealValue<DUnicodeString>(name));

  /* Send argument (object is not compatible) */
  this->__serializer->call(dtype.argumentName(), args);
  this->__networkStream->call("flush");
 
  /* get return value */
  return (this->__deserializer->call(dtype.returnName()));
}

DValue ClientObject::call(DUnicodeString const& name)
{
  this->__serializer->call("DUnicodeString", RealValue<DUnicodeString>("call0"));
  this->__serializer->call("DUInt64", RealValue<DUInt64>(this->__id));
  this->__serializer->call("DUnicodeString", RealValue<DUnicodeString>(name));

  this->__networkStream->call("flush");

  DType  dtype = this->instanceOf()->attribute(name).type();
  return (this->__deserializer->call(dtype.returnName()));
}

DValue ClientObject::getValue(size_t index) const
{
  DAttribute attribute = this->instanceOf()->attribute(index);
  return (this->getValue(attribute.name()));
}

void ClientObject::setValue(size_t index, DValue const &value)
{
  DUnicodeString name = this->instanceOf()->attribute(index).name();
  this->setValue(name, value); 
}

DValue ClientObject::call(size_t index, DValue const &value)
{
  DUnicodeString name = this->instanceOf()->attribute(index).name();
  return (this->call(name, value));  //use call 0 ? 
}

DObject* ClientObject::clone() const
{
  return (new ClientObject(*this));
}  

BaseValue* ClientObject::getBaseValue(size_t index)
{
  std::cout << "get base value " << std::endl;
  return (NULL); 
}


BaseValue const* ClientObject::getBaseValue(size_t index) const
{
  std::cout << "get base value " << std::endl;
  return (NULL);
}
}
