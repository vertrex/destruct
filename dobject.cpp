#include "dobject.hpp"
#include "dstruct.hpp"
#include "dvalue.hpp"
#include "dnullobject.hpp"
#include "drealvalue.hpp"
#include "dexception.hpp"

namespace Destruct
{
 
DObject::DObject(DStruct * dstructDef) : RefcountPolicy(),  __dstructDef(dstructDef)
{
}

DStruct * DObject::instanceOf() const
{
  return (this->__dstructDef);
}

DValue DObject::getValue(DUnicodeString const& name) const
{
  size_t idx = instanceOf()->findAttribute(name); //check for not found ?
  return this->getValue(idx);
}

void DObject::setValue(DUnicodeString const& name, DValue const& v)
{
  size_t index = this->instanceOf()->findAttribute(name); //check not found
  this->setValue(index, v);
}

DValue DObject::call(std::string const& name, DValue const& v) //const ? XXX  : throw
{
  int32_t index = this->instanceOf()->findAttribute(name);

  if (index == -1)
    throw DException(this->instanceOf()->name() + " instance as no attribute " + name); 
  return (this->call(index, v));
}

DValue DObject::call(std::string const& name) //const ? XXX  : throw
{
  int32_t index = this->instanceOf()->findAttribute(name);

  if (index == -1)
    throw DException(this->instanceOf()->name() + " instance as no attribute " + name); 
  return (this->call(index, RealValue<DObject*>(Destruct::DNone)));
}

BaseValue* DObject::getBaseValue(DObject* dobject, size_t index)
{
  return (dobject->getBaseValue(index));
}

BaseValue const* DObject::getBaseValue(DObject const* dobject, size_t index)
{
  return (dobject->getBaseValue(index));
}

}

