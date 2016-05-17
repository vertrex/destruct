#include "py_dstructs.hpp"
#include "py_dstruct.hpp"

#include "dexception.hpp"

PyTypeObject*     PyDStructs::pyType(void)
{
    static PyTypeObject* pyType = (PyTypeObject*)malloc(sizeof(basePyType));
    return (pyType);
}

PyDStructs::PyDStructs()
{
  PyTypeObject* pyType = PyDStructs::pyType(); 
  memcpy(pyType , &basePyType , sizeof(basePyType));

  pyType->ob_type = &PyType_Type;
  pyType->tp_name = "destruct.DStructs";
  pyType->tp_basicsize = sizeof(DPyObject);
  pyType->tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE;
  pyType->tp_doc = "destruct.DStructs objects";
  pyType->tp_methods = PyDStructs::pyMethods;
  pyType->tp_methods = pyMethods;
  pyType->tp_init = (initproc)PyDStructs::_init;
  pyType->tp_new = PyDStructs::_new;
  pyType->tp_dealloc = (destructor)PyDStructs::_dealloc;

  if (PyType_Ready(pyType) < 0)
    throw Destruct::DException("PyType ready error");
}

PyObject* PyDStructs::count(PyDStructs::DPyObject* self, PyObject* args, PyObject* kwds)
{
  size_t count = self->pimpl->count();

  return (Py_BuildValue("i", count)); 
}

PyObject* PyDStructs::findObject(PyDStructs::DPyObject* self, PyObject* args, PyObject* kwds)
{
  if (PyString_Check(args))
  {  
    return (PyDStructs::find(self, PyString_AsString(args)));
  }  
  if (PyInt_Check(args))
  {
    return (PyDStructs::find(self, PyInt_AsLong(args)));
  }

  PyErr_SetString(PyExc_TypeError, "must be string or integer"); 
  return (0);
}

PyObject* PyDStructs::find(PyDStructs::DPyObject* self, const char* index)
{
  Destruct::DStruct* dstruct = self->pimpl->find(std::string(index));
  if (dstruct == NULL)
    Py_RETURN_NONE;

  PyDStruct::DPyObject* dstructObject = (PyDStruct::DPyObject*) _PyObject_New(PyDStruct::pyType());
  dstructObject->pimpl = dstruct;

  return((PyObject*) dstructObject); 
}

PyObject* PyDStructs::find(PyDStructs::DPyObject* self, size_t index)
{
  Destruct::DStruct* dstruct = self->pimpl->find(index);
  if (dstruct == NULL)
    Py_RETURN_NONE;

  PyDStruct::DPyObject* dstructObject = (PyDStruct::DPyObject*) _PyObject_New(PyDStruct::pyType());
  dstructObject->pimpl = dstruct;

  return((PyObject*)dstructObject); 
}

PyObject* PyDStructs::registerDStruct(PyDStructs::DPyObject* self, PyObject* args, PyObject* kwds)
{
  PyDStruct::DPyObject* dstructObject;
 
  if (!PyArg_ParseTuple(args, "O", &dstructObject))
    return (0);

  if (!PyObject_TypeCheck(dstructObject, PyDStruct::pyType()))
  {
    PyErr_SetString(PyExc_TypeError, "must be a DStruct object");
    return (0);
  } 

  self->pimpl->registerDStruct(dstructObject->pimpl);

  Py_RETURN_NONE;
}

PyMethodDef PyDStructs::pyMethods[] = 
{
  {"count", (PyCFunction)count, METH_NOARGS, "Return the number of registered DStruct."},
  {"find", (PyCFunction)findObject, METH_O, "Search and return a DStruct object. Return None if not found."},
  {"registerDStruct", (PyCFunction)registerDStruct, METH_VARARGS, "Add a new DStruct to the registry."},
  {NULL}
};

int PyDStructs::_init(PyDStructsT::DPyObject *self, PyObject *args, PyObject *kwds)
{
   self->pimpl = &Destruct::DStructs::instance();
 
   return (0);
}

void PyDStructs::_dealloc(PyDStructsT::DPyObject* self)
{
  self->ob_type->tp_free((PyObject* )self);
}
