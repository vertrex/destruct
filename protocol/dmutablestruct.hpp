#ifndef DESTRUCT_DMUTABLE_STRUCT_HPP_
#define DESTRUCT_DMUTABLE_STRUCT_HPP_

#include "dstruct.hpp"

namespace Destruct
{
        //cclass DValue;

class DMutableStruct : public DStruct
{
public:
  typedef DObject* (*CreateMutableObjectFunction)(DMutableStruct *, DValue const&);
  typedef std::vector<DAttribute> DAttributeContainer;
  typedef DAttributeContainer::const_iterator DAttributeIterator;

  template<typename Iterator> 
  DMutableStruct(DStruct const* base, const DUnicodeString& name, CreateMutableObjectFunction newObj, Iterator attributeBegin, Iterator attributeEnd) : DStruct(base, name, (DObject* (*) (DStruct*, DValue const&))newObj, attributeBegin, attributeEnd), __createObject(newObj)
  {
  }
  DMutableStruct(DStruct const* base, const DUnicodeString& name, CreateMutableObjectFunction objectFunction);

  DObject* newObject(DValue const& args);
  void     addAttribute(const DAttribute& attribute);
  void     replaceAttribute(size_t idx,const DAttribute& attribute);
private:
  const CreateMutableObjectFunction    __createObject;
};

}

#endif
