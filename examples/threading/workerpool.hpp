#ifndef __DESTRUCT_THREADPOOL__
#define __DESTRUCT_THREADPOOL__

#include <queue>

#include "destruct.hpp"
#include "dvalue.hpp"
#include "dobject.hpp"
#include "dfunction.hpp"
#include "protocol/dcppobject.hpp"
#include "threadsafeobject.hpp"

using namespace Destruct;

/**
 * Task
 */
template<typename ArgumentType, DType::Type_t ArgumentTypeId, typename ResultType, DType::Type_t ResultTypeId >
class Task : public DCppObject<Task< ArgumentType, ArgumentTypeId, ResultType, ResultTypeId > >
{
public:
  Task(DStruct* dstruct, DValue const& args) : DCppObject<Task< ArgumentType, ArgumentTypeId, ResultType, ResultTypeId > >(dstruct, args)
  {
    this->init();
  }

  Task(const Task& copy) : DCppObject<Task<ArgumentType, ArgumentTypeId, ResultType, ResultTypeId > >(copy), function(copy.function), argument(copy.argument), result(copy.result)
  {
    this->init(); 
  }

  RealValue<DFunctionObject* >     function;
  RealValue<ArgumentType >         argument;
  RealValue<ResultType >           result;

public:
  /**
   *  DStruct declaration
   */
  static size_t ownAttributeCount()
  {
    return (3);
  }

  static DAttribute* ownAttributeBegin()
  {
    static DAttribute  attributes[] = 
    {
      DAttribute(DType::DMethodType,  "function"),
      DAttribute(ArgumentTypeId,      "argument"),
      DAttribute(ResultTypeId,        "result"),
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

/**
 * Queue 
 */
class Queue : public DCppObject<Queue> 
{
public: 
  Queue(DStruct* dstruct, DValue const& args);

  void                        enqueue(DValue const& args);
  DValue                      dequeue(void);
  DValue                      empty(void);
  void                        addResult(DValue const& args);
  DValue                      join(void);

protected:
  ~Queue();
private:
  uint64_t                    __itemCount; //semaphore or another mutexed queue
  std::queue<DValue>          __queue;
  std::queue<DValue>          __result;
  
  mutex_def(__mutex);
  mutex_def(__joinMutex);
  cond_def(__enqueueSignal);
  cond_def(__itemCountSignal);
  RealValue<DFunctionObject*> _enqueue, _dequeue, _empty, _addResult, _join;


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
      DAttribute(DType::DNoneType, "addResult", DType::DUnknownType), //python looose
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

/* 
 * WorkerPool
 */
class WorkerPool : public DCppObject<WorkerPool> 
{
public:
  static void Declare(void)
  {
    
    DStruct* workerPool = makeNewDCpp<WorkerPool>("WorkerPool");
    Destruct::DStructs::instance().registerDStruct(workerPool);
    DStruct* task = makeNewDCpp<Task<DUInt64, DType::DUInt64Type, DUInt64, DType::DUInt64Type> >("Task"); //XXX mus derivate to other possible type 
    Destruct::DStructs::instance().registerDStruct(task);
  
    DStruct* taskObject = makeNewDCpp<Task<DObject*, DType::DObjectType, DUInt64, DType::DUInt64Type > >("TaskObject");//mutable etc... ?
    Destruct::DStructs::instance().registerDStruct(taskObject);
  }

  EXPORT WorkerPool(DStruct* dstruct, DValue const& args);
  ~WorkerPool();

  bool                          addTask(DValue const& task);
  DValue                        join(void);
  DValue                        map(DValue const& task);
private:
  DUInt8                        __threadNumber;
#ifdef WIN32
  PHANDLE						__threads;
#else
  pthread_t*                    __threads;
#endif
  DObject*                      __taskQueue;
  RealValue<DFunctionObject*>   _addTask, _join, _map;

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
      DAttribute(DType::DNoneType,  "addTask",   DType::DObjectType),
      DAttribute(DType::DObjectType, "join", DType::DNoneType),
      DAttribute(DType::DObjectType, "map", DType::DObjectType),
    };
    return (attributes);
  }

  static DPointer<WorkerPool>* memberBegin()
  {
    static DPointer<WorkerPool> memberPointer[] = 
    {
      DPointer<WorkerPool>(&WorkerPool::_addTask, &WorkerPool::addTask),
      DPointer<WorkerPool>(&WorkerPool::_join, &WorkerPool::join),
      DPointer<WorkerPool>(&WorkerPool::_map, &WorkerPool::map),
    };
    return (memberPointer);
  }

  static DAttribute* ownAttributeEnd()
  {
    return (ownAttributeBegin() + ownAttributeCount());
  }

  static DPointer<WorkerPool>*  memberEnd()
  {
    return (memberBegin() + ownAttributeCount());
  }
};

#endif
