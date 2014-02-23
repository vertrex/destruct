#include "dsimpleobject.hpp"

namespace Destruct
{

DSimpleObject::DSimpleObject(DStruct * class_, DValue const& args) : DDynamicObject(class_, args)
{
  this->init(this);
}

DSimpleObject::DSimpleObject(DSimpleObject const & rhs) : DDynamicObject(rhs)
{
  this->copy(this, rhs);
}

DObject* DSimpleObject::newObject(DStruct * myClass, DValue const& args)
{
  return (new DSimpleObject(myClass, args));
}

DObject* DSimpleObject::clone() const
{
 return (new DSimpleObject(*this));
}

}
