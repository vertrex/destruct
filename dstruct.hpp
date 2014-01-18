#ifndef DESTRUCT_DSTRUCT_HPP_
#define DESTRUCT_DSTRUCT_HPP_

#include <vector>
#include <algorithm>
#include <iterator>
#include "dunicodestring.hpp"
#include "dattribute.hpp"

#include "dpolicy.hpp"

namespace Destruct 
{

class DObject;

class DStruct : public RefcountPolicy<DStruct> 
{
public:
  typedef std::vector<DAttribute> DAttributeContainer;
  typedef DAttributeContainer::const_iterator DAttributeIterator;
  typedef DObject* (*CreateObjectFunction)(DStruct *);

  template<typename Iterator> 
  DStruct(DStruct const* baseClass, const DUnicodeString& name, CreateObjectFunction createObjectFunction, Iterator attributeBegin, Iterator attributeEnd) : __ownAttributes(attributeBegin, attributeEnd), __baseClass(baseClass) ,  __name(name),  __createObject(createObjectFunction), __definitionFix(false), __defaultObject(0)
  {
     this->__baseInit();
     this->__effectiveAttributes.insert(this->__effectiveAttributes.end(), this->__ownAttributes.begin(), this->__ownAttributes.end());
  }


  DStruct(DStruct const* base, const DUnicodeString& name, CreateObjectFunction objectFunction);
  virtual ~DStruct();

  virtual void          addAttribute(const DAttribute&);
  DUnicodeString        name() const;

  //DObject*              newObject() const;
  DObject*              newObject(); //for mutable
 
  DObject const*        defaultDObject() const;//not serialized, could be usefull for destruct to set default value (dff argument)
  void                  setDefault(DObject const *);

  DAttribute const&     attribute(size_t index) const;
  size_t                attributeCount() const;
  int32_t               findAttribute(DUnicodeString const& name) const;


  DAttributeIterator    attributeBegin() const;
  DAttributeIterator    attributeEnd() const;
protected:
  DAttributeContainer           __ownAttributes;
  DAttributeContainer           __effectiveAttributes;

private:
  void                          __baseInit();
  DStruct const * const         __baseClass;
  const DUnicodeString          __name;
  const CreateObjectFunction    __createObject;
  mutable bool                  __definitionFix;
  DObject const *               __defaultObject;
};

}

#endif
