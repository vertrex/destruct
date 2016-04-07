#include <iostream>

#include "workerpool.hpp"

int main(int argc, char** argv)
{
  //DUInt64 count = 1000000;
  //DType::init();
  WorkerPool::Declare();

  DUInt64 count = 100;

  try 
  {
    std::cout << "Creating thread pool of 4 threads" << std::endl;
    DObject* workerPool = DStructs::instance().find("WorkerPool")->newObject(RealValue<DUInt8>(4));


    std::cout << "generating data" << std::endl; 
    DObject* vector = DStructs::instance().find("DVectorUInt64")->newObject();
    for (DUInt64 index = 0; index < count; ++index)
      vector->call("push", RealValue<DUInt64>(index));

    std::cout << "creating " << count << " tasks " << std::endl;
    DValue get = vector->getValue("get");
    for (DUInt64 i = 0; i < count; ++i)
    {
      DObject* task = DStructs::instance().find("Task")->newObject(); 

      task->setValue("function", get);
      task->setValue("argument", RealValue<DUInt64>(i));
      workerPool->call("addTask", RealValue<DObject*>(task));
    }


    std::cout << "waiting tasks execution end" << std::endl; 
    DObject* result = workerPool->call("join");

    DUInt64 size = result->call("size").get<DUInt64>();
    if (size != count)
      throw DException("WorkerPool join can't get all results");
    else
      std::cout << "got all results (" << size << ")" << std::endl;

    for (DUInt64 index = 0; index < size; ++index)
    {
      DObject* task = result->call("get", RealValue<DUInt64>(index));
      //std::cout << "result " << index << " " << task->getValue("result").asUnicodeString() << std::endl;
      //task->destroy();
    }
    result->destroy();

    std::cout << "Creating task to map" << std::endl;
    DObject* task = DStructs::instance().find("TaskObject")->newObject();
    task->setValue("function", get);
    task->setValue("argument", RealValue<DObject* >(vector));

    std::cout << "map task" << std::endl;
    DObject* mapResult = workerPool->call("map", RealValue<DObject*>(task));
    task->destroy();

    size = mapResult->call("size").get<DUInt64>();
    if (size != count)
      throw (DException("Map didn't got all results"));
    else
      std::cout << "got all results (" << size << ")" << std::endl;

    for (DUInt64 index = 0; index < size; ++index)
    {
      DObject* task = mapResult->call("get", RealValue<DUInt64>(index));
      DValue result = task->getValue("result");
      // std::cout << result.asUnicodeString() << std::endl;
      task->destroy();
    }

    mapResult->destroy();
    vector->destroy();
    workerPool->destroy();
  }
  catch (DException const& exception)
  {
    std::cout << "Error: " << exception.error();
  }
  //DType::clean();
}
