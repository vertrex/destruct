#include "rpcobject.hpp"

RPCObject::RPCObject(NetworkStream stream) : DMutableObject("RPCObject", RealValue<DObject*>(DNone)), __stream(stream)
{
  //serialize DStruct & create member ? 
}

RPCObject::~RPCObject()
{
}

DValue RPCObject::getValue(std::string const& name) const
{
  //receiveValueType 
  std::cout << "send cmd getValue" << std::endl;
  this->__stream.write(std::string("getValue"));
  this->__stream.write(name);

  std::string value;
  ((NetworkStream)this->__stream).read(value);
  return RealValue<DUnicodeString>(value);
}

void RPCObject::setValue(std::string const& name, DValue const &v)
{
  std::cout << "setValue(" << name << ")" << std::endl;
  this->__stream.write(std::string("setValue"));
  this->__stream.write(name);
  DUnicodeString value = v.get<DUnicodeString>();
  this->__stream.write(value);
}
                                        
DValue RPCObject::call(std::string const& name, DValue const &v)
{
  std::cout << "call(" << name << ")" << std::endl;
  this->__stream.write(std::string("call"));
  this->__stream.write(name);

  DUnicodeString value = v.get<DUnicodeString>();
  this->__stream.write(value);

  std::string rvalue;
  ((NetworkStream)this->__stream).read(rvalue);
  return RealValue<DUnicodeString>(rvalue);
}

DValue RPCObject::call(std::string const& name)
{
  std::cout << "call0(" << name << ")" << std::endl;
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

//DObject* RPCObject::clone() const
//{
  //return (this->__realObject->clone());
//}

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


