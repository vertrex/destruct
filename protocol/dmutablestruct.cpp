#include "dmutablestruct.hpp"
#include <iostream>
namespace Destruct
{

DMutableStruct::DMutableStruct(DStruct const * base, const DUnicodeString & name, CreateMutableObjectFunction objectFunction) : DStruct(base, name, NULL), __createObject(objectFunction)
{

}

DObject* DMutableStruct::newObject() //const
{
        //this->__definitionFix = true;
  if (*this->__createObject == NULL)
  {
    return (NULL);
  }
  return (*this->__createObject)(this);
}

void DMutableStruct::addAttribute(const DAttribute& attribute)
{
  std::cout << " MUTABLE ADD ATTIRUBGE DERIBED CALLED " << std::endl;
  //if (!this->__definitionFix)
  //{
    this->__ownAttributes.push_back(attribute);
    this->__effectiveAttributes.push_back(attribute);
    //}
}
//XXX pouvoir le cee en python / inherit dobject et changer la methode set/get pour set auto la value python ?  mutableObject.toto = 1  sans a avoir a ajouter la definition possibilite d heriter de DMutableObject aussi ...

//XXX UTILISER DEFAULT OBJECT ! ? et utiliser aussi ds DNONE voir DCOntainer  ?
//DObject const* DStruct::defaultDObject() const
//{
//return (this->__defaultObject);
//}
//
//void DStruct::setDefault(DObject const * def)
//{
//this->__defaultObject = def;
////}

}
