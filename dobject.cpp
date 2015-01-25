#include "dobject.hpp"
#include "dstruct.hpp"
#include "dvalue.hpp"
#include "dnullobject.hpp"
#include "dexception.hpp"
#include "drealvalue.hpp"

namespace Destruct
{
 
DObject::DObject(DStruct* dstruct, DValue const& args) : RefcountPolicy(),  __dstruct(dstruct), __baseObject(NULL)
{
  if (dstruct && dstruct->base())
    this->__baseObject = ((DStruct*)dstruct->base())->newObject(RealValue<DObject*>(this));
}

DObject::DObject(const DObject& copy) : __dstruct(copy.__dstruct), __baseObject(copy.__baseObject)
{
}

DStruct* DObject::instanceOf(void) const
{
  return (this->__dstruct);
}

DStruct const * const DObject::base(void) const
{
  return (this->__dstruct->base());
}

DObject* DObject::baseObject(void) const
{
  return (this->__baseObject);
}

DValue DObject::getValue(DUnicodeString const& name) const
{
  int32_t index = this->instanceOf()->findAttribute(name); //check not found
  if (index != -1)
    return (this->getValue(index));

  throw DException(this->instanceOf()->name() + "::getValue instance has no attribute " + name);
}

void DObject::setValue(DUnicodeString const& name, DValue const& v)
{
  int32_t index = this->instanceOf()->findAttribute(name); //check not found
  if (index != -1)
   return (this->setValue(index, v));

  throw DException(this->instanceOf()->name() + "::setValue instance has no attribute " + name);
}

DValue DObject::call(DUnicodeString const& name, DValue const& v) //const ? XXX  : throw
{
  int32_t index = this->instanceOf()->findAttribute(name);
  if (index != -1)
   return (this->call(index, v));

  throw DException(this->instanceOf()->name() + "::call instance has no attribute " + name); 
}

DValue DObject::call(DUnicodeString const& name) //const ? XXX  : throw
{
  int32_t index = this->instanceOf()->findAttribute(name);
  if (index != -1)
    return (this->call(index, RealValue<DObject*>(DNone)));

  throw DException(this->instanceOf()->name() + "::call instance has no attribute " + name); 
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
