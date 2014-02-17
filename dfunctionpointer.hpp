#ifndef _DESTRUCT_DFUNCTIONPOINTER_HPP_
#define _DESTRUCT_DFUNCTIONPOINTER_HPP_

#include "dnullobject.hpp"
#include "dvalue.hpp"
#include "dmethodobject.hpp"
//DFunction pas besoin de pointer
//DFunction/DMember call avec this 
namespace Destruct
{

template <typename CPPClass>
class DFunctionObjectBase
{
public:
  virtual ~DFunctionObjectBase()
  {
  };
  virtual DMethodObject* get(CPPClass* self) = 0;
protected:
  DFunctionObjectBase()
  {
  };

private:
 DFunctionObjectBase(DFunctionObjectBase const&);
 DFunctionObjectBase & operator =(DFunctionObjectBase const &);
};

/*
 * DFunctionObjectTyped specialization :  ReturnType CPPClass(Argument)
*/


template<typename RealReturnType, typename CPPClass, typename ArgumentType>
class DFunctionObjectTyped : public DFunctionObjectBase<CPPClass >
{
public:
  DFunctionObjectTyped(RealReturnType (CPPClass::* member)(ArgumentType)) :  __member(member)
  {
  }

  DMethodObject* get(CPPClass* self)
  {
   return new DMethodObject(self, __member);
  }
private:
  RealReturnType (CPPClass::* __member) (ArgumentType);
};

/*
 * DFunctionObjectTyped specialization : void CPPClass(Argument)
*/

template<typename CPPClass, typename ArgumentType>
class DFunctionObjectTyped<void, CPPClass, ArgumentType> : public DFunctionObjectBase<CPPClass >
{
public:
  DFunctionObjectTyped(void (CPPClass::* member)(ArgumentType)) :  __member(member)
  {
  }

  DMethodObject* get(CPPClass* self)
  {
   return new DMethodObject(self, __member);
  }
private:
  void (CPPClass::* __member) (ArgumentType);
};


/*
 * DFunctionObjectTyped specialization : returnType CPPClass(void)
*/

template<typename RealReturnType, typename CPPClass>
class DFunctionObjectTyped<RealReturnType, CPPClass, void > : public DFunctionObjectBase<CPPClass >
{
public:
  DFunctionObjectTyped(RealReturnType (CPPClass::* member) (void)) : __member(member)
  {
  }

  DMethodObject* get(CPPClass* self)
  {
    return new DMethodObject(self, __member);
  }
private:
  RealReturnType (CPPClass::* __member) (void);
};

/*
 * DFunctionObjectTyped specialization : void CPPClass(void)
*/

template<typename CPPClass>
class DFunctionObjectTyped<void, CPPClass, void > : public DFunctionObjectBase<CPPClass >
{
public:
  DFunctionObjectTyped(void (CPPClass::* member) (void)) : __member(member)
  {
  }

  DMethodObject* get(CPPClass* self)
  {
    return new DMethodObject(self, __member);
  }
private:
  void (CPPClass::* __member) (void);
};



//class DMemberPointer
template<typename RCPPClass>
class DFunctionPointer
//class DFunctionObject : public DFunctionObject
{
public:
  template<typename CPPClass, typename ReturnType, typename ArgumentType>
  explicit DFunctionPointer(ReturnType (CPPClass::* member)(ArgumentType)) : __methodBase(new DFunctionObjectTyped<ReturnType, CPPClass, ArgumentType>(static_cast<ReturnType (CPPClass::*)(ArgumentType) >(member)))
  {
  }

  template<typename CPPClass , typename ReturnType> 
  explicit DFunctionPointer(ReturnType (CPPClass::* member)(void)) : __methodBase(new DFunctionObjectTyped<ReturnType, CPPClass, void>(static_cast<ReturnType (CPPClass::*)(void) >(member)))
  {
  }

  ~DFunctionPointer()
  {
    delete __methodBase; 
  }

  DMethodObject* get(RCPPClass* self) const
  {
    return (__methodBase->get(self));
  }

private:
  DFunctionObjectBase<RCPPClass >*  __methodBase;
};

}
#endif
