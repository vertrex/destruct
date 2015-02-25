#ifndef _DESTRUCT_DOBJECT_HPP_
#define _DESTRUCT_DOBJECT_HPP_

#include <memory>
#include "dpolicy.hpp"
#include "dstruct.hpp"
#include "dunicodestring.hpp"

namespace Destruct
{
class DValue;
class BaseValue;

class DObject : public RefcountPolicy<DObject>
{
public:
  friend class RefcountPolicy<DObject>;
 
  EXPORT DObject(DStruct* dstructDef, DValue const& args); //static dstructDef ? 

  EXPORT DStruct* instanceOf(void) const;
  EXPORT DStruct const * const base(void) const;
  EXPORT DObject* baseObject(void) const;

  EXPORT virtual DObject* clone(void) const = 0;
  EXPORT virtual DValue getValue(size_t index) const = 0;
  EXPORT virtual void setValue(size_t idx, DValue const &) = 0;
  EXPORT virtual DValue call(size_t index, DValue const &)  = 0;

  EXPORT virtual DValue getValue(DUnicodeString const& name) const;
  EXPORT virtual void setValue(DUnicodeString const& name, DValue const &);

  EXPORT virtual DValue call(DUnicodeString const& name, DValue const &);
  EXPORT virtual DValue call(DUnicodeString const& name);

  EXPORT virtual BaseValue* getBaseValue(size_t index) = 0;
  EXPORT virtual BaseValue const* getBaseValue(size_t index) const = 0;
  EXPORT static  BaseValue* getBaseValue(DObject*, size_t index);
  EXPORT static  BaseValue const* getBaseValue(DObject const*, size_t idx);
protected:
  DObject(const DObject& rhs);
 
  //virtual BaseValue* getBaseValue(size_t index) = 0;
  //virtual BaseValue const* getBaseValue(size_t index) const = 0;
  //static  BaseValue* getBaseValue(DObject*, size_t index);
  //static  BaseValue const* getBaseValue(DObject const*, size_t idx);
  virtual ~DObject()
  {
  }
private:
  DStruct* const __dstruct;
  DObject*       __baseObject;
};

}

#endif
