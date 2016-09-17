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
ClientObject::ClientObject(DValue const& stream, DValue const& serializer, DValue const& deserializer, uint64_t id, DStruct* dstruct) : DObject(dstruct, RealValue<DObject*>(DNone)), __networkStream(stream), __serializer(serializer), __deserializer(deserializer), __id(id)
{
  //this->init(this);
}

ClientObject::ClientObject(DStruct* dstruct, DValue const& args) : DObject(dstruct, args),  __networkStream(DNone), __serializer(DNone), __deserializer(DNone), __id(-1)
{
  //this->init(this);
}

ClientObject::ClientObject(ClientObject const & rhs) : DObject(rhs),  __networkStream(rhs.__networkStream), __serializer(rhs.__serializer), __deserializer(rhs.__serializer), __id(rhs.__id)
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
  ((DObject*)this->__serializer)->call("DUnicodeString", RealValue<DUnicodeString>("getValue")); 
  ((DObject*)this->__serializer)->call("DUInt64", RealValue<DUInt64>(this->__id)); 
  ((DObject*)this->__serializer)->call("DUnicodeString", RealValue<DUnicodeString>(name));

  DType  dtype = this->instanceOf()->attribute(name).type();
 
  if (dtype.getType() == DType::DMethodType)
  {
    ((DObject*)this->__networkStream)->call("request");
    DUInt64 id = ((DObject*)this->__deserializer)->call("DUInt64");
    ((DObject*)this->__networkStream)->call("flushRead");
    

    //Not directly returned as dvalue and DRef by a DFunction* () function so must deref ourself or memory will leak
    DFunctionObject* clientFunctionObject = new ClientFunctionObject(((DObject*)this->__networkStream), ((DObject*)this->__serializer), ((DObject*)this->__deserializer), id, dtype.getArgumentType(), dtype.getReturnType()); 
    DValue functionObject = RealValue<DFunctionObject*>(clientFunctionObject);
    //clientFunctionObject->destroy(); //python get a 0 ref object
    return (functionObject);
  } 

  ((DObject*)this->__networkStream)->call("request");
  DValue value = (((DObject*)this->__deserializer)->call(dtype.name()));
  ((DObject*)this->__networkStream)->call("flushRead");
  return (value);
}

void ClientObject::setValue(DUnicodeString const& name, DValue const &v)
{
  ((DObject*)this->__serializer)->call("DUnicodeString", RealValue<DUnicodeString>("setValue"));
  ((DObject*)this->__serializer)->call("DUInt64", RealValue<DUInt64>(this->__id));
  ((DObject*)this->__serializer)->call("DUnicodeString", RealValue<DUnicodeString>(name));
 
  ((DObject*)this->__serializer)->call(this->instanceOf()->attribute(name).type().name(), v);
  ((DObject*)this->__networkStream)->call("request");
}
                                        
DValue ClientObject::call(DUnicodeString const& name, DValue const &args)
{
  DType  dtype = this->instanceOf()->attribute(name).type();

  ((DObject*)this->__serializer)->call("DUnicodeString", RealValue<DUnicodeString>("call"));
  ((DObject*)this->__serializer)->call("DUInt64", RealValue<DUInt64>(this->__id));
  ((DObject*)this->__serializer)->call("DUnicodeString", RealValue<DUnicodeString>(name));

  /* Send argument (object is not compatible)  XXX implement it */
  ((DObject*)this->__serializer)->call(dtype.argumentName(), args);
  ((DObject*)this->__networkStream)->call("request");

  DValue value = (((DObject*)this->__deserializer)->call(dtype.returnName()));
  ((DObject*)this->__networkStream)->call("flushRead");
  return (value);
}

DValue ClientObject::call(DUnicodeString const& name)
{
  ((DObject*)this->__serializer)->call("DUnicodeString", RealValue<DUnicodeString>("call0"));
  ((DObject*)this->__serializer)->call("DUInt64", RealValue<DUInt64>(this->__id));
  ((DObject*)this->__serializer)->call("DUnicodeString", RealValue<DUnicodeString>(name));

  ((DObject*)this->__networkStream)->call("request");
  DType  dtype = this->instanceOf()->attribute(name).type();
  DValue value = (((DObject*)this->__deserializer)->call(dtype.returnName()));
  ((DObject*)this->__networkStream)->call("flushRead");

  return (value);
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
  return (this->call(name, value));
}

DObject* ClientObject::clone() const
{
  return (new ClientObject(*this));
}  

BaseValue* ClientObject::getBaseValue(size_t index)
{
  return (NULL); 
}

BaseValue const* ClientObject::getBaseValue(size_t index) const
{
  return (NULL);
}

}
