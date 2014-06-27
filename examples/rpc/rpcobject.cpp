#include "rpcobject.hpp"
#include "destruct.hpp"
#include "protocol/dstream.hpp"

namespace Destruct {

/**
 *  RPCObject Proxy object that handle transparent remote communication and let you use your object as a local object
 */
RPCObject::RPCObject(NetworkStream stream, std::string const& uri, DStruct* dstruct) : DDynamicObject(dstruct, RealValue<DObject*>(DNone)), __stream(stream), __URI(uri) //args
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

DObject* RPCObject::newObject(RPCStruct* dstruct, DValue const& args)//XXX fix me stream
{
  return (new RPCObject(dstruct, args)); //XXX copy network stream, handle connection & struct etc...
}

DValue RPCObject::getValue(std::string const& name) const
{
  this->__stream.write(std::string("getValue"));
  this->__stream.write(name);

  std::string value;
  ((NetworkStream)this->__stream).read(value);

  DValue dvalue = DDynamicObject::getValue(name); 
  //((DStream&)this->__stream) >> dvalue; //best :) 
  DStruct* dstruct =  Destruct::instance().find("DStreamString") ;
  DStreamString streamString = DStreamString(dstruct, RealValue<DObject*>(DNone));

  streamString.write(value.c_str(), value.size());
  streamString.write("\x00", 1);
  streamString >> dvalue;
  return dvalue;
}

void RPCObject::setValue(std::string const& name, DValue const &v)
{
  this->__stream.write(std::string("setValue"));
  this->__stream.write(name);

  DStruct* dstruct =  Destruct::instance().find("DStreamString") ;
  DStreamString streamString = DStreamString(dstruct, RealValue<DObject*>(DNone));
  streamString << (DValue&)v;

  this->__stream.write(streamString.str());
}
                                        
DValue RPCObject::call(std::string const& name, DValue const &v)
{
  this->__stream.write(std::string("call"));
  this->__stream.write(name);
//XXX code me generic
  DUnicodeString value = v.get<DUnicodeString>();
  this->__stream.write(value);

  std::string rvalue;
  ((NetworkStream)this->__stream).read(rvalue);
  return RealValue<DUnicodeString>(rvalue);
}

DValue RPCObject::call(std::string const& name)
{
//XXX code me generic
  this->__stream.write(std::string("call0"));
  this->__stream.write(name);

  std::string rvalue;
  ((NetworkStream)this->__stream).read(rvalue);
  return RealValue<DUnicodeString>(rvalue);
}

void RPCObject::wait(void)
{
  //this->recv();
}

DObject* RPCObject::clone() const
{
  return (new RPCObject(*this)); //XXX 
}

//DValue RPCObject::getValue(size_t index) const
//{
  //return (this->__realObject->getValue(index));
//}

//void RPCObject::setValue(size_t idx, DValue const &v)
//{
  //this->__realObject->setValue(idx, v);
//}

//DValue RPCObject::call(size_t index, DValue const &v)
//{
  //return (this->__realObject->call(index, v));
//}

 }
