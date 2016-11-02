#include "dstructs.hpp"
#include "serializerpc.hpp"

#include "networkstream.hpp"
#include "serializerpc.hpp"
#include "serverobject.hpp"
#include "stubobject.hpp"
#include "stubfunctionobject.hpp"
#include "stubstruct.hpp"

/**
 *  StubObject Proxy object that handle transparent remote communication and let you use your object as a local object
 */
StubObject::StubObject(DValue const& stream, DValue const& serializer, DValue const& deserializer, uint64_t id, DStruct* dstruct) : DObject(dstruct, RealValue<DObject*>(DNone)), __networkStreamObject(stream), __serializerObject(serializer), __deserializerObject(deserializer), __id(id), __serializer(static_cast<SerializeRPC*>((DObject*)serializer)), __deserializer(static_cast<DeserializeRPC*>((DObject*)deserializer)), __networkStream(static_cast<NetworkStream*>((DObject*)stream))
{
  //this->init(this);
}

StubObject::StubObject(DStruct* dstruct, DValue const& args) : DObject(dstruct, args),  __networkStreamObject(DNone), __serializerObject(DNone), __deserializerObject(DNone), __id(-1), __serializer(NULL), __deserializer(NULL), __networkStream(NULL)
{
  //this->init(this);
}

StubObject::StubObject(StubObject const & rhs) : DObject(rhs),  __networkStreamObject(rhs.__networkStream), __serializerObject(rhs.__serializer), __deserializerObject(rhs.__serializer), __id(rhs.__id), __serializer(static_cast<SerializeRPC*>((DObject*)rhs.__serializerObject)), __deserializer(static_cast<DeserializeRPC*>((DObject*)rhs.__deserializerObject)), __networkStream(static_cast<NetworkStream*>((DObject*)rhs.__networkStreamObject))
{
  //this->copy(this, rhs);
}

StubObject::~StubObject()
{
}

DObject* StubObject::newObject(StubStruct* dstruct, DValue const& args, NetworkStream* networkStream)
{
   SerializeRPC* serialize = static_cast<SerializeRPC*>(DStructs::instance().find("SerializeRPC")->newObject(RealValue<DObject*>(networkStream)));
  DeserializeRPC* deserialize = static_cast<DeserializeRPC*>(DStructs::instance().find("DeserializeRPC")->newObject(RealValue<DObject*>(networkStream)));

 //XXX if none ...
  zmsg_t* msg = (zmsg_t*)serialize->sDUInt8(RealValue<DUInt8>(CMD_GENERATE_ARG));
  zmsg_t* submsg = (zmsg_t*)serialize->sDUnicodeString(RealValue<DUnicodeString>(dstruct->name()));

  zmsg_addmsg(msg, &submsg);
  //DUInt64 argId = static_cast<StubObject*>(args.get<DObject*>())->id();
  //serialize->sDUInt64(RealValue<DUInt64>(argId));
  //serialize->sDObject(args); ///XXX must find type 
  //this->dstruct.find("constructor")
  // serialize constructor type 

  networkStream->send(RealValue<DOpaque>(msg));
  zmsg_t* reply = (zmsg_t*)networkStream->recv(); 
  DUInt64 objectId = deserialize->dDUInt64(reply);

  StubObject* root = new StubObject(RealValue<DObject*>(networkStream), RealValue<DObject*>(serialize), RealValue<DObject*>(deserialize), objectId, dstruct); //ARGS ARGS XXX  

  return (RealValue<DObject*>(root));
  //return (new StubObject(dstruct, args)); 
}

void StubObject::setValue(DUnicodeString const& name, DValue const &v)
{
  zmsg_t* msg = (zmsg_t*)this->__serializer->sDUInt8(RealValue<DUInt8>(CMD_SETVALUE)); 
  zmsg_t* submsg = (zmsg_t*)this->__serializer->sDUInt64(RealValue<DUInt64>(this->__id));
  zmsg_addmsg(msg, &submsg);
  submsg = (zmsg_t*)this->__serializer->sDUnicodeString(RealValue<DUnicodeString>(name));
  zmsg_addmsg(msg, &submsg);
  submsg = (zmsg_t*)this->__serializer->call(this->instanceOf()->attribute(name).type().name(), v).get<DOpaque>(); //erialize arg
  zmsg_addmsg(msg, &submsg);
  this->__networkStream->send(RealValue<DOpaque>(msg));
  this->__networkStream->recv();
}

DValue StubObject::getValue(DUnicodeString const& name) const
{
  zmsg_t* msg = (zmsg_t*)this->__serializer->sDUInt8(RealValue<DUInt8>(CMD_GETVALUE));
  zmsg_t* submsg = (zmsg_t*)this->__serializer->sDUInt64(RealValue<DUInt64>(this->__id)); 
  zmsg_addmsg(msg, &submsg);
  submsg = (zmsg_t*)this->__serializer->sDUnicodeString(RealValue<DUnicodeString>(name));
  zmsg_addmsg(msg, &submsg);

  DType  dtype = this->instanceOf()->attribute(name).type();
  if (dtype.getType() == DType::DMethodType)
  {
    this->__networkStream->send(RealValue<DOpaque>(msg));
    zmsg_t* reply = (zmsg_t*)this->__networkStream->recv();
    DUInt64 id = this->__deserializer->dDUInt64(reply);
    
    //Not directly returned as dvalue and DRef by a DFunction* () function so must deref ourself or memory will leak
    DFunctionObject* clientFunctionObject = new StubFunctionObject(((DObject*)this->__networkStream), ((DObject*)this->__serializer), ((DObject*)this->__deserializer), id, dtype.getArgumentType(), dtype.getReturnType()); 
    DValue functionObject = RealValue<DFunctionObject*>(clientFunctionObject);
    //clientFunctionObject->destroy(); //python get a 0 ref object
    return (functionObject);
  } 
  this->__networkStream->send(RealValue<DOpaque>(msg));
  zmsg_t* reply = (zmsg_t*)this->__networkStream->recv();

  DValue value = (((DObject*)this->__deserializer)->call(dtype.name(), RealValue<DOpaque>(reply)));//deserialize return XXX Dobject not in base
  return (value);
}
                                        
DValue StubObject::call(DUnicodeString const& name, DValue const &args)
{
  DType  dtype = this->instanceOf()->attribute(name).type();

  zmsg_t* msg = (zmsg_t*)this->__serializer->sDUInt8(RealValue<DUInt8>(CMD_CALL)); 
  zmsg_t* submsg = (zmsg_t*)this->__serializer->sDUInt64(RealValue<DUInt64>(this->__id));
  zmsg_addmsg(msg, &submsg);
  submsg = (zmsg_t*)this->__serializer->sDUnicodeString(RealValue<DUnicodeString>(name));
  zmsg_addmsg(msg, &submsg);

  /* Send argument (object is not compatible)  XXX implement it */
  submsg = (zmsg_t*) ((DObject*)this->__serializer)->call(dtype.argumentName(), args).get<DOpaque>(); //serialize arg
  zmsg_addmsg(msg, &submsg);
  this->__networkStream->send(RealValue<DOpaque>(msg));
  zmsg_t* reply = (zmsg_t*)this->__networkStream->recv(); 
  DValue value = (((DObject*)this->__deserializer)->call(dtype.returnName(), RealValue<DOpaque>(reply))); //deserialize return XXX dobject not in arg

  return (value);
}

DValue StubObject::call(DUnicodeString const& name)
{
  DType  dtype = this->instanceOf()->attribute(name).type();

  zmsg_t* msg = (zmsg_t*)this->__serializer->sDUInt8(RealValue<DUInt8>(CMD_CALL0)); 
  zmsg_t* submsg = (zmsg_t*)this->__serializer->sDUInt64(RealValue<DUInt64>(this->__id));
  zmsg_addmsg(msg, &submsg);
  submsg = (zmsg_t*)this->__serializer->sDUnicodeString(RealValue<DUnicodeString>(name));
  zmsg_addmsg(msg, &submsg);

  this->__networkStream->send(RealValue<DOpaque>(msg));
  zmsg_t* reply = (zmsg_t*)this->__networkStream->recv(); 
  DValue value = (((DObject*)this->__deserializer)->call(dtype.returnName(), RealValue<DOpaque>(reply))); //deserialize return XXX dobject not in arg

  return (value);
}

DValue StubObject::getValue(size_t index) const
{
  DAttribute attribute = this->instanceOf()->attribute(index);
  return (this->getValue(attribute.name()));
}

void StubObject::setValue(size_t index, DValue const &value)
{
  DUnicodeString name = this->instanceOf()->attribute(index).name();
  this->setValue(name, value); 
}

DValue StubObject::call(size_t index, DValue const &value)
{
  DUnicodeString name = this->instanceOf()->attribute(index).name();
  return (this->call(name, value));
}

DObject* StubObject::clone() const
{
  return (new StubObject(*this));
}  

BaseValue* StubObject::getBaseValue(size_t index)
{
  return (NULL); 
}

BaseValue const* StubObject::getBaseValue(size_t index) const
{
  return (NULL);
}

uint64_t        StubObject::id(void) const
{
  return (this->__id);
}
