#ifndef _DESTRUCT_DMETHODOBJECT_HPP_
#define _DESTRUCT_DMETHODOBJECT_HPP_

#include "drealvalue.hpp"
#include "dnullobject.hpp"
#include "dvalue.hpp"
#include "dexception.hpp"

/*
 *  This is an abstract interface class to implement different CPP member oointer function object
 */

namespace Destruct
{

class DMethodObjectBase
{
public:
  virtual DValue call(const DValue&) = 0;
  virtual ~DMethodObjectBase()
  {
  };
protected:
  DMethodObjectBase()
  {
  };
private:
 DMethodObjectBase(DMethodObjectBase const&);
 DMethodObjectBase & operator =(DMethodObjectBase const &);
};

/*
 *  This implement the interface of DMethodObjectBase 
 *  DMethodObjectTyped specialization : ReturnType CPPClass(Argument)
 */

template<typename RealReturnType, typename CPPClass, typename ArgumentType>
class DMethodObjectTyped : public DMethodObjectBase
{
public:
  DMethodObjectTyped(CPPClass* self, RealReturnType (CPPClass::* member) (ArgumentType)) : __self(self), __member(member)
  {
  }
 
  DValue call(const DValue& args)
  {
    return RealValue<RealReturnType>((__self->*__member)(args));
  }
private:
  CPPClass* __self;
  RealReturnType (CPPClass::* __member) (ArgumentType);
};


template<typename CPPClass, typename ArgumentType>
class DMethodObjectTyped<DValue, CPPClass, ArgumentType> : public DMethodObjectBase
{
public:
  DMethodObjectTyped(CPPClass* self, DValue (CPPClass::* member) (ArgumentType)) : __self(self), __member(member)
  {
  }
 
  DValue call(const DValue& args)
  {
    return (__self->*__member)(args);
  }
private:
  CPPClass* __self;
  DValue (CPPClass::* __member) (ArgumentType);
};

/*
 * DMethodObjectTyped specialization : void CPPClass(Argument)
 */

template<typename CPPClass, typename ArgumentType>
class DMethodObjectTyped<void, CPPClass, ArgumentType> : public DMethodObjectBase
{
public:
  DMethodObjectTyped(CPPClass* self, void (CPPClass::* member) (ArgumentType)) : __self(self), __member(member)
  {
  }
 
  DValue call(const DValue& args)
  {
    (__self->*__member)(args);
    return RealValue<DObject*>(DNone);
  }
private:
  CPPClass* __self;
  void (CPPClass::* __member) (ArgumentType);
};

/*
 * DMethodObjectTyped specialization : returnType CPPClass(void)
 */

template<typename RealReturnType, typename CPPClass>
class DMethodObjectTyped<RealReturnType, CPPClass, void > : public DMethodObjectBase
{
public:
  DMethodObjectTyped(CPPClass* self, RealReturnType (CPPClass::* member) (void)) : __self(self), __member(member)
  {
  }
 
  DValue call(const DValue& args)
  {
    if (args.get<DObject*>() != DNone)
      throw DException("Non DNone argument passed to function(void)");
    return RealValue<RealReturnType >((__self->*__member)());
  }
private:
  CPPClass* __self;
  RealReturnType (CPPClass::* __member) (void);
};

template<typename CPPClass>
class DMethodObjectTyped<DValue , CPPClass, void > : public DMethodObjectBase
{
public:
  DMethodObjectTyped(CPPClass* self, DValue (CPPClass::* member) (void)) : __self(self), __member(member)
  {
  }
 
  DValue call(const DValue& args)
  {
    if (args.get<DObject*>() != DNone)
      throw DException("Non DNone argument passed to function(void)");
    return  (__self->*__member)();
  }
private:
  CPPClass* __self;
  DValue (CPPClass::* __member) (void);
};

/*
 * DMethodObjectTyped specialization : void CPPClass(void)
 */

template<typename CPPClass>
class DMethodObjectTyped<void, CPPClass, void > : public DMethodObjectBase
{
public:
  DMethodObjectTyped(CPPClass* self, void (CPPClass::* member) (void)) : __self(self), __member(member)
  {
  }
 
  DValue call(const DValue& args)
  {
     if (args.get<DObject*>() != DNone)
       throw DException("Non DNone argument passed to function(void)");
     (__self->*__member)();
     return RealValue<DObject*>(DNone);
  }
private:
  CPPClass* __self;
  void (CPPClass::* __member) (void);
};

/*
 *  DMethodObject handler 
 */

class DMethodObject : public DFunctionObject
{
public:
  template<typename CPPClass, typename ReturnType, typename ArgumentType>
  explicit DMethodObject(CPPClass* self, ReturnType (CPPClass::* member)(ArgumentType)) : __methodBase(new DMethodObjectTyped<ReturnType, CPPClass, ArgumentType>(static_cast<CPPClass* >(self), static_cast<ReturnType (CPPClass::*)(ArgumentType) >(member)))
  {
  }

  template<typename CPPClass , typename ReturnType> 
  explicit DMethodObject(CPPClass* self, ReturnType (CPPClass::* member)(void)) : __methodBase(new DMethodObjectTyped<ReturnType, CPPClass, void>(static_cast<CPPClass* >(self), static_cast<ReturnType (CPPClass::*)(void) >(member)))
  {
  }

  ~DMethodObject()
  {
    delete __methodBase; 
  }

  DValue call(DValue const& args) const
  {
    return (__methodBase->call(args));
  }

  DValue call(void) const
  {
    return (__methodBase->call(RealValue<DObject*>(DNone)));
  }
private:
  DMethodObjectBase*  __methodBase;
};

}
#endif 
