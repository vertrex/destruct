#include "py_destruct_test.hpp"
#include "py_dstruct.hpp"
#include "py_dobject.hpp"

template<>
PyTypeObject* PyTestT::pyType = NULL;

PyTest::PyTest()
{
  pyType = (PyTypeObject*)malloc(sizeof(basePyType));
  memcpy(pyType , &basePyType , sizeof(basePyType));

  pyType->tp_name = "destruct.Test";
  pyType->tp_basicsize = sizeof(PyTest::DPyObject);
  pyType->tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE;
  pyType->tp_doc = "destruct.Test objects";
  pyType->tp_methods = PyTest::pyMethods;
  pyType->tp_methods = pyMethods;
  pyType->tp_init = (initproc)_init;
  pyType->tp_new = _new;
  pyType->tp_dealloc = (destructor)_dealloc;

  if (PyType_Ready(pyType) < 0)
    throw std::string("PyType ready error");
}

PyObject*    PyTest::run(PyTest::DPyObject* self, PyObject* args, PyObject *kwds)
{
  self->pimpl->run();

  Py_RETURN_NONE;
}

PyObject*   PyTest::showAttribute(PyTest::DPyObject* self, PyObject* args, PyObject *kwds)
{
  PyDStruct::DPyObject*       dstructObject;

  if (!PyArg_ParseTuple(args, "O", &dstructObject))
    return (0);

  if (!PyObject_TypeCheck(dstructObject, PyDStruct::pyType))
    return (0);

  self->pimpl->showAttribute(dstructObject->pimpl);
  Py_RETURN_NONE;
}

PyObject*    PyTest::showObjectAttribute(PyTest::DPyObject* self, PyObject* args, PyObject *kwds)
{
  PyDObject::DPyObject*        dobjectObject;

  if (!PyArg_ParseTuple(args, "O", &dobjectObject))
    return (0);

  if (!PyObject_TypeCheck(dobjectObject, PyDObject::pyType))
    return (0);

  self->pimpl->showObjectAttribute(dobjectObject->pimpl);
  Py_RETURN_NONE;
}


PyObject*    PyTest::setObjectValue(PyTest::DPyObject* self, PyObject* args, PyObject *kwds)
{ 
  PyDObject::DPyObject*        dobjectObject;

  if (!PyArg_ParseTuple(args, "O", &dobjectObject))
    return (0);

  if (!PyObject_TypeCheck(dobjectObject, PyDObject::pyType))
    return (0);


  self->pimpl->setObjectValue(dobjectObject->pimpl);
  Py_RETURN_NONE;
}


PyObject*    PyTest::getObjectValue(PyTest::DPyObject* self, PyObject* args, PyObject *kwds)
{
  Destruct::DObject* dobject = self->pimpl->getObjectValue();

  PyDObject::DPyObject*  dobjectObject = (PyDObject::DPyObject*)_PyObject_New(PyDObject::pyType);
  dobjectObject->pimpl = dobject;

  Py_INCREF(dobjectObject);

  return (Py_BuildValue("O", dobjectObject));
}

PyMethodDef PyTest::pyMethods[] = 
{
  {"run", (PyCFunction)run, METH_NOARGS, "Run basic cpp test."},
  {"showAttribute", (PyCFunction)showAttribute, METH_VARARGS, "Show DStruct attribute."},
  {"showObjectAttribute", (PyCFunction)showObjectAttribute, METH_VARARGS, "Show DObject attribute and value."},
  {"setObjectValue", (PyCFunction)setObjectValue, METH_VARARGS, "Set default val to a DObject (Test transfer of python object to CPP)."},
  {"getObjectValue", (PyCFunction)getObjectValue, METH_NOARGS, "Get a DObject with setted value (Test transfer of CPP object to Python)."},
  {NULL}
};
