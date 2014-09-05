#include "dstruct.hpp"
#include "dnullobject.hpp"
#include "dexception.hpp"
#include "drealvalue.hpp"

#include "protocol/dmutablestruct.hpp"
#include "protocol/dmutableobject.hpp"

namespace Destruct
{

DMutableObject::DMutableObject(const DUnicodeString& name, DValue const& args) : DDynamicObject(static_cast<DStruct* >(new DMutableStruct(NULL, name, DMutableObject::newObject)), args)
{
  this->init(this);
}

DMutableObject::DMutableObject(DMutableStruct* dstructDef, DValue const& args) : DDynamicObject(dstructDef, args)
{
  this->init(this);
}

DMutableObject::DMutableObject(DMutableObject const & rhs) : DDynamicObject(rhs)
{
  this->copy(this, rhs);
}

DMutableObject::~DMutableObject(void)
{
  delete this->instanceOf();
}

DObject* DMutableObject::newObject(DMutableStruct* myClass, DValue const& args)
{
  return (new DMutableObject(myClass, args));
}

DObject* DMutableObject::clone() const
{
 return (new DMutableObject(*this));
}

DValue DMutableObject::getValue(size_t idx) const
{
  if (idx > this->__values.size())
    throw Destruct::DException("DMutableObject::Value(idx) doesn't exist.");
  
  return (this->__values[idx]->getFinal());

  //XXX values must be set to 0 by default 
  return Destruct::RealValue<Destruct::DObject*>(Destruct::DNone);
}

void DMutableObject::setValue(size_t idx, DValue const & v)
{
  if (idx >= this->__values.size())
  {
    DStruct* dstruct = this->instanceOf();
    for (size_t i = this->__values.size() ; i < dstruct->attributeCount(); i++)
        this->__values.push_back(dstruct->attribute(i).type().newValue());
  }
  this->__values[idx]->set(v);
}

DValue DMutableObject::call(size_t idx, DValue const& v)
{
   if (idx > this->__values.size())
     throw Destruct::DException("Value doesn't exist.");
 
  return (this->__values[idx]->getFinal().get<DFunctionObject *>()->call(v));
}

void DMutableObject::setValueAttribute(DType::Type_t type, std::string const& name, DValue const& v)
{
  DAttribute attribute(type, name);

  this->instanceOf()->addAttribute(attribute);
  this->__values.push_back(attribute.type().newValue()); // XXX clone 
  this->__values.back()->set(v);
}

}
