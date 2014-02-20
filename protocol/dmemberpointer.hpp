#ifndef DESTRUCT_MEMPTR_H
#define DESTRUCT_MEMPTR_H

#include "../dvalue.hpp"
#include "../drealvalue.hpp"

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
  virtual void init(CPPClass * obj)
  {
  }

  //virtual void default(CPPClass* obj) access base method if overwriten  
  //{
  // 
  //}

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

  void  init(CPPClass* obj) 
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

/*
 * DMemberFunctionPointer specialization : returnType CPPClass(void)
*/

template<typename CPPClass, typename RealReturnType, typename MReturnType, typename ArgumentType>
class DMemberFunctionPointer : public DMemoryPointerBase<CPPClass>
{
public:
    DMemberFunctionPointer(RealReturnType CPPClass::* member, MReturnType (CPPClass::* methodPtr)(ArgumentType)) : __member(member), __method(methodPtr)
  {
  }

  void init(CPPClass* obj)
  {
    obj->*__member = new DMethodObject(obj, __method);
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
  MReturnType (CPPClass::* __method) (ArgumentType);
};

/*
 * DMemberFunctionPointer specialization : void CPPClass(Argument)
*/

template<typename CPPClass, typename RealReturnType, typename ArgumentType>
class DMemberFunctionPointer<CPPClass, RealReturnType, void, ArgumentType> : public DMemoryPointerBase<CPPClass>
{
public:
    DMemberFunctionPointer(RealReturnType CPPClass::* member, void (CPPClass::* methodPtr)(ArgumentType)) : __member(member), __method(methodPtr)
  {
  }

  void init(CPPClass* obj)
  {
    obj->*__member = new DMethodObject(obj, __method);
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
  void (CPPClass::* __method) (ArgumentType);
};


/*
 * DMemberFunctionPointer specialization : returnType CPPClass(void)
*/

template<typename CPPClass, typename RealReturnType, typename MReturnType>
class DMemberFunctionPointer<CPPClass, RealReturnType, MReturnType, void> : public DMemoryPointerBase<CPPClass>
{
public:
  DMemberFunctionPointer(RealReturnType CPPClass::* member, MReturnType (CPPClass::* methodPtr)(void)) : __member(member), __method(methodPtr)
  {
  }

  void init(CPPClass* obj)
  {
    /*DFunctionObject* = new DMethodObject XXX encore besoin de dobject ? de le new ? a reflechir*/
    obj->*__member = new DMethodObject(obj, __method);
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
  MReturnType (CPPClass::* __method) (void);
};

/*
 * DMemberFunctionPointer specialization : void CPPClass(void)
*/

template<typename CPPClass, typename RealReturnType>
class DMemberFunctionPointer<CPPClass, RealReturnType, void, void> : public DMemoryPointerBase<CPPClass>
{
public:
  DMemberFunctionPointer(RealReturnType CPPClass::* member, void (CPPClass::* methodPtr)(void)) : __member(member), __method(methodPtr)
  {
  }

  void init(CPPClass* obj)
  {
    obj->*__member = new DMethodObject(obj, __method);
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
  void (CPPClass::* __method) (void);
};

template<typename CPPClass>
class DMemoryPointer
{
public:
  template<typename RealCPPClass, typename ReturnType> 
  explicit DMemoryPointer(ReturnType RealCPPClass::* ptr) : __pointerBase(new DMemberPointer<CPPClass, ReturnType>(static_cast<ReturnType CPPClass::*>(ptr)))
  {
  }

  template<typename RealCPPClass, typename ReturnType, typename MReturnType, typename ArgumentType> 
  explicit DMemoryPointer(ReturnType RealCPPClass::* ptr, MReturnType (CPPClass::* method)(ArgumentType)) : __pointerBase(new DMemberFunctionPointer<CPPClass, ReturnType, MReturnType, ArgumentType>(static_cast<ReturnType CPPClass::*>(ptr), static_cast<MReturnType (CPPClass::*)(ArgumentType) >(method)))
  {
  }

  template<typename RealCPPClass, typename ReturnType, typename MReturnType> 
  explicit DMemoryPointer(ReturnType RealCPPClass::* ptr, MReturnType (CPPClass::* method)(void)) : __pointerBase(new DMemberFunctionPointer<CPPClass, ReturnType, MReturnType, void>(static_cast<ReturnType CPPClass::*>(ptr), static_cast<MReturnType (CPPClass::*)(void) >(method)))
  {
  }

  ~DMemoryPointer()
  {
    delete __pointerBase;
  }

  void  init(CPPClass* self)
  {
    return (this->__pointerBase->init(self));
  }

  FinalValue&  value(CPPClass* self) const
  {
    return (this->__pointerBase->value(self));
  }

  FinalValue const& value(CPPClass const* self) const
  {
    return (this->__pointerBase->value(self));
  }

private:
  DMemoryPointerBase<CPPClass >*   __pointerBase;  
};

}
#endif
