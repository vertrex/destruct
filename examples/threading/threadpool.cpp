#include <pthread.h>

#include "destruct.hpp"
#include "threadpool.hpp"


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
    //std::cout << object.asUnicodeString() 
    //<< " => " << result.asUnicodeString() << std::endl; 
    workQueue->deref();
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

void ThreadPool::join(void)
{
  this->__taskQueue->call("join");
}


bool    ThreadPool::addTask(DValue const& args)
{
  this->__taskQueue->call("enqueue", args); 

  return (true);
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

void Queue::join(void)
{
  while (this->__itemCount != 0)
    continue;
   //pthread_cond_wait(&m_condv, &m_mutex);
}

void Queue::deref(void)
{
  pthread_mutex_lock(&m_mutex);
  this->__itemCount--;
  pthread_mutex_unlock(&m_mutex);
}

DValue  Queue::empty(void)
{
  pthread_mutex_lock(&m_mutex);
  int size = this->__queue.empty();
  pthread_mutex_unlock(&m_mutex);

  return (RealValue<DUInt8>(size));
}

void    Queue::enqueue(DValue const& args) //DFunctionObject*, DValue const& args)
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

/**
 * Task
 */
//Task::Task(DFunctionObject* functionObject, DValue const& args) : __functionObject(functionObject), __args(args)
//{
//}
