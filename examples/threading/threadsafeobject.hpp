#ifndef _DESTRUCT_THREADSAFEOBJECT_HPP_
#define _DESTRUCT_THREADSAFEOBJECT_HPP_

#include "dobject.hpp"
#include "dsimpleobject.hpp"
#include "dfunction.hpp"
#include "threading.hpp"

namespace Destruct
{

class BaseValue;

//XXX ThreadSafeFunctionObject
//XXX RecursiveThreadSafeObject
//XXX RecursiveThreadSafeFunctionObject

////thread safe functin object ?
//class DebugFunctionObject : public DFunctionObject
//{
//public:
//DebugFunctionObject(DFunctionObject* functionObject); 
//~DebugFunctionObject();
//
//virtual DValue call(void) const;
//virtual DValue call(DValue const& args) const;
//private:
//DFunctionObject*      __functionObject;
//};

class ThreadSafeObject : public DObject 
{
public:
  ThreadSafeObject(DStruct* dstruct, DValue const& args);
  ThreadSafeObject(ThreadSafeObject const&);
  //ThreadSafeObject(DObject*); //untested

  EXPORT static DObject*   newObject(DStruct *, DValue const& args);

  DObject*          clone() const;
  virtual DValue    getValue(size_t index) const;
  void              setValue(size_t idx, DValue const &);
  virtual DValue    call(size_t index, DValue const &);

  virtual DValue    getValue(DUnicodeString const& name) const;
  void              setValue(DUnicodeString  const& name, DValue const &);

  virtual DValue    call(DUnicodeString const& name, DValue const &);
  virtual DValue    call(DUnicodeString const& name);
protected:
  ~ThreadSafeObject();

  BaseValue*        getBaseValue(size_t index);
  BaseValue const*  getBaseValue(size_t index) const;
  DObject*          object(void) const;

  //addRef // delRef a locker ... ? a part si ds des func deja locker ?
private:
  DObject*          __dobject; //RealValue (refcount)
  mutex             __mutex;
};

}

#endif
