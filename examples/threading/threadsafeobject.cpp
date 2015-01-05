#include "threadsafeobject.hpp"

#include "dvalue.hpp"
#include "drealvalue.hpp"
#include "dfunction.hpp"

#include <pthread.h>

namespace Destruct
{

ThreadSafeObject::ThreadSafeObject(DStruct* dstruct, DValue const& args) : DObject(args.get<DObject*>()->instanceOf(), args), __dobject(args.get<DObject*>())
{
  //std::cout << "create mutex object" << std::endl;
  pthread_mutex_init(&this->__mutex, NULL);
}

ThreadSafeObject::ThreadSafeObject(ThreadSafeObject const& copy) : DObject(copy), __dobject(copy.__dobject)
{
  //std::cout << "copy threadsafe object" << std::endl;
  pthread_mutex_init(&this->__mutex, NULL);
}

//ThreadSafeObject::ThreadSafeObject(DObject* object) : DObject(*object), __dobject(object)
//{
  //pthread_mutex_init(&__mutex, NULL);
//}

ThreadSafeObject::~ThreadSafeObject()
{
  //std::cout << "ThreadSafeObject destroyed " << std::endl;
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
  //std::cout << "Lock call index " << std::endl;
  pthread_mutex_lock(&this->__mutex);
  //std::cout << "call " << std::endl;
  DValue result = this->__dobject->call(index, args);
  //std::cout << "Unlock call " << std::endl;
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
  //std::cout << "Lock call name " << std::endl;
  pthread_mutex_lock(&this->__mutex);
  //std::cout << "call thread safe name " << std::endl;
  DValue result = this->__dobject->call(name, args);
  //std::cout << "Unlock call name " << std::endl;
  pthread_mutex_unlock(&this->__mutex);

  return (result);
}

DValue           ThreadSafeObject::call(std::string const& name)
{
  //if (name != "dequeue")
  pthread_mutex_lock(&this->__mutex);
//  DValue result = this->__dobject->call(name);
  DValue func = this->__dobject->getValue(name);
  DFunctionObject* funcObj = func.get<DFunctionObject*>();

  pthread_mutex_unlock(&this->__mutex);
  DValue result = funcObj->call(); //because of "Dequeue" call who lock himself ...

  //if (name != "dequeue")
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
