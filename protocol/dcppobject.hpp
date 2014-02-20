#ifndef DSTRUCT_CPP_OBJECT_HPP_
#define DSTRUCT_CPP_OBJECT_HPP_

#include "../dunicodestring.hpp"
#include "../dobject.hpp"
#include "../drealvalue.hpp" 

#include "dmemberpointer.hpp"
#include "dmutablestruct.hpp"

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
  DCppObject(DStruct* dstruct) : DObject(dstruct), __members(CppClass::memberBegin()) 
  {
    this->init();
  }

  DCppObject(const DCppObject& copy) : DObject(copy)
  {
    this->init();
  }

  void  init(void)
  {
    for (size_t idx = 0; idx < CppClass::ownAttributeCount(); ++idx)
    {
      this->__members[idx].init(static_cast<CppClass*>(this));
    }
  }

  static DObject* newObject(DStruct * dstruct)
  {
    return (new CppClass(dstruct));
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
    return (v.get<DFunctionObject* >()->call(args));
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

  DMemoryPointer<CppClass >*       __members;
};

}
#endif
