#include "dstruct.hpp"
#include "dfunction.hpp"
//#include "drealvalue.hpp"
#include "dvalue.hpp"
#include "dobject.hpp"
#include "ddynamicobject.hpp"

namespace Destruct
{

DDynamicObject::DDynamicObject(DStruct * dstruct, DValue const& args) : DObject(dstruct, args), __values(dstruct->attributeCount(), 0), __object(NULL)
{
  // we can't initialize 'values' here, as 'this' is not a valid pointer
}

DDynamicObject::DDynamicObject(DDynamicObject const & rhs) : DObject(rhs), __values(rhs.instanceOf()->attributeCount(), 0), __object(NULL)
{
  // we can't initialize 'values' here, as 'this' is not a valid pointer
}

void DDynamicObject::init(DDynamicObject* self)
{
   DObject const * def = instanceOf()->defaultDObject();//XXX use it to set default value ! not used yet
  
   if (this->baseObject())
   {
     DObject* baseObject = this->baseObject();
     const DStruct* base = this->base();
    
     uint32_t index = 0;
     uint32_t attributeCount = (uint32_t)base->attributeCount();
     for (; index < attributeCount; ++index)
       this->__values[index] = baseObject->getBaseValue(index);

     for (; index < this->instanceOf()->attributeCount(); ++index)
       this->__values[index] = this->instanceOf()->attribute(index).type().newValue();
   }
   else
   {
     if (def) // def is not necessarily a DDynamicObject! XXX not used yet
     {
       for (size_t idx = 0; idx != this->__values.size(); ++idx)
       {
          this->__values[idx] = DObject::getBaseValue(def, idx)->clone(self);
       }
     }
     else
     {
       DStruct::DAttributeIterator a;
       std::vector<BaseValue*>::iterator i = this->__values.begin();

       for (a = instanceOf()->attributeBegin(); a != instanceOf()->attributeEnd(); ++a, ++i)
       {
         *i = a->type().newValue();
       }
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
//  if (idx > this->__values.size)
 //throw DExceptions
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

BaseValue * DDynamicObject::getBaseValue(size_t idx)
{
  return (this->__values[idx]);
}

BaseValue const * DDynamicObject::getBaseValue(size_t idx) const
{
  return (this->__values[idx]);
}

//DObject* DDynamicObject::privateObject() const
//{
  //return (this->__object);
//}

//void DDynamicObject::setPrivate(DObject * _privateObject)
//{
  //this->__object = _privateObject;
//}

}
