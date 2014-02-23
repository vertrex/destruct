#ifndef DSTRUCT_CLASS_OBJECT_HPP_
#define DSTRUCT_CLASS_OBJECT_HPP_

#include "dunicodestring.hpp"
#include "dobject.hpp"
#include "drealvalue.hpp" 

#include "protocol/dmemberpointer.hpp"

namespace Destruct
{

template< typename CppClass>
class DClassObject : public DObject, public CppClass
{
public:
  DClassObject(DStruct * dstruct, DValue const& args) : DObject(dstruct, args), CppClass()
  {
    this->__members = this->memberBegin();

    for (size_t idx = 0; idx < this->ownAttributeCount(); ++idx)
       this->__members[idx].init(this);
  }
 
  DClassObject(DStruct * dstruct, DValue const& args, CppClass const&  classobject) : DObject(dstruct, args) , CppClass(classobject)
  {
    this->__members = this->memberBegin();

    for (size_t idx = 0; idx < this->ownAttributeCount(); ++idx)
      this->__members[idx].init(this);
  }

  static DObject* newObject(DStruct * dstruct, DValue const& args)
  {
    return (new DClassObject(dstruct, args));
  }

  virtual DObject* clone() const
  {
    return (new DClassObject(*this));
  }

  using DObject::getValue;
  using DObject::setValue;
  using DObject::call;

  virtual void   setValue(size_t idx, DValue const& v)
  {
     BaseValue* p = &(this->__members[idx].value(this));
     p->set(v);
  }

  virtual DValue getValue(size_t idx) const
  {
    return (this->__members[idx].value(this));
  }

  //virtual DValue callBase(size_t idx, DValue const& args) implementer ca ? pour aller la base class si overwriten ? 
  //{
  //DValue v = this->__members[idx].default(this);
  //return (v.get<DFunctionObject*>()->call(args);
  //}

  virtual DValue call(size_t idx, DValue const & args)
  {
    DValue v = this->__members[idx].value(this);
    return (v.get<DFunctionObject* >()->call(args));
  }

protected:
  virtual BaseValue* getBaseValue(size_t idx)
  {
     return (&this->__members[idx].value(this));
  }

  virtual BaseValue const* getBaseValue(size_t idx) const
  {
     return (&this->__members[idx].value(this));
  }
  virtual ~DClassObject()
  {
  };

private:
  DPointer<CppClass >*       __members;
};

template <typename CppClass>
inline 
DStruct  makeDClass(DStruct * base, DUnicodeString const& name)
{
  return DStruct(base, name, DClassObject<CppClass>::newObject, CppClass::ownAttributeBegin(), CppClass::ownAttributeEnd());
}

template <typename CppClass>
inline 
DStruct*  makeNewDClass(DStruct * base, DUnicodeString const& name)
{
  return (new DStruct(base, name, DClassObject<CppClass>::newObject, CppClass::ownAttributeBegin(), CppClass::ownAttributeEnd()));
}

template <typename CppClass>
inline 
DStruct*  makeNewDClass(DUnicodeString const& name)
{
  return (new DStruct(NULL, name, DClassObject<CppClass>::newObject, CppClass::ownAttributeBegin(), CppClass::ownAttributeEnd()));
}

template <typename CppClass>
inline 
DClassObject<CppClass> makeDObject(DStruct * cl, CppClass const& obj)
{
  return (DClassObject<CppClass>(cl, obj));
}

template <typename CppClass>
inline 
DClassObject<CppClass>* makeNewDObject(DStruct * cl, CppClass const& obj)
{
  return (new DClassObject<CppClass>(cl, obj));
}

}
#endif
