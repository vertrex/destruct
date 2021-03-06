#ifndef _DESTRUCTMODULE_TEST_HPP_
#define _DESTRUCTMODULE_TEST_HPP_

#include <Python.h>
#include <frameobject.h>

#include "py_module.hpp"
#include "destruct_test.hpp"

#define DESTRUCT_ADD_MODULE(moduleType, moduleName)\
  Py_INCREF(moduleType);\
  PyModule_AddObject(module, moduleName, (PyObject*)moduleType);


typedef PythonModule< class PyTest, DestructTest > PyTestT;
class PyTest : public PyTestT 
{
public:
                      PyTest();
  static PyTypeObject*  pyType(void);
  static PyMethodDef  pyMethods[];

  static PyObject*    run(DPyObject* self, PyObject* args, PyObject *kwds);
  static PyObject*    showAttribute(DPyObject* self, PyObject* args, PyObject *kwds);
  static PyObject*    showObjectAttribute(DPyObject* self, PyObject* args, PyObject *kwds);
  static PyObject*    setObjectValue(DPyObject* self, PyObject* args, PyObject *kwds);
  static PyObject*    getObjectValue(DPyObject* self, PyObject* args, PyObject *kwds);
};


static PyMethodDef destruct_methods[] = 
{
  { NULL, NULL, 0, NULL }
};

#endif
