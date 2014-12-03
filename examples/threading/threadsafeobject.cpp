#include "threadsafeobject.hpp"

#include "dvalue.hpp"
#include "drealvalue.hpp"
#include "dfunction.hpp"

#include <pthread.h>

namespace Destruct
{

ThreadSafeObject::ThreadSafeObject(DStruct* dstruct, DValue const& args) : DObject(args.get<DObject*>()->instanceOf(), args), __dobject(args.get<DObject*>())
{
  pthread_mutex_init(&this->__mutex, NULL);
}

ThreadSafeObject::ThreadSafeObject(ThreadSafeObject const& copy) : DObject(copy), __dobject(copy.__dobject)
{
  pthread_mutex_init(&this->__mutex, NULL);
}

//ThreadSafeObject::ThreadSafeObject(DObject* object) : DObject(*object), __dobject(object)
//{
  //pthread_mutex_init(&__mutex, NULL);
//}

ThreadSafeObject::~ThreadSafeObject()
{
  pthread_mutex_lock(&this->__mutex);
  this->__dobject->destroy();
  pthread_mutex_unlock(&this->__mutex);
  pthread_mutex_destroy(&this->__mutex);
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
  DValue result = this->__dobject->getValue(index);
  return (result);
}

void             ThreadSafeObject::setValue(size_t index, DValue const& args)
{
  pthread_mutex_lock(&this->__mutex);
  this->__dobject->setValue(index, args);
  pthread_mutex_unlock(&this->__mutex);
}

DValue           ThreadSafeObject::call(size_t index, DValue const& args)
{
  pthread_mutex_lock(&this->__mutex);
  DValue result = this->__dobject->call(index, args);
  pthread_mutex_unlock(&this->__mutex);

  return (result);
}

DValue           ThreadSafeObject::getValue(std::string const& name) const//...
{
  DValue result = this->__dobject->getValue(name);
  return (result);
}

void             ThreadSafeObject::setValue(std::string const& name, DValue const& args)
{
  //pthread_mutex_lock(&this->__mutex);
  this->__dobject->setValue(name, args);
  //pthread_mutex_unlock(&this->__mutex);
}


DValue           ThreadSafeObject::call(std::string const& name, DValue const& args)
{
  pthread_mutex_lock(&this->__mutex);
  DValue result = this->__dobject->call(name, args);
  pthread_mutex_unlock(&this->__mutex);

  return (result);
}

DValue           ThreadSafeObject::call(std::string const& name)
{
  //pthread_mutex_lock(&this->__mutex);
  DValue result = this->__dobject->call(name);
  //pthread_mutex_unlock(&this->__mutex);

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
