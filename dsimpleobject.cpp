#include "dsimpleobject.hpp"
#include <string>
#include <iostream>

#include "dvalue.hpp"

namespace Destruct
{

DSimpleObject::DSimpleObject(DStruct * class_, DValue const& args) : DDynamicObject(class_, args)
{
  this->init(this);
}

DSimpleObject::~DSimpleObject()
{
}

DSimpleObject::DSimpleObject(DSimpleObject const & rhs) : DDynamicObject(rhs)
{
  this->copy(this, rhs);
}

DObject* DSimpleObject::newObject(DStruct * dstruct, DValue const& args)
{
  return (new DSimpleObject(dstruct, args));
}

DObject* DSimpleObject::clone() const
{
 return (new DSimpleObject(*this));
}


}
