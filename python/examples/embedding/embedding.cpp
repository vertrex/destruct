#include <python2.7/Python.h>
#include "dstructs.hpp"

#include <iostream>

#include "embedding.hpp"

PythonInterpreter::PythonInterpreter(void)
{
  Py_SetProgramName((char*)"embedding");
  Py_Initialize();

  PyRun_SimpleString("print 'python interpreter loaded'");
}

PythonInterpreter::~PythonInterpreter(void)
{
  Py_Finalize();
}

void PythonInterpreter::loadModule(std::string path)
{
        //std::cout << "Loading python module" << std::endl;
        //PyRun_SimpleString("from pyembedding import *\nembed = Embed()\nembed.run()");
        //std::cout << "Loading finish " << std::endl;
}

void PythonInterpreter::registerStructures(void)
{
        //typedef Destruct::DVector<Destruct::DUnicodeString, Destruct::DType::DUnicodeStringType> DVectorString;
        //Destruct::DStruct* dstructvectors = Destruct::makeNewDClass < DVectorString >(NULL, "DVectorString");
        //Destruct::DStructs::instance().registerDStruct(dstructvectors);
}

int main(int argc, char **argv)
{

  PythonInterpreter     pythonInterpreter;

  pythonInterpreter.registerStructures();
  pythonInterpreter.loadModule(".");

 
  //std::cout << "--- Showing destruct database --- " << std::endl; 
  //Destruct::DStructs&  destruct = Destruct::DStructs::instance(); //::instance();
  //for (size_t i = 0; i < destruct.count() ; i++)
  //std::cout << destruct.find(i)->name() << std::endl;

         
  return (0); 
}
/*
TaskManager::TaskManager
{
  cmd = exec("module arguments")
  sched.add(cmd)
}


class loader()
{
 load(path)  

}

class PythonLoader() : loader()
{
 PyInitialize()

 loadpythonmodule("python/")
  addModule(module)
}



class cmd
{
  DFunction __function;
  DObject   __arguments;
  exec()
  {
    function->(argument);
  }
}

Scheduler::Scheduler
{
  DList<cmd>; 


  for cmd in cmds:
  {
    worker = pool_getworker()
    worker.execute(cmd)
  }
}*/
