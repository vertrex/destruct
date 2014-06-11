#include "rpcobject.hpp"

namespace Destruct {

/**
 *  RPCStruct Proxy that handle transparent remote communication and localy create struct from a remote dstruct 
 */
//Really useful au final on cree une struct on la met ds la base (bon permet d etre sur que l object et distant)
// puis c le RPC OBject qui va etre cree depuis un mutable ou pas et qui saura se deserialier 

RPCStruct::RPCStruct(NetworkStream stream, const DUnicodeString & name, CreateMutableObjectFunction objectFunction) : DStruct(NULL, name, NULL), __createObject(objectFunction)
{
   stream.write(std::string("getDStruct"));
   std::string  rawStruct;
   stream.read(rawStruct);
   std::cout << "RPCStruct receive " << rawStruct << std::endl;
//   DSerializeRaw << rawStruct;
//  Struct =  DSerializeRaw(stream) 
   //registerStruct(struct);
  //RPCObject(Destruct.find(Struct)
}

DObject* RPCStruct::newObject(DValue const& args) //for mutable non const so ++definitionFIx=false ?
{
  if (*this->__createObject == NULL)
    return (NULL);
  
  return (*this->__createObject)(new RPCStruct(*this), args); //Delete the copy !
}

//void RPCStruct::addAttribute(const DAttribute& attribute)
//{
  //this->__ownAttributes.push_back(attribute);           //?
  //this->__effectiveAttributes.push_back(attribute);     //?
//}

//void RPCStruct::replaceAttribute(size_t idx, const DAttribute& attribute)
//{
   //this->__effectiveAttributes[idx] = attribute;
//}


/**
 *  RPCObject Proxy object that handle transparent remote communication and let you use your object as a local object
 */
//RPCObject::RPCObject(NetworkStream stream) : DMutableObject("RPCObject", RealValue<DObject*>(DNone)), __stream(stream)
RPCObject::RPCObject(NetworkStream stream, std::string const& name) : DDynamicObject(static_cast<DStruct* >(new RPCStruct(stream, name, RPCObject::newObject)), RealValue<DObject*>(DNone)), __stream(stream) //args !
{
  //serialize DStruct & create member ? 
  //
  //DStream = args.getValue("Stream"); //XXX else throw !

  this->init(this);
}

RPCObject::RPCObject(RPCStruct* dstruct, DValue const& args) : DDynamicObject(dstruct, args), __stream(0, RealValue<DInt64>(0)) //XXX fix me stream as DOBject
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
  //receiveValueType 
  std::cout << "send cmd getValue" << std::endl;
  this->__stream.write(std::string("getValue"));
  this->__stream.write(name);

  //deserializeRaw Value ...

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
