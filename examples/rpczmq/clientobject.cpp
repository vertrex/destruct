#include "dstructs.hpp"
#include "serializerpc.hpp"

#include "networkstream.hpp"
#include "serializerpc.hpp"
#include "clientobject.hpp"
#include "clientfunctionobject.hpp"

namespace Destruct {

/**
 *  ClientObject Proxy object that handle transparent remote communication and let you use your object as a local object
 */
ClientObject::ClientObject(DValue const& stream, DValue const& serializer, DValue const& deserializer, uint64_t id, DStruct* dstruct) : DObject(dstruct, RealValue<DObject*>(DNone)), __networkStreamObject(stream), __serializerObject(serializer), __deserializerObject(deserializer), __id(id), __serializer(static_cast<SerializeRPC*>((DObject*)serializer)), __deserializer(static_cast<DeserializeRPC*>((DObject*)deserializer)), __networkStream(static_cast<NetworkStream*>((DObject*)stream))
{
  //this->init(this);
}

ClientObject::ClientObject(DStruct* dstruct, DValue const& args) : DObject(dstruct, args),  __networkStreamObject(DNone), __serializerObject(DNone), __deserializerObject(DNone), __id(-1), __serializer(NULL), __deserializer(NULL), __networkStream(NULL)
{
  //this->init(this);
}

ClientObject::ClientObject(ClientObject const & rhs) : DObject(rhs),  __networkStreamObject(rhs.__networkStream), __serializerObject(rhs.__serializer), __deserializerObject(rhs.__serializer), __id(rhs.__id), __serializer(static_cast<SerializeRPC*>((DObject*)rhs.__serializerObject)), __deserializer(static_cast<DeserializeRPC*>((DObject*)rhs.__deserializerObject)), __networkStream(static_cast<NetworkStream*>((DObject*)rhs.__networkStreamObject))
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
  this->__serializer->sDUnicodeString(RealValue<DUnicodeString>("getValue")); 
  this->__serializer->sDUInt64(RealValue<DUInt64>(this->__id)); 
  this->__serializer->sDUnicodeString(RealValue<DUnicodeString>(name));

  DType  dtype = this->instanceOf()->attribute(name).type();
 
  if (dtype.getType() == DType::DMethodType)
  {
    this->__networkStream->request();
    DUInt64 id = this->__deserializer->dDUInt64();
    this->__networkStream->flushRead();
    
    //Not directly returned as dvalue and DRef by a DFunction* () function so must deref ourself or memory will leak
    DFunctionObject* clientFunctionObject = new ClientFunctionObject(((DObject*)this->__networkStream), ((DObject*)this->__serializer), ((DObject*)this->__deserializer), id, dtype.getArgumentType(), dtype.getReturnType()); 
    DValue functionObject = RealValue<DFunctionObject*>(clientFunctionObject);
    //clientFunctionObject->destroy(); //python get a 0 ref object
    return (functionObject);
  } 

  this->__networkStream->request();
  DValue value = (((DObject*)this->__deserializer)->call(dtype.name()));
  this->__networkStream->flushRead();
  return (value);
}

void ClientObject::setValue(DUnicodeString const& name, DValue const &v)
{
  this->__serializer->sDUnicodeString(RealValue<DUnicodeString>("setValue"));
  this->__serializer->sDUInt64(RealValue<DUInt64>(this->__id));
  this->__serializer->sDUnicodeString(RealValue<DUnicodeString>(name));
 
  this->__serializer->call(this->instanceOf()->attribute(name).type().name(), v);
  this->__networkStream->request();
}
                                        
DValue ClientObject::call(DUnicodeString const& name, DValue const &args)
{
  DType  dtype = this->instanceOf()->attribute(name).type();

  this->__serializer->sDUnicodeString(RealValue<DUnicodeString>("call"));
  this->__serializer->sDUInt64(RealValue<DUInt64>(this->__id));
  this->__serializer->sDUnicodeString(RealValue<DUnicodeString>(name));

  /* Send argument (object is not compatible)  XXX implement it */
  ((DObject*)this->__serializer)->call(dtype.argumentName(), args);
  this->__networkStream->request();

  DValue value = (((DObject*)this->__deserializer)->call(dtype.returnName()));
  this->__networkStream->flushRead();
  return (value);
}

DValue ClientObject::call(DUnicodeString const& name)
{
  this->__serializer->sDUnicodeString(RealValue<DUnicodeString>("call0"));
  this->__serializer->sDUInt64(RealValue<DUInt64>(this->__id));
  this->__serializer->sDUnicodeString(RealValue<DUnicodeString>(name));

  this->__networkStream->request();
  DType  dtype = this->instanceOf()->attribute(name).type();
  DValue value = (((DObject*)this->__deserializer)->call(dtype.returnName()));
  this->__networkStream->flushRead();

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
