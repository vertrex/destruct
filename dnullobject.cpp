#include "dnullobject.hpp"
#include "dvalue.hpp"
#include "drealvalue.hpp"

namespace Destruct
{

DNullObject*  DNullObject::instance()
{
  static DNullObject  instance;
  instance.addRef();
  return &instance;
}

DNullObject::DNullObject() : DObject(NULL)
{
}

DNullObject::~DNullObject()
{
}

void DNullObject::destroy(void)
{
  this->delRef();
}

DObject* DNullObject::clone() const
{
  return ((DObject*)this);
} 

DValue DNullObject::getValue(size_t index) const
{
  return RealValue<DObject*>((DNullObject*)this);
} 

void DNullObject::setValue(size_t index, DValue const&)
{
}

DValue DNullObject::call(size_t index, DValue const&)
{
  return (RealValue<DObject*>(this));
}

DValue DNullObject::getValue(std::string const& name) const
{
  return RealValue<DObject*>((DObject*)this);
}

void DNullObject::setValue(std::string const& name, DValue const& value)
{
}

DValue DNullObject::call(std::string const& name, DValue const& value)
{
  return RealValue<DObject*>(this); 
}

BaseValue* DNullObject::getBaseValue(size_t index)
{
  return (NULL); //XXX ?
};

BaseValue const* DNullObject::getBaseValue(size_t index) const
{
  return (NULL); //XXX ?
};



}
