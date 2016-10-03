#include "threadsafeobject.hpp"

#include "dvalue.hpp"
#include "drealvalue.hpp"
#include "dfunction.hpp"



namespace Destruct
{

ThreadSafeObject::ThreadSafeObject(DStruct* dstruct, DValue const& args) : DObject(args.get<DObject*>()->instanceOf(), args), __dobject(args.get<DObject*>())
{
  //std::cout << "create mutex object" << std::endl;
  mutex_init(&this->__mutex);
}

ThreadSafeObject::ThreadSafeObject(ThreadSafeObject const& copy) : DObject(copy), __dobject(copy.__dobject)
{
  //use same mutex than ref object and refcount ?
  mutex_init(&this->__mutex);
}

//ThreadSafeObject::ThreadSafeObject(DObject* object) : DObject(*object), __dobject(object)
//{
  //pthread_mutex_init(&__mutex, NULL);
//}

ThreadSafeObject::~ThreadSafeObject()
{
  //XXX use same mutex and refcount ?
  mutex_lock(&this->__mutex);
  this->__dobject->destroy();
  mutex_unlock(&this->__mutex);
  mutex_destroy(&this->__mutex);
}

DObject*         ThreadSafeObject::newObject(DStruct* dstruct, DValue const& args)
{
  return (new ThreadSafeObject(dstruct, args));
}

DObject*         ThreadSafeObject::clone() const //no lock on const
{
  return (this->__dobject->clone());
}

DValue           ThreadSafeObject::getValue(size_t index) const //no lock on const
{
  mutex_lock((mutex*)&(this->__mutex));
  DValue result = this->__dobject->getValue(index);
  mutex_unlock((mutex*)&this->__mutex);
  return (result);
}

void             ThreadSafeObject::setValue(size_t index, DValue const& args)
{
  mutex_lock(&this->__mutex);
  this->__dobject->setValue(index, args);
  mutex_unlock(&this->__mutex);
}

DValue           ThreadSafeObject::call(size_t index, DValue const& args)
{
  mutex_lock(&this->__mutex);
  DValue result = this->__dobject->call(index, args);
  mutex_unlock(&this->__mutex);

  return (result);
}

DValue           ThreadSafeObject::getValue(DUnicodeString const& name) const//...
{
  mutex_lock((mutex*)&(this->__mutex));
  DValue result = this->__dobject->getValue(name);
  mutex_unlock((mutex*)&this->__mutex);
  return (result);
}

void             ThreadSafeObject::setValue(DUnicodeString const& name, DValue const& args)
{
  mutex_lock(&this->__mutex);
  this->__dobject->setValue(name, args);
  mutex_unlock(&this->__mutex);
}


DValue           ThreadSafeObject::call(DUnicodeString const& name, DValue const& args)
{
  mutex_lock(&this->__mutex);
  DValue result = this->__dobject->call(name, args);
  mutex_unlock(&this->__mutex);

  return (result);
}

DValue           ThreadSafeObject::call(DUnicodeString const& name)
{
  mutex_lock(&this->__mutex);
  DValue result = this->__dobject->call(name);
  //DValue func = this->__dobject->getValue(name);
  //DFunctionObject* funcObj = func.get<DFunctionObject*>();
  //
  //DValue result = funcObj->call();
  mutex_unlock(&this->__mutex);

  return (result);
}

BaseValue*       ThreadSafeObject::getBaseValue(size_t index) 
{
  return (NULL);
}

BaseValue const* ThreadSafeObject::getBaseValue(size_t index) const 
{
  return (NULL);
}

DObject*         ThreadSafeObject::object(void) const
{
  return (this->__dobject);
}

}
