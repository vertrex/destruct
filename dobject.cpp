#include "dobject.hpp"
#include "dstruct.hpp"
#include "dvalue.hpp"

namespace Destruct
{

DObject::DObject(DStruct const* dstructDef) : __dstructDef(dstructDef), __refCount(1)
{
}

DStruct const* DObject::instanceOf() const
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

BaseValue* DObject::getBaseValue(DObject* dobject, size_t index)
{
  return (dobject->getBaseValue(index));
}

BaseValue const* DObject::getBaseValue(DObject const* dobject, size_t index)
{
  return (dobject->getBaseValue(index));
}

}

