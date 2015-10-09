#include <Python.h>
#include <iostream>

#include "dstructs.hpp"

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

void PythonInterpreter::loadModule(void)
{
  std::cout << "Loading python module" << std::endl;
  PyRun_SimpleString("import sys");
  PyRun_SimpleString("sys.path.append('../loader/')");
  PyRun_SimpleString("from loader import loadPath");

  std::string path = "[('dtest', 'destruct_test',),\
	      ('threading', 'dthreading',),\
	      ('inheritance', 'destruct_inherit',),\
              ('rpc', 'destruct_rpc',),\
              ('registry', 'registry',),\
	     ]";

  std::string cmd = "loadPath(" + path + ")";

  PyRun_SimpleString(cmd.c_str());
  std::cout << "Loading finish " << std::endl;

  std::cout << "Destructs total struct" << Destruct::DStructs::instance().count() << std::endl;
}

int main(int argc, char **argv)
{
  PythonInterpreter     pythonInterpreter;

  pythonInterpreter.loadModule();
 
  return (0); 
}

