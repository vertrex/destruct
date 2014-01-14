#include "dobject.hpp"
#include "dstruct.hpp"
#include "dvalue.hpp"
#include "dnullobject.hpp"
#include "drealvalue.hpp"

namespace Destruct
{
                                                                //faudrait le ref count ou le forcer ds le dico aussi simple que ca !
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

DValue DObject::call(std::string const& name, DValue const& v) //const ? XXX 
{
  int32_t index = this->instanceOf()->findAttribute(name);
 //XXX mismatch int32_t en return de fin et pass size_t a call ou value pas logique 
  if (index != -1)
  {
    return (this->call(index, v));
  }
  std::cout << "DObject::" << name << "() : Attribute not found" << std::endl; //XXX throw error ? 
  return RealValue<DObject* >(DNone);
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

