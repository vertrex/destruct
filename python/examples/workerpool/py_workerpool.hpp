#ifndef _DESTRUCTMODULE_EXAMPLES_HPP_
#define _DESTRUCTMODULE_EXAMPLES_HPP_

#include <Python.h>
#include <frameobject.h>

#include "destructmodule.hpp"


#define DESTRUCT_ADD_MODULE(moduleType, moduleName)\
  Py_INCREF(moduleType);\
  PyModule_AddObject(module, moduleName, (PyObject*)moduleType);


typedef PythonModule< class PyExamples, PyExamples > PyExamplesT;
class PyExamples : public PyExamplesT 
{
public:
                      PyExamples();
  static PyTypeObject*  pyType(void);
  static PyMethodDef  pyMethods[];
};

static PyMethodDef destruct_methods[] = 
{
  { NULL, NULL, 0, NULL }
};

#endif
