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
RPCObject::RPCObject(NetworkStream stream, uint64_t id, DStruct* dstruct, ObjectManager & objects) : DDynamicObject(dstruct, RealValue<DObject*>(DNone)), __id(id), __stream(stream), __serializer(new DSerializeRPC(stream, objects)) //args
{
  //serialize DStruct & create member ? 
  //DStream = args.getValue("Stream"); //XXX else throw !

  this->init(this);
}

RPCObject::RPCObject(DStruct* dstruct, DValue const& args) : DDynamicObject(dstruct, args), __stream(0, RealValue<DInt64>(0)) //XXX fix me stream as DOBject
{
  this->init(this);
}

RPCObject::RPCObject(RPCObject const & rhs) : DDynamicObject(rhs), __stream(rhs.__stream)
{
  this->copy(this, rhs);
}

RPCObject::~RPCObject()
{
}

DObject* RPCObject::newObject(DStruct* dstruct, DValue const& args)//XXX fix me stream
{
  return (new RPCObject(dstruct, args)); //XXX copy network stream, handle connection & struct etc...
}

DValue RPCObject::getValue(std::string const& name) const
{
  this->__stream.write(std::string("getValue"));
  this->__stream.write(this->__id);
  this->__stream.write(name);

  std::string valueBuffer;
  ((NetworkStream)this->__stream).read(valueBuffer);

  DStruct* dstruct =  Destruct::instance().find("DStreamString"); //Virer moi cet merde :)
  DStreamString streamString = DStreamString(dstruct, RealValue<DObject*>(DNone));
  streamString.write(valueBuffer.c_str(), valueBuffer.size());
  streamString.write("\x00", 1);

  DType  dtype = this->instanceOf()->attribute(name).type();
  DValue dvalue = this->__serializer->deserialize(streamString, dtype.getType());

  return dvalue;
}

void RPCObject::setValue(std::string const& name, DValue const &v)
{
  this->__stream.write(std::string("setValue"));
  this->__stream.write(this->__id);
  this->__stream.write(name);

  DStruct* dstruct =  Destruct::instance().find("DStreamString") ;
  DStreamString streamString = DStreamString(dstruct, RealValue<DObject*>(DNone));
  streamString << (DValue&)v; ///XXX fix me with deserializer please ? for object etc.. ?

  this->__stream.write(streamString.str());
}
                                        
DValue RPCObject::call(std::string const& name, DValue const &v)//XXX UNTESTED
{
  this->__stream.write(std::string("call"));
  this->__stream.write(this->__id);
  this->__stream.write(name);

  /* Send argument (object is not compatible) */
  DStruct* dstruct =  Destruct::instance().find("DStreamString") ;
  DStreamString streamString = DStreamString(dstruct, RealValue<DObject*>(DNone));
  streamString << (DValue&)v; ///XXX fix me with deserializer please ? for object etc.. ?

  this->__stream.write(streamString.str());

  /* get return value */
  std::string returnValueBuffer;
  ((NetworkStream)this->__stream).read(returnValueBuffer);

  DStreamString rstreamString = DStreamString(dstruct, RealValue<DObject*>(DNone));
  rstreamString.write(returnValueBuffer.c_str(), returnValueBuffer.size()); 
  rstreamString.write("\x00", 1); // heheheheheoooo c pas tjrs des string ! 

  DType  dtype = this->instanceOf()->attribute(name).type();
  DValue dvalue = this->__serializer->deserialize(rstreamString, dtype.getReturnType()); // ==>  streamString >> dvalue;

  return dvalue;

}

DValue RPCObject::call(std::string const& name)
{
  this->__stream.write(std::string("call0"));
  this->__stream.write(this->__id);
  this->__stream.write(name); //send func name to call

  std::string returnValueBuffer;
  ((NetworkStream)this->__stream).read(returnValueBuffer);

  DStruct* dstruct =  Destruct::instance().find("DStreamString") ;
  DStreamString streamString = DStreamString(dstruct, RealValue<DObject*>(DNone));
  streamString.write(returnValueBuffer.c_str(), returnValueBuffer.size()); 
  streamString.write("\x00", 1); // heheheheheoooo c pas tjrs des string ! 

  DType  dtype = this->instanceOf()->attribute(name).type();
  DValue dvalue = this->__serializer->deserialize(streamString, dtype.getReturnType()); // ==>  streamString >> dvalue;

  return dvalue;
}

void RPCObject::wait(void)
{
  //this->recv();
}

DObject* RPCObject::clone() const
{
  return (new RPCObject(*this)); //XXX 
}

 }
