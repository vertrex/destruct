#ifndef DESTRUCT_DSTRUCT_HPP_
#define DESTRUCT_DSTRUCT_HPP_

#include <vector>
#include <algorithm>
#include <iterator>

#include "destruct.hpp"
#include "dpolicy.hpp"
#include "dattribute.hpp"

namespace Destruct 
{

class DObject;
class DValue;
class DAttribute;

class DStruct : public RefcountPolicy<DStruct> 
{
public:
  typedef std::vector<DAttribute> DAttributeContainer;
  typedef DAttributeContainer::const_iterator DAttributeIterator;
  typedef DObject* (*CreateObjectFunction)(DStruct *, DValue const& args);

  template<typename Iterator> 
  EXPORT DStruct(DStruct const* baseClass, const DUnicodeString& name, CreateObjectFunction createObjectFunction, Iterator attributeBegin, Iterator attributeEnd) : __ownAttributes(attributeBegin, attributeEnd), __baseClass(baseClass) ,  __name(name),  __createObject(createObjectFunction), __definitionFix(false), __defaultObject(0)
  {
    this->__baseInit();
    this->__effectiveAttributes.insert(this->__effectiveAttributes.end(), this->__ownAttributes.begin(), this->__ownAttributes.end());
  }

  EXPORT DStruct(DStruct const* base, const DUnicodeString& name, CreateObjectFunction objectFunction);

  EXPORT virtual                       ~DStruct();

  EXPORT virtual void                  addAttribute(const DAttribute&);
  EXPORT DUnicodeString                name() const;
  EXPORT DStruct const * const         base() const;

  //DObject*                    newObject() const;
  EXPORT DObject*                      newObject(); 
  EXPORT virtual DObject*              newObject(DValue const& args); //for mutable
 
  EXPORT DObject const*                defaultDObject() const;//not serialized, could be usefull for destruct to set default value 
  EXPORT void                          setDefault(DObject const *);

  EXPORT DAttribute const&             attribute(size_t index) const;
  EXPORT DAttribute const&             attribute(DUnicodeString const& name) const; //virtual ??XXX
  EXPORT size_t                        attributeCount() const;
  EXPORT int32_t                       findAttribute(DUnicodeString const& name) const;

  EXPORT DAttributeIterator            attributeBegin() const;
  EXPORT DAttributeIterator            attributeEnd() const;
protected:
  DAttributeContainer           __ownAttributes;
  DAttributeContainer           __effectiveAttributes;
private:
  EXPORT void                   __baseInit();
  DStruct const * const         __baseClass;
  const DUnicodeString          __name;
  const CreateObjectFunction    __createObject;
  mutable bool                  __definitionFix;
  DObject const *               __defaultObject;
};

}

#endif
