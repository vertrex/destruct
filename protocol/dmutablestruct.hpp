#ifndef DESTRUCT_DMUTABLE_STRUCT_HPP_
#define DESTRUCT_DMUTABLE_STRUCT_HPP_

#include "destruct.hpp"
#include "dstruct.hpp"

namespace Destruct
{
class DMutableStruct : public DStruct
{
public:
  typedef DObject* (*CreateMutableObjectFunction)(DMutableStruct *, DValue const&);
  typedef std::vector<DAttribute> DAttributeContainer;
  typedef DAttributeContainer::const_iterator DAttributeIterator;

  template<typename Iterator> 
  EXPORT DMutableStruct(DStruct const* base, const DUnicodeString& name, CreateMutableObjectFunction newObj, Iterator attributeBegin, Iterator attributeEnd) : DStruct(base, name, (DObject* (*) (DStruct*, DValue const&))newObj, attributeBegin, attributeEnd), __createObject(newObj)
  {
  }
  EXPORT DMutableStruct(DStruct const* base, const DUnicodeString& name, CreateMutableObjectFunction objectFunction);
  EXPORT ~DMutableStruct();

  EXPORT DObject* newObject(DValue const& args);
  EXPORT void     addAttribute(const DAttribute& attribute);
  EXPORT void     replaceAttribute(size_t idx,const DAttribute& attribute);
private:
  const CreateMutableObjectFunction    __createObject;
};

}

#endif
