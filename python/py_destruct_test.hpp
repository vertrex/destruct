#ifndef _DESTRUCTMODULE_TEST_HPP_
#define _DESTRUCTMODULE_TEST_HPP_

#include "destructmodule.hpp"
#include "destruct_test.hpp"

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

#endif
