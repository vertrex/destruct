#include "dmutablestruct.hpp"

namespace Destruct
{

DMutableStruct::DMutableStruct(DStruct const * base, const DUnicodeString & name, CreateObjectFunction objectFunction) : DStruct(base, name, objectFunction)
{

}

DObject* DMutableStruct::newObject() const
{
        //if (*this->__createObject == NULL)
        //return (NULL);
        // 
        //return (*this->__createObject)(this);
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
