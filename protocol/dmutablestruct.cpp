#include "dmutablestruct.hpp"
#include <iostream>
namespace Destruct
{

DMutableStruct::DMutableStruct(DStruct const * base, const DUnicodeString & name, CreateMutableObjectFunction objectFunction) : DStruct(base, name, NULL), __createObject(objectFunction)
{

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
