#include "dsimpleobject.hpp"

namespace Destruct
{

DSimpleObject::DSimpleObject(DStruct const * class_) : DDynamicObject(class_)
{
  this->init(this);
}

DSimpleObject::DSimpleObject(DSimpleObject const & rhs) : DDynamicObject(rhs)
{
  this->copy(this, rhs);
}

DObject* DSimpleObject::newObject(DStruct const* myClass)
{
  return (new DSimpleObject(myClass));
}

DObject* DSimpleObject::clone() const
{
 return (new DSimpleObject(*this));
}

}
