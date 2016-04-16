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

  Task(const Task& rhs) : DCppObject<Task<ArgumentType, ArgumentTypeId, ResultType, ResultTypeId > >(rhs), function(rhs.function), argument(rhs.argument), result(rhs.result)
  {
    this->copy(this, rhs); 
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
  
  mutex                       __mutex;
  mutex                       __joinMutex;
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
    DStructs& dstructs = Destruct::DStructs::instance();

    DStruct* queue = makeNewDCpp<Queue>("Queue");
    dstructs.registerDStruct(queue);

    DStruct* workerPool = makeNewDCpp<WorkerPool>("WorkerPool");
    dstructs.registerDStruct(workerPool);
    DStruct* task = makeNewDCpp<Task<DUInt64, DType::DUInt64Type, DUInt64, DType::DUInt64Type> >("Task"); //XXX mus derivate to other possible type 
    dstructs.registerDStruct(task);
  
    DStruct* taskObject = makeNewDCpp<Task<DObject*, DType::DObjectType, DUInt64, DType::DUInt64Type > >("TaskObject");//mutable etc... ?
    dstructs.registerDStruct(taskObject);

    DStruct* threadSafeObject = new DStruct(NULL, "ThreadSafeObject", ThreadSafeObject::newObject);
    dstructs.registerDStruct(threadSafeObject);
  }

  EXPORT WorkerPool(DStruct* dstruct, DValue const& args);

  bool                          addTask(DValue const& task);
  DValue                        join(void);
  DValue                        map(DValue const& task);
protected:
  ~WorkerPool();
private:
  DUInt8                        __threadNumber;
  ThreadStruct*                 __threads;
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
