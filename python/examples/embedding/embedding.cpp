#include <Python.h>
#include <iostream>

#include "dstructs.hpp"
#include "dexception.hpp"

#include "embedding.hpp"
#include "py_dobject.hpp"

PythonInterpreter::PythonInterpreter(void) : __pyModule(NULL)
{
  Py_SetProgramName((char*)"embedding");
  Py_Initialize();

  PyRun_SimpleString("print 'python interpreter loaded'");
}

PythonInterpreter::~PythonInterpreter(void)
{
  if (this->__pyModule)
    Py_DECREF(this->__pyModule);
  Py_Finalize();
}

void    PythonInterpreter::import(void)
{
  PyRun_SimpleString("import sys");
  PyRun_SimpleString("sys.path.append('.')");

  PyObject* moduleName = PyString_FromString("pyembedding");
  this->__pyModule = PyImport_Import(moduleName);
  Py_DECREF(moduleName);

  if (this->__pyModule == NULL)
    throw Destruct::DException("Can't load module pyembedding"); 
}

PyObject*   PythonInterpreter::getPythonObject(void)
{
  PyObject* pyObject = NULL;
  PyObject* getPythonObject = PyObject_GetAttrString(this->__pyModule, "getPythonObject");

  if (getPythonObject && PyCallable_Check(getPythonObject)) 
    pyObject = PyObject_CallObject(getPythonObject, NULL);
 
  if (getPythonObject)
    Py_DECREF(getPythonObject);

  return (pyObject);
}

void    PythonInterpreter::showObject(PyObject* dobject)
{
  PyObject* pyShowObject = PyObject_GetAttrString(this->__pyModule, "showObject");

  if (pyShowObject && PyCallable_Check(pyShowObject)) 
  {
    Py_INCREF(dobject);
    PyObject* argument = Py_BuildValue("(O)", dobject);
    PyObject_CallObject(pyShowObject, argument);
    Py_DECREF(pyShowObject);
    Py_DECREF(argument);
  }
}

void showObject(Destruct::DObject* dobject)
{
  std::cout << "showing object of type " << dobject->instanceOf()->name() << std::endl;
  std::cout << "object->name " << dobject->getValue("name").get<Destruct::DUnicodeString>() << std::endl;
  std::cout << "object->size " << dobject->getValue("size").get<DInt64>() << std::endl;
}

void setPythonObject(Destruct::DObject* dobject)
{
  dobject->setValue("name", Destruct::RealValue<Destruct::DUnicodeString>("cpp name"));
  dobject->setValue("size", Destruct::RealValue<DInt64>(512));
}

int main(int argc, char **argv)
{
  PythonInterpreter     pythonInterpreter;

  try
  {
    pythonInterpreter.import();
    PyObject* pyObject = pythonInterpreter.getPythonObject();

    Destruct::DObject* dobject = ((PyDObject::DPyObject*)pyObject)->pimpl;
    showObject(dobject);
    setPythonObject(dobject);

    pythonInterpreter.showObject(pyObject);
    Py_DECREF(pyObject);
  }
  catch (Destruct::DException const& exception)
  {
    std::cout << exception.error() << std::endl;
  }

  return (0); 
}

