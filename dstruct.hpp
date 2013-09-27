#ifndef DESTRUCT_DSTRUCT_HPP_
#define DESTRUCT_DSTRUCT_HPP_

#include <vector>
#include <algorithm>
#include <iterator>
#include "dunicodestring.hpp"
#include "dattribute.hpp"

namespace Destruct 
{

class DObject;

class DStruct
{
public:
  typedef std::vector<DAttribute> DAttributeContainer;
  typedef DAttributeContainer::const_iterator DAttributeIterator;
  typedef DObject* (*CreateObjectFunction)(DStruct const *);

  template<typename Iterator> 
  DStruct(DStruct const* baseClass, const DUnicodeString& name, CreateObjectFunction createObjectFunction, Iterator attributeBegin, Iterator attributeEnd) : __baseClass(baseClass) ,  __name(name), __ownAttributes(attributeBegin, attributeEnd), __createObject(createObjectFunction), __definitionFix(false), __defaultObject(0)
  {
     this->__baseInit();
     this->__effectiveAttributes.insert(this->__effectiveAttributes.end(), this->__ownAttributes.begin(), this->__ownAttributes.end());
  }


  DStruct(DStruct const* base, const DUnicodeString& name, CreateObjectFunction objectFunction);
  //~DStruct();

  DUnicodeString        name() const;

  DObject*              newObject() const;
 
  DObject const*        defaultDObject() const;//not serialized, could be usefull for destruct to set default value (dff argument)
  void                  setDefault(DObject const *);

  DAttribute const&     attribute(size_t index) const;
  size_t                attributeCount() const;
  void                  addAttribute(const DAttribute&);
  int32_t               findAttribute(DUnicodeString const& name) const;


  DAttributeIterator    attributeBegin() const;
  DAttributeIterator    attributeEnd() const;
private:
  void                          __baseInit();
  DStruct const * const         __baseClass;
  const DUnicodeString          __name;
  DAttributeContainer           __ownAttributes;
  DAttributeContainer           __effectiveAttributes;
  const CreateObjectFunction    __createObject;
  mutable bool                  __definitionFix;
  DObject const *               __defaultObject;
};

}

#endif
