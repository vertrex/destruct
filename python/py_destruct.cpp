#include "py_destruct.hpp"
#include "py_dstruct.hpp"

template<>
PyTypeObject* PyDestructT::pyType = NULL;

PyDestruct::PyDestruct()
{
  pyType = (PyTypeObject*)malloc(sizeof(basePyType));
  memcpy(pyType , &basePyType , sizeof(basePyType));

  pyType->tp_name = "destruct.Destruct";
  pyType->tp_basicsize = sizeof(DPyObject);
  pyType->tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE;
  pyType->tp_doc = "destruct.Destruct objects";
  pyType->tp_methods = PyDestruct::pyMethods;
  pyType->tp_methods = pyMethods;
  pyType->tp_init = (initproc)PyDestruct::_init;
  pyType->tp_new = PyDestruct::_new;
  pyType->tp_dealloc = (destructor)PyDestruct::_dealloc;

  if (PyType_Ready(pyType) < 0)
    throw std::string("PyType ready error");
}

PyObject* PyDestruct::count(PyDestruct::DPyObject* self, PyObject* args, PyObject* kwds)
{
  size_t count = self->pimpl->count();

  return (Py_BuildValue("i", count)); 
}

PyObject* PyDestruct::findObject(PyDestruct::DPyObject* self, PyObject* args, PyObject* kwds)
{
  if (PyString_Check(args))
  {  
    return (PyDestruct::find(self, PyString_AsString(args)));
  }  
  if (PyInt_Check(args))
  {
    return (PyDestruct::find(self, PyInt_AsLong(args)));
  }
 
  PyErr_SetString(PyExc_TypeError, "must be string or integer"); 
  return (0);
}

PyObject* PyDestruct::find(PyDestruct::DPyObject* self, const char* index)
{
  Destruct::DStruct* dstruct = self->pimpl->find(index);
  if (dstruct == NULL)
    Py_RETURN_NONE;

  PyDStruct::DPyObject* dstructObject = (PyDStruct::DPyObject*) _PyObject_New(PyDStruct::pyType);
  dstructObject->pimpl = dstruct;

  return(Py_BuildValue("O", dstructObject)); 
}

PyObject* PyDestruct::find(PyDestruct::DPyObject* self, size_t index)
{
  Destruct::DStruct* dstruct = self->pimpl->find(index);
  if (dstruct == NULL)
    Py_RETURN_NONE;

  PyDStruct::DPyObject* dstructObject = (PyDStruct::DPyObject*) _PyObject_New(PyDStruct::pyType);
  dstructObject->pimpl = dstruct;

  return(Py_BuildValue("O", dstructObject)); 
}

PyObject* PyDestruct::registerDStruct(PyDestruct::DPyObject* self, PyObject* args, PyObject* kwds)
{
  PyDStruct::DPyObject* dstructObject;
 
  if (!PyArg_ParseTuple(args, "O", &dstructObject))
    return (0);

  if (!PyObject_TypeCheck(dstructObject, PyDStruct::pyType))
  {
    PyErr_SetString(PyExc_TypeError, "must be a DStruct object");
    return (0);
  } 

  self->pimpl->registerDStruct(dstructObject->pimpl);

  Py_RETURN_NONE;
}

PyMethodDef PyDestruct::pyMethods[] = 
{
  {"count", (PyCFunction)count, METH_NOARGS, "Return the number of registered DStruct."},
  {"find", (PyCFunction)findObject, METH_O, "Search and return a DStruct object. Return None if not found."},
  {"registerDStruct", (PyCFunction)registerDStruct, METH_VARARGS, "Add a new DStruct to the registry."},
  {NULL}
};

int PyDestruct::_init(PyDestructT::DPyObject *self, PyObject *args, PyObject *kwds)
{
   self->pimpl = &Destruct::Destruct::instance();
 
   return (0);
}

void PyDestruct::_dealloc(PyDestructT::DPyObject* self)
{
  self->ob_type->tp_free((PyObject* )self);
}
