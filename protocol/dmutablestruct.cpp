#include "dmutablestruct.hpp"

namespace Destruct
{

DMutableStruct::DMutableStruct(DStruct const * base, const DUnicodeString & name, CreateMutableObjectFunction objectFunction) : DStruct(base, name, NULL), __createObject(objectFunction)
{

}

DObject* DMutableStruct::newObject() //for mutable non const so ++definitionFIx=false ?
{
  if (*this->__createObject == NULL)
    return (NULL);
  
  return (*this->__createObject)(new DMutableStruct(*this)); //Delete the copy !
}

void DMutableStruct::addAttribute(const DAttribute& attribute)
{
  this->__ownAttributes.push_back(attribute);           //?
  this->__effectiveAttributes.push_back(attribute);     //?
}

void DMutableStruct::replaceAttribute(size_t idx, const DAttribute& attribute)
{
   this->__effectiveAttributes[idx] = attribute;
}

}
