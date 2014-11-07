#include "dobject.hpp"
#include "dstruct.hpp"
#include "dvalue.hpp"
#include "dnullobject.hpp"
#include "dexception.hpp"
#include "drealvalue.hpp"

namespace Destruct
{
 
DObject::DObject(DStruct * dstructDef, DValue const& args) : RefcountPolicy(),  __dstructDef(dstructDef)
{
}

DStruct * DObject::instanceOf() const
{
  return (this->__dstructDef);
}

DValue DObject::getValue(DUnicodeString const& name) const
{
  int32_t index = instanceOf()->findAttribute(name); //check for not found ?
  if (index == -1)
    throw DException(this->instanceOf()->name() + " instance has no attribute " + name);

  return this->getValue(index);
}

void DObject::setValue(DUnicodeString const& name, DValue const& v)
{
  int32_t index = this->instanceOf()->findAttribute(name); //check not found
  if (index == -1)
    throw DException(this->instanceOf()->name() + " instance has no attribute " + name);
  this->setValue(index, v);
}

DValue DObject::call(std::string const& name, DValue const& v) //const ? XXX  : throw
{
  int32_t index = this->instanceOf()->findAttribute(name);

  if (index == -1)
    throw DException(this->instanceOf()->name() + " instance has no attribute " + name); 
  return (this->call(index, v));
}

DValue DObject::call(std::string const& name) //const ? XXX  : throw
{
  int32_t index = this->instanceOf()->findAttribute(name);

  if (index == -1)
    throw DException(this->instanceOf()->name() + " instance has no attribute " + name); 
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
