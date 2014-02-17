#ifndef DSTRUCT_CPP_OBJECT_HPP_
#define DSTRUCT_CPP_OBJECT_HPP_

#include "dunicodestring.hpp"
#include "dmemberpointer.hpp"
#include "dobject.hpp"
#include "drealvalue.hpp" 

namespace Destruct
{
 /*
  *  This object protocol is used to bind CPP object to MOP
  *  This is a multi-inheritance adaptor pattern wich get object trough const reference (copy the passe object)
  *  (Take care of memory leak :) 
  *  This let you acces object method through the MOP 
  *
  */

template< typename CppClass>
class DClassObject : public DObject, public CppClass
{
public:
  DClassObject(DStruct * dstruct) : DObject(dstruct), CppClass()
  {
    this->__members = this->memberBegin();

    DMemoryPointer<CppClass >* methods = this->memberBegin(); //bouger ailleurs genre ds makeNew ?
    for (size_t idx = 0; idx < this->ownAttributeCount(); ++idx)
    {
       methods[idx].initMember(this); //XXX seem pas mal
    }
  }
 
//get original cpp object to wrap by ref  as const so original object is not modified !!! XXX not modified !
//DClassObject(DStruct const * dstruct, CppClass const&  classobject) : DObject(dstruct) , CppClass(classobject)
  DClassObject(DStruct * dstruct, CppClass const&  classobject) : DObject(dstruct) , CppClass(classobject)
  {
    this->__members = this->memberBegin();

    DMemoryPointer<CppClass >* methods = this->memberBegin();
    for (size_t idx = 0; idx < this->ownAttributeCount(); ++idx)
    {
      methods[idx].initMember(this);
    }
  }

  static DObject* newObject(DStruct * dstruct)
  {
    return (new DClassObject(dstruct));
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
  DMemoryPointer<CppClass >*       __members;
};

template <typename CppClass>
inline 
//DStruct  makeDClass(DStruct const* base, DUnicodeString const& name)
DStruct  makeDClass(DStruct * base, DUnicodeString const& name)
{
  return DStruct(base, name, DClassObject<CppClass>::newObject, CppClass::ownAttributeBegin(), CppClass::ownAttributeEnd());
}

template <typename CppClass>
inline 
//DStruct*  makeNewDClass(DStruct const* base, DUnicodeString const& name)
DStruct*  makeNewDClass(DStruct * base, DUnicodeString const& name)
{
  return (new DStruct(base, name, DClassObject<CppClass>::newObject, CppClass::ownAttributeBegin(), CppClass::ownAttributeEnd()));
}

template <typename CppClass>
inline 
//DClassObject<CppClass> makeDObject(DStruct const* cl, CppClass const& obj)
DClassObject<CppClass> makeDObject(DStruct * cl, CppClass const& obj)
{
  return (DClassObject<CppClass>(cl, obj));
}

template <typename CppClass>
inline 
//DClassObject<CppClass>* makeNewDObject(DStruct const* cl, CppClass const& obj)
DClassObject<CppClass>* makeNewDObject(DStruct * cl, CppClass const& obj)
{
  return (new DClassObject<CppClass>(cl, obj));
}

}
#endif
