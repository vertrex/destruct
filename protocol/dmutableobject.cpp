#include "dmutableobject.hpp"
#include "../dstruct.hpp"

namespace Destruct
{

DMutableObject::DMutableObject(DStruct* dstructDef) : DDynamicObject(dstructDef)
{
  this->init(this);
}

DMutableObject::DMutableObject(DMutableObject const & rhs) : DDynamicObject(rhs)
{
  this->copy(this, rhs);
}

DObject* DMutableObject::newObject(DStruct* myClass)
{
  return (new DMutableObject(myClass));
}

DObject* DMutableObject::clone() const
{
 return (new DMutableObject(*this));
}

}
