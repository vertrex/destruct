#include "dnullobject.hpp"
#include "dvalue.hpp"
#include "drealvalue.hpp"
#include "dnullobject.hpp"

#include "dsimpleobject.hpp"

namespace Destruct
{

DNullObject*  DNullObject::instance()
{
  static DNullObject  instance; //XXX sert a rien
  instance.addRef();
  return &instance;
}

DStruct* DNullObject::__dstructInstance()
{
  static DStruct dstruct(NULL, "None", DSimpleObject::newObject);
  return &dstruct;
}

DNullObject::DNullObject() : DObject(this->__dstructInstance(), RealValue<DObject*>(DNone))
{
}

DNullObject::DNullObject(DValue const& args) : DObject(this->__dstructInstance(), args) //XXX args sert clairement a rien
{
//XXX use newobject to return singleton ?
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

DValue DNullObject::getValue(DUnicodeString const& name) const
{
  return RealValue<DObject*>((DObject*)this);
}

void DNullObject::setValue(DUnicodeString const& name, DValue const& value)
{
}

DValue DNullObject::call(DUnicodeString const& name, DValue const& value)
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
