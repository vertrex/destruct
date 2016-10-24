#include "dstructs.hpp"
#include "serializerpc.hpp"

#include "networkstream.hpp"
#include "serializerpc.hpp"
#include "clientobject.hpp"
#include "clientfunctionobject.hpp"
#include "serverobject.hpp"
#include "clientstruct.hpp"

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

DObject* ClientObject::newObject(ClientStruct* dstruct, DValue const& args, NetworkStream* networkStream)
{

   SerializeRPC* serialize = static_cast<SerializeRPC*>(DStructs::instance().find("SerializeRPC")->newObject(RealValue<DObject*>(networkStream)));
  DeserializeRPC* deserialize = static_cast<DeserializeRPC*>(DStructs::instance().find("DeserializeRPC")->newObject(RealValue<DObject*>(networkStream)));

 //XXX if none ...

  serialize->sDUInt8(RealValue<DUInt8>(CMD_GENERATE_ARG));
  serialize->sDUnicodeString(RealValue<DUnicodeString>(dstruct->name()));

  //DUInt64 argId = static_cast<ClientObject*>(args.get<DObject*>())->id();
  //serialize->sDUInt64(RealValue<DUInt64>(argId));

  //serialize->sDObject(args); ///XXX must find type 
  //this->dstruct.find("constructor")
  // serialize constructor type 

  networkStream->request();
  DUInt64 objectId = deserialize->dDUInt64();
  networkStream->flushRead();

  ClientObject* root = new ClientObject(RealValue<DObject*>(networkStream), RealValue<DObject*>(serialize), RealValue<DObject*>(deserialize), objectId, dstruct); //ARGS ARGS XXX  

  return (RealValue<DObject*>(root));

  //return (new ClientObject(dstruct, args)); 
}

DValue ClientObject::getValue(DUnicodeString const& name) const
{
  this->__serializer->sDUInt8(RealValue<DUInt8>(CMD_GETVALUE)); 
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
  DValue value = (((DObject*)this->__deserializer)->call(dtype.name()));//deserialize return XXX Dobject not in base
  this->__networkStream->flushRead();
  return (value);
}

void ClientObject::setValue(DUnicodeString const& name, DValue const &v)
{
  this->__serializer->sDUInt8(RealValue<DUInt8>(CMD_SETVALUE)); 
  this->__serializer->sDUInt64(RealValue<DUInt64>(this->__id));
  this->__serializer->sDUnicodeString(RealValue<DUnicodeString>(name));
 
  this->__serializer->call(this->instanceOf()->attribute(name).type().name(), v); //erialize arg
  this->__networkStream->request();
}
                                        
DValue ClientObject::call(DUnicodeString const& name, DValue const &args)
{
  DType  dtype = this->instanceOf()->attribute(name).type();

  this->__serializer->sDUInt8(RealValue<DUInt8>(CMD_CALL)); 
  this->__serializer->sDUInt64(RealValue<DUInt64>(this->__id));
  this->__serializer->sDUnicodeString(RealValue<DUnicodeString>(name));

  /* Send argument (object is not compatible)  XXX implement it */
  ((DObject*)this->__serializer)->call(dtype.argumentName(), args); //serialize arg
  this->__networkStream->request();
  DValue value = (((DObject*)this->__deserializer)->call(dtype.returnName())); //deserialize return XXX dobject not in arg
  this->__networkStream->flushRead();

  return (value);
}

DValue ClientObject::call(DUnicodeString const& name)
{
  DType  dtype = this->instanceOf()->attribute(name).type();

  this->__serializer->sDUInt8(RealValue<DUInt8>(CMD_CALL0)); 
  this->__serializer->sDUInt64(RealValue<DUInt64>(this->__id));
  this->__serializer->sDUnicodeString(RealValue<DUnicodeString>(name));

  this->__networkStream->request();
  DValue value = (((DObject*)this->__deserializer)->call(dtype.returnName())); //deserialize return XXX dobject not in arg
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

uint64_t        ClientObject::id(void) const
{
  return (this->__id);
}

}
