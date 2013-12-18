#ifndef _DESTRUCTMODULE_TEST_HPP_
#define _DESTRUCTMODULE_TEST_HPP_

#include <python2.7/Python.h>
#include <python2.7/frameobject.h>

#include "destructmodule.hpp"
#include "destruct_test.hpp"

#define DESTRUCT_ADD_MODULE(moduleType, moduleName)\
  Py_INCREF(moduleType);\
  PyModule_AddObject(module, moduleName, (PyObject*)moduleType);


typedef PythonModule< class PyTest, Destruct::DestructTest > PyTestT;
class PyTest : public PyTestT 
{
public:
                      PyTest();
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

PyMODINIT_FUNC init_dtest(void)
{
  PyObject* module;

  PyTest::moduleInit();

  if ((module = Py_InitModule3("_dtest", destruct_methods, "Test library for destruct")) == NULL)
    return;    

  DESTRUCT_ADD_MODULE(PyTest::pyType, "Test")
}


#endif
