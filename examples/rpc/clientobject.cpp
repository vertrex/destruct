#include "destruct.hpp"
#include "protocol/dserialize.hpp"

#include "networkstream.hpp"
#include "serializerpc.hpp"
#include "clientobject.hpp"


namespace Destruct {

/**
 *  ClientObject Proxy object that handle transparent remote communication and let you use your object as a local object
 */
ClientObject::ClientObject(NetworkStream& stream, DSerialize* serializer, uint64_t id, DStruct* dstruct) : DObject(*dstruct->newObject()), __id(id), __networkStream(stream), __serializer(serializer), __object(dstruct->newObject())
{
  //this->init(this);
}

ClientObject::ClientObject(DStruct* dstruct, DValue const& args) : DObject(dstruct, args), __networkStream(*(new NetworkStream(NULL, RealValue<DInt32>(0))))
{
  //this->init(this);
}

ClientObject::ClientObject(ClientObject const & rhs) : DObject(rhs), __networkStream(rhs.__networkStream)
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

DValue ClientObject::getValue(std::string const& name) const
{
   this->__networkStream.write(std::string("getValue"));
   this->__networkStream.write(this->__id);
   this->__networkStream.write(name);
   this->__networkStream.flush();

  NetworkStream& networkStream = const_cast<NetworkStream&>(this->__networkStream);
  DType  dtype = this->instanceOf()->attribute(name).type();
 
  if (dtype.getType() == DType::DMethodType)
  {
    DValue returnValue = this->__serializer->deserialize(networkStream, dtype.getArgumentType(), dtype.getReturnType());
    return (returnValue);
  } 

  DValue returnValue = this->__serializer->deserialize(networkStream, dtype.getType());

  return (returnValue);
}

void ClientObject::setValue(std::string const& name, DValue const &v)
{
  this->__networkStream.write(std::string("setValue"));
  this->__networkStream.write(this->__id);
  this->__networkStream.write(name);

  //don't need get type if serialized in a streamstring or binary ? 
  this->__serializer->serialize(this->__networkStream, v, this->instanceOf()->attribute(name).type().getType());
  this->__networkStream.flush();
}
                                        
DValue ClientObject::call(std::string const& name, DValue const &args)
{
  DType  dtype = this->instanceOf()->attribute(name).type();
  if (name == "serializeText")
  {
    BaseValue *b = DObject::getBaseValue(this->__object, this->instanceOf()->findAttribute(name));
    DValue v = b->getFinal().get<DFunctionObject* >()->call(args); 
    return RealValue<DObject*>(DNone);
  }

  this->__networkStream.write(std::string("call"));
  this->__networkStream.write(this->__id);
  this->__networkStream.write(name);
  /* Send argument (object is not compatible) */
  this->__serializer->serialize(this->__networkStream, args, dtype.getArgumentType());
  this->__networkStream.flush();
 
  /* get return value */
  DValue dvalue = this->__serializer->deserialize(this->__networkStream, dtype.getReturnType());

  return (dvalue);
}

DValue ClientObject::call(std::string const& name)
{
  this->__networkStream.write(std::string("call0"));
  this->__networkStream.write(this->__id);
  this->__networkStream.write(name);
  this->__networkStream.flush();

  DType  dtype = this->instanceOf()->attribute(name).type();
  DValue dvalue = this->__serializer->deserialize(this->__networkStream, dtype.getReturnType());

  return (dvalue);
}

DValue ClientObject::getValue(size_t index) const
{
  DAttribute attribute = this->instanceOf()->attribute(index);
  return (this->getValue(attribute.name()));
}

void ClientObject::setValue(size_t index, DValue const &value)
{
  std::string name = this->instanceOf()->attribute(index).name();
  this->setValue(name, value); 
}

DValue ClientObject::call(size_t index, DValue const &value)
{
  std::string name = this->instanceOf()->attribute(index).name();
  return (this->call(name, value));  //use call 0 ? 
}

DObject* ClientObject::clone() const
{
  return (new ClientObject(*this));
}  

BaseValue* ClientObject::getBaseValue(size_t index)
{
  std::cout << "get base value " << std::endl;
  return (NULL); //XXX  
}


BaseValue const* ClientObject::getBaseValue(size_t index) const
{
  std::cout << "get base value " << std::endl;
  return (NULL);
}
}
