#ifndef DSTRUCT_CPP_OBJECT_HPP_
#define DSTRUCT_CPP_OBJECT_HPP_

#include "protocol/dcpppointer.hpp"

namespace Destruct
{

/*
 *  This object protocol is used to bind CPP object to MOP
 *  This is a multi-inheritance adaptor pattern wich get object trough const reference (copy the passe object)
 *  (Take care of memory leak :) 
 *  This let you acces object method through the MOP 
 *  
 */
template<typename CppClass>
class DCppObject : public DObject
{
public:
  //XXX new dstruct will be created each time and not destroyed !
  //DCppObject(const std::string name) : DObject(&DStruct(NULL, name, DCppObject::newObject, CppClass::ownAttributeBegin(), CppClass::ownAttributeEnd()), RealValue<DObject*>(DNone)), __members(CppClass::memberBegin()) //auto-registering class
  //{
  //}

   ////new dstruct will be created each time and not destroyed ! 
  //DCppObject(const std::string name, DValue const& args) : DObject(&DStruct(NULL, name, DCppObject::newObject, CppClass::ownAttributeBegin(), CppClass::ownAttributeEnd()), args), __members(CppClass::memberBegin()) //auto-registering class
  //{
  //}

  DCppObject(DStruct* dstruct) : DObject(dstruct, RealValue<DObject*>(DNone)), __members(CppClass::memberBegin())
  {
  }

  DCppObject(DStruct* dstruct, DValue const& args) : DObject(dstruct, args), __members(CppClass::memberBegin()) 
  {
  }

  DCppObject(const DCppObject& copy) : DObject(copy)
  {
  }

  void  init(void) //Must be cal by inherited object constructor
  {
    for (size_t idx = 0; idx < CppClass::ownAttributeCount(); ++idx)
    {
      this->__members[idx].init(static_cast<CppClass*>(this));
    }
  }

  static DObject* newObject(DStruct * dstruct, DValue const& args)
  {
    return (new CppClass(dstruct, args));
  }

  virtual DObject* clone() const
  {
    return (new CppClass(*static_cast<const CppClass *>(this)));
  }

  using DObject::getValue;
  using DObject::setValue;
  using DObject::call;

  virtual void   setValue(size_t idx, DValue const& v)
  {
    BaseValue* p = &(this->__members[idx].value(static_cast<CppClass *>(this)));
    p->set(v);
  }

  virtual DValue getValue(size_t idx) const
  {
    return (this->__members[idx].value(static_cast<const CppClass* >(this)));
  }

  virtual DValue call(size_t idx, DValue const & args)
  {
    DValue v = this->__members[idx].value(static_cast<CppClass* >(this));
    DFunctionObject* fo = v.get<DFunctionObject*>();
    DValue res = fo->call(args);
    fo->destroy();
    return (res);
  }
protected:
  virtual BaseValue* getBaseValue(size_t idx)
  {
    return (&this->__members[idx].value(static_cast<CppClass *>(this)));
  }

  virtual BaseValue const* getBaseValue(size_t idx) const
  {
    return (&this->__members[idx].value(static_cast<const CppClass *>(this)));
  }

  virtual ~DCppObject()
  {
  };

  DPointer<CppClass>*       __members;
};


template <typename CppClass>
inline 
DStruct*  makeNewDCpp(DUnicodeString const& name)
{
  return (new DStruct(NULL, name, DCppObject<CppClass>::newObject, CppClass::ownAttributeBegin(), CppClass::ownAttributeEnd()));
}

}
#endif
