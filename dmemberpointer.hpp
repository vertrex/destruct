#ifndef DESTRUCT_MEMPTR_H
#define DESTRUCT_MEMPTR_H

#include "dvalue.hpp"
#include "drealvalue.hpp"

namespace Destruct
{

/*  
 *  naming:
 *  BaseType: the class to which a pointer-to-member is applied
 *  TargetType : the result type to which a pointer-to-member points
 *  a base class of TargetType
 */

template <typename CPPClass>
class DMemoryPointerBase
{
public:
  virtual FinalValue& value(CPPClass* obj) const = 0;
  virtual FinalValue const& value(CPPClass const * obj) const = 0;
  virtual ~DMemoryPointerBase() 
  {
  };

protected:
  DMemoryPointerBase() 
  {
  };
private:
  DMemoryPointerBase(DMemoryPointerBase const&);
  DMemoryPointerBase & operator=(DMemoryPointerBase const &);
};

template<typename CPPClass, typename RealReturnType > 
class DMemberPointer : public DMemoryPointerBase<CPPClass>
{
public:
  DMemberPointer( RealReturnType CPPClass::* member )  : __member(member)
  {
  }

  FinalValue& value(CPPClass* obj) const
  {
    return (obj->*__member);
  }

  FinalValue const& value(CPPClass const *obj) const
  {
    return (obj->*__member);
  }

private:
  RealReturnType CPPClass::* __member;
};

template<typename CPPClass>
class DMemoryPointer
{
public:
  template<typename RealCPPClass, typename ReturnType> 
  explicit DMemoryPointer(ReturnType RealCPPClass::* ptr) : __pointerBase(new DMemberPointer<CPPClass, ReturnType>(static_cast<ReturnType CPPClass::*>(ptr)))
  {
  }

   ~DMemoryPointer()
  {
    delete __pointerBase;
  }

  FinalValue&  value(CPPClass* obj) const
  {
    return (this->__pointerBase->value(obj));
  }

//FINAL VALUE A TEMPLTER POUR ACCEDER DIRECTE SANS DECLARATION EN REALVALUE ? possible ? 

  FinalValue const& value(CPPClass const* obj) const
  {
    return (this->__pointerBase->value(obj));
  }

private:
  DMemoryPointerBase<CPPClass >*   __pointerBase;  
};

}
#endif
