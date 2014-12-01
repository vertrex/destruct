#ifndef __DESTRUCT_THREADPOOL__
#define __DESTRUCT_THREADPOOL__

#include <queue>

#include "dvalue.hpp"
#include "dobject.hpp"
#include "dfunction.hpp"
#include "protocol/dcppobject.hpp"

using namespace Destruct;

template<typename ArgumentType, DType::Type_t ArgumentTypeId, typename ResultType, DType::Type_t ResultTypeId >
class Task : public DCppObject<Task< ArgumentType, ArgumentTypeId, ResultType, ResultTypeId > >
{
public:
  Task(DStruct* dstruct, DValue const& args) : DCppObject<Task< ArgumentType, ArgumentTypeId, ResultType, ResultTypeId > >(dstruct, args)
  {
    this->init();
  }

  RealValue<DFunctionObject* >     function;
  RealValue<ArgumentType >         argument;
  RealValue<ResultType >           result;
  /**
   *  DStruct declaration
   */
public:
  static size_t ownAttributeCount()
  {
    return (3);
  }

  static DAttribute* ownAttributeBegin()
  {
    static DAttribute  attributes[] = 
    {
      DAttribute(DType::DMethodType,  "function"),
      DAttribute(ArgumentTypeId,   "argument"),
      DAttribute(ResultTypeId,   "result"),
    };
    return (attributes);
  }

  static DPointer<Task>* memberBegin()
  {
    static DPointer<Task> memberPointer[] = 
    {
      DPointer<Task>(&Task::function),
      DPointer<Task>(&Task::argument),
      DPointer<Task>(&Task::result),
    };
    return (memberPointer);
  }

  static DAttribute* ownAttributeEnd()
  {
    return (ownAttributeBegin() + ownAttributeCount());
  }

  static DPointer<Task>*  memberEnd()
  {
    return (memberBegin() + ownAttributeCount());
  } 
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
  void          addResult(DValue const& args);
  DValue        join(void);

  uint64_t      __itemCount;

  RealValue<DFunctionObject*> _enqueue, _dequeue, _empty, _addResult, _join;
protected:
  ~Queue();
private:
  std::queue<DValue > __queue;
  std::queue<DValue > __result;

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
      DAttribute(DType::DNoneType, "addResult", DType::DUnknownType),
      DAttribute(DType::DObjectType, "join", DType::DNoneType),
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
      DPointer<Queue>(&Queue::_addResult, &Queue::addResult),
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
  bool          addTask(DValue const& task);
  DValue        join(void);
  DValue        map(DValue const& task);
private:
  DUInt8        __threadNumber;
  pthread_t*     __threads;
  DObject*      __taskQueue;
};

#endif
