#include "dobject.hpp"
#include "dstruct.hpp"
#include "dvalue.hpp"

namespace Destruct
{
                                                                //faudrait le ref count ou le forcer ds le dico aussi simple que ca !
DObject::DObject(DStruct const* dstructDef) : RefcountPolicy(),  __dstructDef(dstructDef)
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

DValue DObject::call(std::string const& name, DValue const& v) //const ? XXX 
{
  size_t index = this->instanceOf()->findAttribute(name);
  return (this->call(index, v));
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

