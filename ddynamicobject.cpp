#include "ddynamicobject.hpp"
#include "dstruct.hpp"
#include "dvalue.hpp"
#include "drealvalue.hpp"

namespace Destruct
{

DDynamicObject::DDynamicObject(DStruct * class_) : DObject(class_), __values(class_->attributeCount(), 0), __object(0)
{
  // we can't initialize 'values' here, as 'this' is not a valid pointer
}

DDynamicObject::DDynamicObject(DDynamicObject const & rhs) : DObject(rhs), __values(rhs.instanceOf()->attributeCount(), 0)
{
  // we can't initialize 'values' here, as 'this' is not a valid pointer
}

DDynamicObject::~DDynamicObject()
{
  for (size_t idx = 0; idx != this->__values.size(); ++idx)
  {
    delete this->__values[idx];
    this->__values[idx] = NULL;
  }
}

DValue DDynamicObject::getValue(size_t idx) const
{
  return (this->__values[idx]->getFinal());
}

void DDynamicObject::setValue(size_t idx, DValue const & v)
{
  this->__values[idx]->set(v);
}

DValue DDynamicObject::call(size_t idx, DValue const& v)
{
  return (this->__values[idx]->getFinal().get<DFunctionObject *>()->call(v));
}

DObject* DDynamicObject::privateObject() const
{
  return (this->__object);
}

void DDynamicObject::setPrivate(DObject * _privateObject)
{
  this->__object = _privateObject;
}

BaseValue * DDynamicObject::getBaseValue(size_t idx)
{
  return (this->__values[idx]);
}

BaseValue const * DDynamicObject::getBaseValue(size_t idx) const
{
  return (this->__values[idx]);
}

void DDynamicObject::init(DDynamicObject* self)
{
   DObject const * def = instanceOf()->defaultDObject();

   if (def) // def is not necessarily a DDynamicObject!
   {
     for (size_t idx = 0; idx != this->__values.size(); ++idx)
     {
        this->__values[idx] = DObject::getBaseValue(def, idx)->clone(self);
     }
   }
   else
   {
     DStruct::DAttributeIterator a;
     ValueContainer::iterator i = this->__values.begin();

     for (a = instanceOf()->attributeBegin(); a != instanceOf()->attributeEnd(); ++a, ++i)
     {
       *i = a->type().newValue();
     }
  }
}

void DDynamicObject::copy(DDynamicObject * self, DDynamicObject const & rhs)
{
  for (size_t idx = 0; idx != this->__values.size(); ++idx)
  {
     this->__values[idx] = rhs.__values[idx]->clone(self);
  }
}

}
