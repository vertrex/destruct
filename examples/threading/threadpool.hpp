#ifndef __DESTRUCT_THREADPOOL__
#define __DESTRUCT_THREADPOOL__

#include <queue>

#include "dvalue.hpp"
#include "dobject.hpp"
#include "dfunction.hpp"
#include "protocol/dcppobject.hpp"

using namespace Destruct;

class Task
{
public:
  Task(DFunctionObject* functionObject, DValue const& args);
private:
  DFunctionObject*      __functionObject;
  DValue                __args;
};

class Queue : public DCppObject<Queue> 
{
public: 
  Queue(DStruct* dstruct, DValue const& args);
    pthread_mutex_t m_mutex;
    pthread_cond_t  m_condv;
  void          enqueue(DValue const& args);
  DValue        dequeue(void);
  DValue        empty(void);
  void          deref(void);
  void          join(void);

  uint64_t      __itemCount;

  RealValue<DFunctionObject*> _enqueue, _dequeue, _empty, _deref, _join;
protected:
  ~Queue();
private:
  std::queue<DFunctionObject* > __queue;

  /**
   *  DStruct declaration
   */
public:
  static size_t ownAttributeCount()
  {
    return (5);
  }

  static DAttribute* ownAttributeBegin()
  {
    static DAttribute  attributes[] = 
    {
      DAttribute(DType::DUInt8Type,  "empty",   DType::DNoneType),
      DAttribute(DType::DNoneType,   "enqueue", DType::DObjectType),
      DAttribute(DType::DObjectType, "dequeue", DType::DNoneType),
      DAttribute(DType::DNoneType, "deref", DType::DNoneType),
      DAttribute(DType::DNoneType, "join", DType::DNoneType),
    };
    return (attributes);
  }

  static DPointer<Queue>* memberBegin()
  {
    static DPointer<Queue> memberPointer[] = 
    {
      DPointer<Queue>(&Queue::_empty,   &Queue::empty),
      DPointer<Queue>(&Queue::_enqueue, &Queue::enqueue),
      DPointer<Queue>(&Queue::_dequeue, &Queue::dequeue),
      DPointer<Queue>(&Queue::_deref, &Queue::deref),
      DPointer<Queue>(&Queue::_join, &Queue::join),
    };
    return (memberPointer);
  }

  static DAttribute* ownAttributeEnd()
  {
    return (ownAttributeBegin() + ownAttributeCount());
  }

  static DPointer<Queue >*  memberEnd()
  {
    return (memberBegin() + ownAttributeCount());
  } 
};

class ThreadPool
{
public:
  ThreadPool(DUInt8 threadNumber);
  ~ThreadPool();
  bool          addTask(DValue const& args);
  void          join(void);
private:
  DUInt8        __threadNumber;
  pthread_t*     __threads;
  DObject*      __taskQueue;
};

#endif
