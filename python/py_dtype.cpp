#include "py_dtype.hpp"
#include "py_dvalue.hpp"
#include "py_dobject.hpp"

#include "dexception.hpp"

PyTypeObject*     PyDType::pyType(void)
{
  static PyTypeObject* pyType = (PyTypeObject*)malloc(sizeof(basePyType));
  return (pyType);
}

PyDType::PyDType()
{
  Destruct::DType::init();//XXX should not be done here  if already called by cpp, must be freed at end  
  //pyType = (PyTypeObject*)malloc(sizeof(basePyType));
  PyTypeObject* pyType = PyDType::pyType();
  memcpy(pyType , &basePyType , sizeof(basePyType));
  
  pyType->ob_type = &PyType_Type;
  pyType->tp_name = "destruct.DType";
  pyType->tp_basicsize = sizeof(PyDType::DPyObject);
  pyType->tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE;
  pyType->tp_doc = "destruct.DType objects";
  pyType->tp_methods = PyDType::pyMethods;
  pyType->tp_methods = pyMethods;
  pyType->tp_init = (initproc)PyDType::_init;
  pyType->tp_new = _new;
  pyType->tp_dealloc = (destructor)_dealloc;
  //pyType->tp_repr = (reprfunc)PyDType::_repr;
  pyType->tp_str = (reprfunc)PyDType::_repr;

  if (PyType_Ready(pyType) < 0)
    throw Destruct::DException("PyType ready error");
}

PyObject* PyDType::getType(PyDType::DPyObject* self, PyObject *args, PyObject* kwds)
{
  int typeId = self->pimpl->getType();
  if (typeId >= Destruct::DType::DUnknownType)
    Py_RETURN_NONE;
  return (DValueDispatchTable[typeId]->typeObject());
}

PyObject* PyDType::returnType(PyDType::DPyObject* self, PyObject *args, PyObject* kwds)
{
  int typeId = self->pimpl->getReturnType();
  if (typeId >= Destruct::DType::DUnknownType)
    Py_RETURN_NONE;
  return (DValueDispatchTable[typeId]->typeObject());
}

PyObject* PyDType::argumentType(PyDType::DPyObject* self, PyObject *args, PyObject* kwds)
{
  int typeId = self->pimpl->getArgumentType();
  if (typeId >= Destruct::DType::DUnknownType)
    Py_RETURN_NONE;
  return (DValueDispatchTable[typeId]->typeObject());
}

PyObject* PyDType::_repr(PyDType::DPyObject* self)
{
  CHECK_PIMPL

  Destruct::DUnicodeString fullTypeName = "";
  Destruct::DUnicodeString returnName = self->pimpl->returnName();
  Destruct::DUnicodeString typeName = self->pimpl->name();
  Destruct::DUnicodeString argumentName = self->pimpl->argumentName();
  
  if (returnName != "DUnknown")
     fullTypeName = returnName + " ";
  fullTypeName += typeName;
  if (argumentName != "DUnknown")
    fullTypeName += " " + argumentName;
  
  return (PyString_FromString(fullTypeName.c_str()));
}

PyObject* PyDType::returnName(PyDType::DPyObject* self, PyObject* args, PyObject* kwds)
{
  CHECK_PIMPL
  return (PyString_FromString(self->pimpl->returnName().c_str()));
}

PyObject* PyDType::name(PyDType::DPyObject* self, PyObject* args, PyObject* kwds)
{
  CHECK_PIMPL
  return (PyString_FromString(self->pimpl->name().c_str()));
}

PyObject* PyDType::argumentName(PyDType::DPyObject* self, PyObject* args, PyObject* kwds)
{
  CHECK_PIMPL
  return (PyString_FromString(self->pimpl->argumentName().c_str()));
}

PyMethodDef PyDType::pyMethods[] = 
{
  {"getType", (PyCFunction)PyDType::getType, METH_NOARGS, "Return the type."},
  {"returnType", (PyCFunction)PyDType::returnType, METH_NOARGS, "Return method return type."},
  {"argumentType", (PyCFunction)PyDType::argumentType, METH_NOARGS, "Return method argument type."},
  {"name", (PyCFunction)PyDType::name, METH_NOARGS, "Return the type name."},
  {"returnName", (PyCFunction)PyDType::returnName, METH_NOARGS, "Return method return type name."},
  {"argumentName", (PyCFunction)PyDType::argumentName, METH_NOARGS, "Return method argument type name."},
//XXX name() add for serialization
  {NULL}
};

int PyDType::_init(PyDTypeT::DPyObject *self, PyObject *args, PyObject *kwds)
{
  int  typeId = Destruct::DType::DUnknownType;

  if (!PyArg_ParseTuple(args, "i", &typeId))
  {
    return (-1);
  }

  if ((typeId >= Destruct::DType::DUnknownType) || (DValueDispatchTable[typeId] == NULL))
  { 
    PyErr_SetString(PyExc_RuntimeError, "Bad type id value");
    return (0);
  }

  self->pimpl = new Destruct::DType((Destruct::DType::Type_t)typeId);
  INIT_CHECK_ALLOC(self->pimpl)
 
  return (0);
}

/*
void PyDType::_dealloc(PyDTypeT::DPyObject* self)
{
  delete self->pimpl;
  Destruct::DType::clean();
  self->ob_type->tp_free((PyObject*)self);
}
*/
