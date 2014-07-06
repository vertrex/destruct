#include "rpcobject.hpp"
#include "networkstream.hpp"
#include "destruct.hpp"
#include "protocol/dserialize.hpp"
#include "protocol/dstream.hpp"

#include "server.hpp"
#include "serializerpc.hpp"

namespace Destruct {

/**
 *  RPCObject Proxy object that handle transparent remote communication and let you use your object as a local object
 */
RPCObject::RPCObject(NetworkStream stream, uint64_t id, DStruct* dstruct, ObjectManager & objects) : DDynamicObject(dstruct, RealValue<DObject*>(DNone)), __id(id), __networkStream(stream), __serializer(new DSerializeRPC(stream, objects))
{
  this->init(this);
}

RPCObject::RPCObject(DStruct* dstruct, DValue const& args) : DDynamicObject(dstruct, args), __networkStream(0, RealValue<DInt64>(0))
{
  this->init(this);
}

RPCObject::RPCObject(RPCObject const & rhs) : DDynamicObject(rhs), __networkStream(rhs.__networkStream)
{
  this->copy(this, rhs);
}

RPCObject::~RPCObject()
{
}

DObject* RPCObject::newObject(DStruct* dstruct, DValue const& args)
{
  return (new RPCObject(dstruct, args)); //XXX copy network stream, handle connection & struct etc...
}

DValue RPCObject::getValue(std::string const& name) const
{
  this->__networkStream.write(std::string("getValue"));
  this->__networkStream.write(this->__id);
  this->__networkStream.write(name);

  DType  dtype = this->instanceOf()->attribute(name).type();
  
  NetworkStream& networkStream = const_cast<NetworkStream&>(this->__networkStream);
  DValue returnValue = this->__serializer->deserialize(networkStream, dtype.getType());

  return (returnValue);
}

void RPCObject::setValue(std::string const& name, DValue const &v)
{
  this->__networkStream.write(std::string("setValue"));
  this->__networkStream.write(this->__id);
  this->__networkStream.write(name);

  //don't need get type if serialized in a streamstring or binary ? 
  this->__serializer->serialize(this->__networkStream, v, this->instanceOf()->attribute(name).type().getType());
}
                                        
DValue RPCObject::call(std::string const& name, DValue const &v)
{
  this->__networkStream.write(std::string("call"));
  this->__networkStream.write(this->__id);
  this->__networkStream.write(name);
  DType  dtype = this->instanceOf()->attribute(name).type();

  /* Send argument (object is not compatible) */
  this->__serializer->serialize(this->__networkStream, v, dtype.getArgumentType());
 
  /* get return value */
  DValue dvalue = this->__serializer->deserialize(this->__networkStream, dtype.getReturnType());

  return (dvalue);
}

DValue RPCObject::call(std::string const& name)
{
  this->__networkStream.write(std::string("call0"));
  this->__networkStream.write(this->__id);
  this->__networkStream.write(name);

  DType  dtype = this->instanceOf()->attribute(name).type();
  DValue dvalue = this->__serializer->deserialize(this->__networkStream, dtype.getReturnType());

  return (dvalue);
}

//XXX XXX XX
DValue RPCObject::getValue(size_t index) const
{
  DAttribute attribute = this->instanceOf()->attribute(index);
  //XXX error call getValue on method ...
 
  if (attribute.type().getType() != DType::DMethodType)
    return this->getValue(attribute.name());
  return RealValue<DObject*>(DNone);
}

void RPCObject::setValue(size_t index, DValue const &value)
{
  std::string name = this->instanceOf()->attribute(index).name();
  this->setValue(name, value); 
}

DValue RPCObject::call(size_t index, DValue const &value)
{
  std::string name = this->instanceOf()->attribute(index).name();
  return (this->call(name, value));  //use call 0 if None ?
}

DObject* RPCObject::clone() const
{
  return (new RPCObject(*this)); //XXX 
}

}
