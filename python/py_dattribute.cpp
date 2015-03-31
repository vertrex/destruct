#include "py_dattribute.hpp"
#include "py_dtype.hpp"

#include "dstruct.hpp"
#include "dsimpleobject.hpp"
#include "dattribute.hpp"

PyTypeObject*     PyDAttribute::pyType(void)
{
    static PyTypeObject* pyType = (PyTypeObject*)malloc(sizeof(basePyType));
    return (pyType);
}

PyDAttribute::PyDAttribute()
{
  PyTypeObject* pyType = PyDAttribute::pyType();
  memcpy(pyType , &basePyType , sizeof(basePyType));
  
  pyType->ob_type = &PyType_Type;
  pyType->tp_name = "destruct.DAttribute";
  pyType->tp_basicsize = sizeof(PyDAttribute::DPyObject);
  pyType->tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE;
  pyType->tp_doc = "destruct.DAttribute objects";
  pyType->tp_methods = PyDAttribute::pyMethods;
  pyType->tp_methods = pyMethods;
  pyType->tp_init = (initproc)PyDAttribute::_init;
  pyType->tp_new = _new;
  pyType->tp_dealloc = (destructor)_dealloc;

  if (PyType_Ready(pyType) < 0)
    throw Destruct::DException("PyType ready error");
}

PyObject* PyDAttribute::name(PyDAttribute::DPyObject* self, PyObject* args, PyObject* kwds)
{
  return (Py_BuildValue("s", self->pimpl->name().c_str())); 
}

PyObject* PyDAttribute::description(PyDAttribute::DPyObject* self, PyObject* args, PyObject* kwds)
{
  return (PyString_FromString(self->pimpl->description().c_str()));
}

PyObject* PyDAttribute::dtype(PyDAttribute::DPyObject* self, PyObject*args, PyObject* kwds)
{
  Destruct::DType* dtype = new Destruct::DType(self->pimpl->type());
  CHECK_ALLOC(dtype) 

  PyDType::DPyObject* dtypeObject = (PyDType::DPyObject*) _PyObject_New(PyDType::pyType());
  dtypeObject->pimpl = dtype;

  return (Py_BuildValue("O", dtypeObject));
}

PyMethodDef PyDAttribute::pyMethods[] = 
{
  {"name", (PyCFunction)PyDAttribute::name, METH_NOARGS, "Return the name of the attribute."},
  {"dtype", (PyCFunction)PyDAttribute::dtype, METH_NOARGS, "Return the type of the attribute."},
  {"description", (PyCFunction)PyDAttribute::description, METH_NOARGS, "Return a description of the attribute."},
  {NULL}
};

int PyDAttribute::_init(PyDAttributeT::DPyObject *self, PyObject *args, PyObject *kwds)
{
  int                          objectId = Destruct::DType::DUnknownType;
  int                          returnId = Destruct::DType::DUnknownType;
  int                          argumentId = Destruct::DType::DUnknownType;
  const char*                  name = NULL;
  PyObject*                    dtypeObject;
  PyObject*                    dtypeReturn;
  PyObject*                    dtypeArgument;

  if (PyArg_ParseTuple(args, "sO", &name, &dtypeObject))
  {
    if (!PyType_Check(dtypeObject))
      return (-1);

    PyObject* objectTypeId = PyObject_CallMethod(dtypeObject, (char *)"getType", NULL);

    objectId = PyInt_AsLong(objectTypeId);
    self->pimpl = new Destruct::DAttribute((Destruct::DType::Type_t)objectId, std::string(name));
    INIT_CHECK_ALLOC(self->pimpl)

    return (0);
  }
  else if (PyArg_ParseTuple(args, "sOOO", &name, &dtypeObject, &dtypeReturn, &dtypeArgument))
  {
    if (!PyType_Check(dtypeObject))
      return (-1);

    PyErr_Clear(); //car le premier parse tuple a mis une erreur ? a check

    PyObject* objectTypeId = PyObject_CallMethod(dtypeObject, (char *)"getType", NULL);
    PyObject* returnTypeId = PyObject_CallMethod(dtypeReturn, (char *)"getType", NULL);
    PyObject* argumentTypeId = PyObject_CallMethod(dtypeArgument, (char *)"getType", NULL);

    objectId = PyInt_AsLong(objectTypeId);
    returnId = PyInt_AsLong(returnTypeId);
    argumentId = PyInt_AsLong(argumentTypeId);

    self->pimpl = new Destruct::DAttribute((Destruct::DType::Type_t)returnId, std::string(name), (Destruct::DType::Type_t)argumentId, (Destruct::DType::Type_t)objectId);
    INIT_CHECK_ALLOC(self->pimpl)
    return (0);
  }

  return (-1);
}
