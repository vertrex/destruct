#include <pthread.h>

#include "destruct.hpp"
#include "threadpool.hpp"
#include "protocol/dcppobject.hpp"

/**
 * Worker
 */
void *Worker(void *dobject)
{
  //DObject* workQueue = ((DObject*)dobject); //segfault car les method instanceOf() et call sont pas locker bizarre acces a une list en mme temps suffit 
  Queue* workQueue = ((Queue*)dobject);
  //workQueue->addRef();

  while (1)
  {
    DValue object = workQueue->dequeue(); //get
    DObject* task = object.get<DObject*>();

    DFunctionObject* function = task->getValue("function").get<DFunctionObject*>();
    DValue args = task->getValue("argument");

    DValue result = function->call(args);
    task->setValue("result", result);
    workQueue->addResult(RealValue<DObject*>(task));

    //std::cout << object.asUnicodeString() 
    //<< " => " << result.asUnicodeString() << std::endl; 
  }
  std::cout << "worker exit " << std::endl;
  pthread_exit(0);
}


/**
 * ThreadPool
 */
ThreadPool::ThreadPool(DUInt8 threadNumber) //: __workQueue(Destruct::Destruct::instance().generate("DVectorObject"))
{
  this->__threadNumber = threadNumber;
  this->__taskQueue = makeNewDCpp<Queue>("Queue")->newObject();
  this->__threads = new pthread_t[threadNumber + 1];
  for (int i = 0; i < threadNumber ; i++)
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

ThreadPool::~ThreadPool()
{
  std::cout << "~ThreadPool()" << std::endl;
}

DValue ThreadPool::join(void)
{
  return (this->__taskQueue->call("join"));
}


bool    ThreadPool::addTask(DValue const& args)
{
  this->__taskQueue->call("enqueue", args); 

  return (true);
}

DValue  ThreadPool::map(DValue const& arg)
{
  DObject* args = arg.get<DObject*>();
  DValue function = args->getValue("function");
  DObject* vector = args->getValue("argument").get<DObject*>();
  DObject* iterator = vector->call("iterator").get<DObject*>();

  std::cout << "mapping each tasks : " << vector->call("size").get<DUInt64>() << std::endl;
  
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
  pthread_mutex_init(&m_mutex, NULL);
  pthread_cond_init(&m_condv, NULL);
  this->__itemCount = 0;
  this->init();
}

Queue::~Queue()
{
  std::cout << "~Queue" << std::endl;
  pthread_mutex_destroy(&m_mutex);
  pthread_cond_destroy(&m_condv);
}

DValue  Queue::join(void)
{
  while (this->__itemCount != 0)
    continue;
  //pthread_cond_wait(&m_condv, &m_mutex);

  std::cout << "getting results " << std::endl;
  pthread_mutex_lock(&m_mutex);
  DObject* results = Destruct::Destruct::instance().generate("DVectorObject");
  while (!this->__result.empty())
  {
    DValue result = this->__result.front();
    results->call("push" , result);
    this->__result.pop();
  }
  pthread_mutex_unlock(&m_mutex);
  return RealValue<DObject*>(results);
}

void Queue::addResult(DValue const& task)
{
  pthread_mutex_lock(&m_mutex);
  this->__itemCount--;
  this->__result.push(task);
  pthread_mutex_unlock(&m_mutex);
}

DValue  Queue::empty(void)
{
  pthread_mutex_lock(&m_mutex);
  int size = this->__queue.empty();
  pthread_mutex_unlock(&m_mutex);

  return (RealValue<DUInt8>(size));
}

void    Queue::enqueue(DValue const& args)
{ 
  pthread_mutex_lock(&m_mutex);
  this->__itemCount++;
  this->__queue.push(args);
  pthread_cond_signal(&m_condv);
  pthread_mutex_unlock(&m_mutex);
}

DValue  Queue::dequeue(void)
{ 
  pthread_mutex_lock(&m_mutex);
  while (this->__queue.empty()) 
    pthread_cond_wait(&m_condv, &m_mutex);
          
  DValue object = this->__queue.front();
  this->__queue.pop();
  pthread_mutex_unlock(&m_mutex);
  
  return (object);
}
