#ifndef DSTRUCT_CPP_OBJECT_HPP_
#define DSTRUCT_CPP_OBJECT_HPP_

#include "dunicodestring.hpp"
#include "dmemberpointer.hpp"
#include "dobject.hpp"

namespace Destruct
{

template <typename OriginalClass>
class DCppObject : public DObject
{
public:
  typedef MemberPointer<OriginalClass, FinalValue> MemPtr;
  DCppObject(DStruct const * myClass) : DObject(myClass), __myObject(), __members(OriginalClass::memberBegin())
  {
  }
  //this is a copy constructor so original dobject whould not be modified, must use getOriginal to modify it !

  DCppObject(DStruct const * myClass, OriginalClass const & object) : DObject(myClass), __myObject(object), __members(OriginalClass::memberBegin())
  {
  }

  static DObject * newObject(DStruct const * myClass)
  {
    return (new DCppObject(myClass));
  }

  virtual DObject * clone() const
  {
    return (new DCppObject(*this));
  }

  using DObject::getValue;
  using DObject::setValue;

  virtual DValue getValue(size_t idx) const
  {
    return (this->__members[idx].value(this->__myObject));
  }

  virtual void setValue(size_t idx, DValue const & v)
  {
    BaseValue* p = &(this->__members[idx].value(this->__myObject));
    p->set(v);
  }

  OriginalClass const&  getOriginal()
  {
    return this->__myObject;
  }

protected:
  virtual BaseValue * getBaseValue(size_t idx)
  {
    return (&this->__members[idx].value(this->__myObject));
  }

  virtual BaseValue const * getBaseValue(size_t idx) const
  {
    return (&this->__members[idx].value(this->__myObject));
  }

private:
  OriginalClass __myObject;
  MemPtr*       __members;
};



template <typename CppClass>
inline 
DStruct  makeClass(DStruct const* base, DUnicodeString const& name)
{
  return DStruct(base, name, DCppObject<CppClass>::newObject, CppClass::ownAttributeBegin(), CppClass::ownAttributeEnd());
}

template <typename CppClass>
inline 
DCppObject<CppClass> makeObject(DStruct const* cl, CppClass const& obj)
{
  return (DCppObject<CppClass>(cl, obj));
}

}
#endif
