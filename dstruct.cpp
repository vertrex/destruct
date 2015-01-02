#include <iterator>

#include "destruct.hpp"
#include "dexception.hpp"
#include "drealvalue.hpp"
#include "dnullobject.hpp"

namespace Destruct
{

DStruct::DStruct(DStruct const * base, const DUnicodeString & name, CreateObjectFunction objectFunction) : __baseClass(base), __name(name), __createObject(objectFunction), __definitionFix(false), __defaultObject(0)
{
  this->__baseInit();
}

void DStruct::__baseInit()
{
  if (this->__baseClass)
  {
     this->__baseClass->__definitionFix = true;
     std::copy(this->__baseClass->attributeBegin(), this->__baseClass->attributeEnd(), 
               std::back_inserter<DAttributeContainer>(this->__effectiveAttributes));
  }
}

DStruct::~DStruct()
{
  //should remove from Destruct if exist !
  Destruct::Destruct::instance().unregister(this);
}

DUnicodeString  DStruct::name() const
{
  return (this->__name);
}
//XXX declare destructor to avoid destruction of attribute because some attribute are static and could be used twice 

//DStruct const * const  DStruct::base() const;
//{
//return (this->__baseClass);
//}

DObject* DStruct::newObject()
{
  return (this->newObject(RealValue<DObject*>(DNone)));
}

DObject* DStruct::newObject(DValue const& args) //for mutable non const so ++definitionFIx=false ?
{
  this->__definitionFix = true;
  if (*this->__createObject == NULL)
    return (NULL);
  
  return (*this->__createObject)(this, args);
}

DObject const* DStruct::defaultDObject() const
{
  return (this->__defaultObject);
}

void DStruct::setDefault(DObject const * def)
{
  this->__defaultObject = def;
}

DStruct::DAttributeIterator    DStruct::attributeBegin() const
{
  return (this->__effectiveAttributes.begin());
}

DStruct::DAttributeIterator  DStruct::attributeEnd() const
{
  return (this->__effectiveAttributes.end());
}

DAttribute const& DStruct::attribute(size_t idx) const
{
  return (this->__effectiveAttributes[idx]);
}

DAttribute const& DStruct::attribute(DUnicodeString const& name) const
{
  int32_t idx =  this->findAttribute(name);
  
  if (idx == -1)
    throw DException(this->name() + " as no attribute " + name);
  
  return (this->attribute(idx));
}

void DStruct::addAttribute(const DAttribute& attribute)
{
  if (!this->__definitionFix)
  {
    this->__ownAttributes.push_back(attribute);
    this->__effectiveAttributes.push_back(attribute);
  }
}

size_t DStruct::attributeCount() const
{
  return (this->__effectiveAttributes.size());
}

int32_t DStruct::findAttribute(std::string const& name) const
{
  for (DAttributeContainer::const_reverse_iterator i = this->__effectiveAttributes.rbegin(); i != this->__effectiveAttributes.rend(); ++i)
  {
     if (i->name() == name)
     {
       return std::distance(i, this->__effectiveAttributes.rend()) - 1; 
     } 
  }
  return (-1);
}


}
