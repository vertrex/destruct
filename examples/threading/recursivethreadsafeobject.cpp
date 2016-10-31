#include "threadsafeobject.hpp"
#include "recursivethreadsafeobject.hpp"

#include "dvalue.hpp"
#include "drealvalue.hpp"
#include "dfunction.hpp"
#include "dobject.hpp"


namespace Destruct
{

///XXX handle exception (unlock and throw)

RecursiveThreadSafeObject::RecursiveThreadSafeObject(DStruct* dstruct, DValue const& args) : DObject(args.get<DObject*>()->instanceOf(), args), __dobject(args.get<DObject*>())
{
  this->__mutex = new mutex;
  memset(this->__mutex, 0, sizeof(mutex));
  mutex_init(this->__mutex);
//pthread_mutexattr_t Attr;

//pthread_mutexattr_init(&Attr);
//pthread_mutexattr_settype(&Attr, PTHREAD_MUTEX_RECURSIVE);
//pthread_mutex_init(this->__mutex, &Attr);
}

RecursiveThreadSafeObject::RecursiveThreadSafeObject(RecursiveThreadSafeObject const& copy) : DObject(copy), __dobject(copy.__dobject), __mutex(copy.__mutex)
{
}

RecursiveThreadSafeObject::RecursiveThreadSafeObject(DObject* dobject, mutex* mtx) : DObject(*dobject), __dobject(dobject), __mutex(mtx)
{
}

RecursiveThreadSafeObject::~RecursiveThreadSafeObject()
{
  //XXX use same mutex and refcount ?
  //mutex_lock(this->__mutex);
  ////this->__dobject->destroy();
  //mutex_unlock(this->__mutex);
  //mutex_destroy(this->__mutex);
}

DObject*         RecursiveThreadSafeObject::newObject(DStruct* dstruct, DValue const& args)
{
  return (new RecursiveThreadSafeObject(dstruct, args));
}

DObject*         RecursiveThreadSafeObject::clone() const
{
  return (this->__dobject->clone());
}

DValue           RecursiveThreadSafeObject::getValue(size_t index) const
{
  mutex_lock((mutex*)this->__mutex);
  DType::Type_t  attributeType = this->object()->instanceOf()->attribute(index).type().getType();
  if (attributeType == DType::DObjectType)
  {
    DValue result = this->__dobject->getValue(index);
    mutex_unlock((mutex*)this->__mutex);
 
    return (RealValue<DObject*>(new RecursiveThreadSafeObject(result.get<DObject*>(), this->__mutex)));
  }
 //else if (attributeType == DType::DMethodType)
 //return (RealValue<DFunctionObject*>(new RecursiveThreadSafeObject(result.get<DFunctionObject*>())));

  DValue result = this->__dobject->getValue(index);
  mutex_unlock((mutex*)this->__mutex);
  return (result);
}

DValue           RecursiveThreadSafeObject::getValue(DUnicodeString const& name) const//...
{
  mutex_lock((mutex*)this->__mutex);
  DType::Type_t  attributeType = this->object()->instanceOf()->attribute(name).type().getType();
  if (attributeType == DType::DObjectType)
  {
    DValue result = this->__dobject->getValue(name);
    mutex_unlock((mutex*)this->__mutex);
 
    return (RealValue<DObject*>(new RecursiveThreadSafeObject(result.get<DObject*>(), this->__mutex)));
  }
  //else if (attributeType == DType::DMethodType)
  //return (RealValue<DFunctionObject*>(new RecursiveTraceFunctionObject(result.get<DFunctionObject*>())));
  DValue result = this->__dobject->getValue(name);
  mutex_unlock((mutex*)this->__mutex);
  return (result);
}

void             RecursiveThreadSafeObject::setValue(size_t index, DValue const& args)
{
  mutex_lock(this->__mutex);
  this->__dobject->setValue(index, args);
  mutex_unlock(this->__mutex);
}

void             RecursiveThreadSafeObject::setValue(DUnicodeString const& name, DValue const& args)
{
  mutex_lock(this->__mutex);
  this->__dobject->setValue(name, args);
  mutex_unlock(this->__mutex);
}

DValue           RecursiveThreadSafeObject::call(size_t index, DValue const& args)
{
  mutex_lock(this->__mutex);
  if (this->object()->instanceOf()->attribute(index).type().getReturnType() == DType::DObjectType)
  {
  // try //catch throw // ??
    DValue result;
    try {
    result = this->__dobject->call(index, args); //if args is recmutex it will freeze or use multilockable object
    }
    catch (DException const& exception)
    {
      std::cout << "thread safe exception " << std::endl;
      mutex_unlock(this->__mutex);
      throw exception;
    }
    mutex_unlock(this->__mutex);
    return (RealValue<DObject*>(new RecursiveThreadSafeObject(result.get<DObject*>(), this->__mutex)));
  }

  DValue result = this->__dobject->call(index, args);
  mutex_unlock(this->__mutex);

  return (result);
}

DValue           RecursiveThreadSafeObject::call(DUnicodeString const& name, DValue const& args)
{
  mutex_lock(this->__mutex);
  if (this->object()->instanceOf()->attribute(name).type().getReturnType() == DType::DObjectType)
  {
    DValue result;
    try {
    result = this->__dobject->call(name, args);


  }
    catch (DException const& exception)
    {
      std::cout << "thread safe exception " << std::endl;
      mutex_unlock(this->__mutex);
      throw exception;
    }


    mutex_unlock(this->__mutex);
    return (RealValue<DObject*>(new RecursiveThreadSafeObject(result.get<DObject*>(), this->__mutex)));
  }
//try catch too ...
  try {
  DValue result = this->__dobject->call(name, args);
  mutex_unlock(this->__mutex);
  return (result);
  }
  catch (DException const& exception)
  {
  mutex_unlock(this->__mutex);
    throw exception;
  }
}

DValue           RecursiveThreadSafeObject::call(DUnicodeString const& name)
{
  mutex_lock(this->__mutex);
  if (this->object()->instanceOf()->attribute(name).type().getReturnType() == DType::DObjectType)
  {
    DValue result;
        try{
    result = this->__dobject->call(name);
        }
    catch (DException const& exception)
    {
      std::cout << "thread safe exception " << std::endl;
      mutex_unlock(this->__mutex);
      throw exception;
    }

    mutex_unlock(this->__mutex);
    return (RealValue<DObject*>(new RecursiveThreadSafeObject(result.get<DObject*>(), this->__mutex)));
  }
  try {
  DValue result = this->__dobject->call(name);
  mutex_unlock(this->__mutex);
  return (result);
  }
 catch (DException const& exception)
 {

      mutex_unlock(this->__mutex);
      throw exception;
}
}

BaseValue*       RecursiveThreadSafeObject::getBaseValue(size_t index) 
{
  return (NULL);
}

BaseValue const* RecursiveThreadSafeObject::getBaseValue(size_t index) const 
{
  return (NULL);
}

DObject*         RecursiveThreadSafeObject::object(void) const
{
  return (this->__dobject);
}

}
