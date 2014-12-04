#ifndef _DESTRUCTMODULE_EXAMPLES_HPP_
#define _DESTRUCTMODULE_EXAMPLES_HPP_

#include <python2.7/Python.h>
#include <python2.7/frameobject.h>

#include "destructmodule.hpp"
//#include "registry.hpp"
//#include "registryclient.hpp"

#define DESTRUCT_ADD_MODULE(moduleType, moduleName)\
  Py_INCREF(moduleType);\
  PyModule_AddObject(module, moduleName, (PyObject*)moduleType);


typedef PythonModule< class PyExamples, PyExamples > PyExamplesT;
class PyExamples : public PyExamplesT 
{
public:
                      PyExamples();
  static PyMethodDef  pyMethods[];
};


static PyMethodDef destruct_methods[] = 
{
  { NULL, NULL, 0, NULL }
};

#endif
