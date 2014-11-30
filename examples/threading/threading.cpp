#include <iostream>

#include "threadpool.hpp"

//using namespace Destruct;

int main(int argc, char** argv)
{
  std::cout << "creating thread pool of 4 thread " << std::endl;
  ThreadPool* threadPool = new ThreadPool(4);

  int count = 100000;

  DObject* vector = Destruct::Destruct::instance().find("DVectorInt64")->newObject();
  for (int index = 0; index < count; ++index)
    vector->call("push", RealValue<DInt64>(index));

  std::cout << "adding " << count <<" task" << std::endl;
  DValue get = vector->getValue("get");
  for (int i = 0; i < count; ++i)
  {
    DObject* task = makeNewDCpp<Task<DUInt64, DType::DUInt64Type> >("Task")->newObject(); 

    task->setValue("function", get);
    task->setValue("argument", RealValue<DUInt64>(i));
    threadPool->addTask(RealValue<DObject*>(task));
    //DObject* result = threadPool->addTask(RealValue<DObject*>(task));
  }

  std::cout << "waiting task is executed" << std::endl; 
  //DObject* result = threadPool->join(); 
  threadPool->join(); 

  std::cout << "end" << std::endl;
}
