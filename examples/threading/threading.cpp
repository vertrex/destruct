#include <iostream>

#include "threadpool.hpp"

//using namespace Destruct;

int main(int argc, char** argv)
{
  std::cout << "creating thread pool of 4 thread " << std::endl;
  ThreadPool* threadPool = new ThreadPool(4);

  DObject* vector = Destruct::Destruct::instance().find("DVectorInt64")->newObject();
  for (uint32_t index = 0; index < 10; ++index)
    vector->call("push", RealValue<DInt64>(index));

  int count = 100;
  std::cout << "adding " << count <<" task" << std::endl;
  DFunctionObject* get = vector->getValue("get").get<DFunctionObject*>();
  for (int i = 0; i < count; ++i)
    threadPool->addTask(RealValue<DFunctionObject*>(get));
 
  std::cout << "waiting task is executed" << std::endl; 
  threadPool->join(); 
  std::cout << "end" << std::endl;
}
