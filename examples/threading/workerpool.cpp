//#include <pthread.h>

#include "dstructs.hpp"
#include "workerpool.hpp"
#include "protocol/dcppobject.hpp"

#include "threadsafeobject.hpp"

extern "C"
{
  EXPORT void DestructExport(void)
  {
    WorkerPool::Declare();
  }
}

/**
 * Worker
 */
ThreadResult Worker(ThreadData dobject)
{
  //DObject* workQueue = ((DObject*)dobject); //segfault car les method instanceOf() et call sont pas locker bizarre acces a une list en mme temps suffit 
  Queue* workQueue = ((Queue*)dobject);

  while (1)
  {
    DValue object = workQueue->call("dequeue");
    DObject* task = object.get<DObject*>();

    DFunctionObject* function = task->getValue("function").get<DFunctionObject*>();
    DValue args = task->getValue("argument");

    DValue result = function->call(args);
    task->setValue("result", result);

    workQueue->call("addResult", RealValue<DObject*>(task));
  }
}


/**
 * WorkerPool
 */
WorkerPool::WorkerPool(DStruct* dstruct, DValue const& args) : DCppObject<WorkerPool>(dstruct, args) 
{
  this->init();

  this->__threadNumber = args.get<DUInt8>();
  this->__taskQueue = makeNewDCpp<Queue>("Queue")->newObject();

  this->__taskQueue = (new DStruct(NULL, "ThreadSafeObject", ThreadSafeObject::newObject))->newObject(RealValue<DObject*>(this->__taskQueue)); 

  this->__threads = new ThreadStruct[this->__threadNumber];

  for (int i = 0; i < this->__threadNumber; ++i)
  {
    createThread(Worker, (void*)this->__taskQueue, this->__threads[i])
  }

  std::cout << "wait for object" << std::endl;
}

WorkerPool::~WorkerPool()
{
  for(int i=0; i< this->__threadNumber; i++)
  {
    destroyThread(this->__threads[i]);
  }
  delete this->__threads;
}

DValue WorkerPool::join(void)
{
  return (this->__taskQueue->call("join"));
}


bool    WorkerPool::addTask(DValue const& args)
{
  this->__taskQueue->call("enqueue", args); 

  return (true);
}

DValue  WorkerPool::map(DValue const& arg)
{
  DObject* args = arg.get<DObject*>();
  DValue function = args->getValue("function");
  DObject* vector = args->getValue("argument").get<DObject*>();
  DObject* iterator = vector->call("iterator").get<DObject*>();

  for (; !iterator->call("isDone").get<DInt8>(); iterator->call("nextItem"))
  {
    //DObject* task = args->clone();
    DObject* task = makeNewDCpp< Task<DUInt64, DType::DUInt64Type, DUInt64, DType::DUInt64Type > >("Task")->newObject();
    task->setValue("function", function);
    task->setValue("argument", iterator->call("currentItem"));
    this->addTask(RealValue<DObject*>(task));
  }
  return (this->__taskQueue->call("join"));
}

/**
 * Queue
 */
Queue::Queue(DStruct* dstruct, DValue const& args) : DCppObject<Queue>(dstruct, args)
{    
  mutex_init(&__mutex);
  mutex_init(&__joinMutex);
  cond_init(&__enqueueSignal);
  cond_init(&__itemCountSignal);
  this->__itemCount = 0;
  this->init();
}

Queue::~Queue()
{
  mutex_destroy(&__mutex);
  mutex_destroy(&__joinMutex);
  cond_destroy(&__enqueueSignal);
  cond_destroy(&__itemCountSignal);
}

DValue  Queue::join(void)
{
  mutex_lock(&__joinMutex);
  while (this->__itemCount != 0)
    cond_wait(&__itemCountSignal, &__joinMutex);

  DObject* results = Destruct::DStructs::instance().generate("DVectorObject");
  while (!this->__result.empty())
  {
    DValue result = this->__result.front();
    results->call("push" , result);
    this->__result.pop();
  }
  mutex_unlock(&__joinMutex);
  return RealValue<DObject*>(results);
}

void Queue::addResult(DValue const& task)
{
  mutex_lock(&__mutex);
  this->__itemCount--;
  this->__result.push(task);
  if (this->__itemCount == 0)
    cond_signal(&__itemCountSignal);
  mutex_unlock(&__mutex);
}

DValue  Queue::empty(void)
{
  mutex_lock(&__mutex);
  int size = this->__queue.empty();
  mutex_unlock(&__mutex);

  return (RealValue<DUInt8>(size));
}

void    Queue::enqueue(DValue const& args)
{ 
  mutex_lock(&__mutex);
  this->__itemCount++;
  this->__queue.push(args);
  cond_signal(&__enqueueSignal);
  mutex_unlock(&__mutex);
}

DValue  Queue::dequeue(void)
{ 
  mutex_lock(&__mutex);
  while (this->__queue.empty()) 
    cond_wait(&__enqueueSignal, &__mutex);
          
  DValue object = this->__queue.front();
  this->__queue.pop();
  mutex_unlock(&__mutex);
  
  return (object);
}
