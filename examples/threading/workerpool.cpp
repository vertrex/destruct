#include <pthread.h>

#include "destruct.hpp"
#include "workerpool.hpp"
#include "protocol/dcppobject.hpp"

#include "threadsafeobject.hpp"

/**
 * Worker
 */
void *Worker(void *dobject)
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
  pthread_exit(0);
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

  this->__threads = new pthread_t[this->__threadNumber];
  for (int i = 0; i < this->__threadNumber ; ++i)
  {
    int result = pthread_create(&this->__threads[i], NULL, Worker, (void *)this->__taskQueue);
    if (result)
    {
      std::cout << "Error:unable to create thread," << result << std::endl;
      break; 
    } 
  }

 //pthread_exit(NULL);
}

WorkerPool::~WorkerPool()
{
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
  pthread_mutex_init(&__mutex, NULL);
  pthread_cond_init(&__conditionWait, NULL);
  this->__itemCount = 0;
  this->init();
}

Queue::~Queue()
{
  pthread_mutex_destroy(&__mutex);
  pthread_cond_destroy(&__conditionWait);
}

DValue  Queue::join(void)
{
  while (this->__itemCount != 0)
    continue;
  //pthread_cond_wait(&__conditionWaitv, &__mutex);

  pthread_mutex_lock(&__mutex);
  DObject* results = Destruct::Destruct::instance().generate("DVectorObject");
  while (!this->__result.empty())
  {
    DValue result = this->__result.front();
    results->call("push" , result);
    this->__result.pop();
  }
  pthread_mutex_unlock(&__mutex);
  return RealValue<DObject*>(results);
}

void Queue::addResult(DValue const& task)
{
  pthread_mutex_lock(&__mutex);
  this->__itemCount--;
  this->__result.push(task);
  pthread_mutex_unlock(&__mutex);
}

DValue  Queue::empty(void)
{
  pthread_mutex_lock(&__mutex);
  int size = this->__queue.empty();
  pthread_mutex_unlock(&__mutex);

  return (RealValue<DUInt8>(size));
}

void    Queue::enqueue(DValue const& args)
{ 
  pthread_mutex_lock(&__mutex);
  this->__itemCount++;
  this->__queue.push(args);
  pthread_cond_signal(&__conditionWait);
  pthread_mutex_unlock(&__mutex);
}

DValue  Queue::dequeue(void)
{ 
  pthread_mutex_lock(&__mutex);
  while (this->__queue.empty()) 
    pthread_cond_wait(&__conditionWait, &__mutex);
          
  DValue object = this->__queue.front();
  this->__queue.pop();
  pthread_mutex_unlock(&__mutex);
  
  return (object);
}
