#include <iostream>

#include "workerpool.hpp"

int main(int argc, char** argv)
{
  try 
  {
  std::cout << "creating thread pool of 4 thread " << std::endl;
  DObject* workerPool = makeNewDCpp<WorkerPool>("WorkerPool")->newObject(RealValue<DUInt8>(4));

  int count = 1000000;
  
  std::cout << "generating number" << std::endl;
  DObject* vector = Destruct::DStructs::instance().find("DVectorUInt64")->newObject();
  for (int index = 0; index < count; ++index)
    vector->call("push", RealValue<DUInt64>(index));

  std::cout << "adding " << count <<" task" << std::endl;
  DValue get = vector->getValue("get");
  for (int i = 0; i < count; ++i)
  {
    DObject* task = makeNewDCpp<Task<DUInt64, DType::DUInt64Type, DUInt64, DType::DUInt64Type> >("Task")->newObject(); 

    task->setValue("function", get);
    task->setValue("argument", RealValue<DUInt64>(i));
    workerPool->call("addTask", RealValue<DObject*>(task));
  }

  std::cout << "waiting task is executed" << std::endl; 
  DObject* result = workerPool->call("join").get<DObject*>(); 
  
  DUInt64 size = result->call("size").get<DUInt64>();
  std::cout << "calcul end got results type : " << result->instanceOf()->name() << std::endl;
  if (size != count)
	throw DException("WorkerPool join can't get all results");

  size = result->call("size").get<DUInt64>();
  for (DUInt64 index = 0; index < size; ++index)
  {
    DObject* task = result->call("get", RealValue<DUInt64>(index)).get<DObject*>();
  //  std::cout << "result " << index << " " << task->getValue("result").asUnicodeString() << std::endl;
  }

  std::cout << "create task" << std::endl;
  DObject* task = makeNewDCpp<Task<DObject*, DType::DObjectType, DUInt64, DType::DUInt64Type > >("Task")->newObject();
  task->setValue("function", get);
  task->setValue("argument", RealValue<DObject* >(vector));

  std::cout << "map task" << std::endl;
  DObject* mapResult = workerPool->call("map", RealValue<DObject*>(task)).get<DObject*>();
 
  size = mapResult->call("size").get<DUInt64>();
  std::cout << "got " << size << " result " << std::endl;
  if (size != count)
	throw (DException("Map didn't got all results"));

  for (DUInt64 index = 0; index < size; ++index)
  {
    DObject* task = mapResult->call("get", RealValue<DUInt64>(index)).get<DObject* >();
    DValue result = task->getValue("result");
   // std::cout << result.asUnicodeString() << std::endl;
  }
  }
  catch (DException const& exception)
  {
    std::cout << "Error: " << exception.error();
  }
  std::cout << "end" << std::endl;
}
