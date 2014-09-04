#ifndef DSTRUCT_CPPMUTABLE_OBJECT_HPP_
#define DSTRUCT_CPPMUTABLE_OBJECT_HPP_

#include "dcppobject.hpp"
#include "dmutablestruct.hpp"

namespace Destruct
{

template< typename CppClass >
class DCppMutable : public DCppObject<CppClass>  //public DCppObject<Mutable, CppClass> //DObject<Mutable, CppClass, RefCount> 
{
public:
  DCppMutable(DMutableStruct* dstruct, DValue const& args) : DCppObject<CppClass>(dstruct, args), __struct(dstruct)
  {
  }

  DCppMutable(const DCppMutable& copy) : DCppObject<CppClass>(copy)
  {
  }

  ~DCppMutable()
  {
    delete this->__struct;
  }

  static DObject* newObject(DMutableStruct* dstruct, DValue const& args)
  {
    return (new CppClass(args));
  }

protected:
  DMutableStruct*     __struct; //delete ?
};

template<typename CppClass>
inline DStruct* makeNewDMutable(DUnicodeString const& name)
{
  return ((DStruct*)new DMutableStruct(NULL, name, DCppMutable< CppClass >::newObject, CppClass::ownAttributeBegin(), CppClass::ownAttributeEnd()));
}

template<typename CppClass>
inline DStruct* makeNewDMutable(DStruct* base, DUnicodeString const& name)
{
  return ((DStruct*)new DMutableStruct(base, name, CppClass::DIterator::newObject, CppClass::ownAttributeBegin(), CppClass::ownAttributeEnd()));
}

}
#endif
