#include "py_dtype.hpp"
#include "py_dvalue.hpp"
#include "py_dobject.hpp"

template<>
PyTypeObject* PyDTypeT::pyType = NULL;

PyDType::PyDType()
{
  Destruct::DType::init();//XXX should not be done here as it allocate object that are already allocated and clean is not called ! 
  pyType = (PyTypeObject*)malloc(sizeof(basePyType));
  memcpy(pyType , &basePyType , sizeof(basePyType));

  pyType->tp_name = "destruct.DType";
  pyType->tp_basicsize = sizeof(PyDType::DPyObject);
  pyType->tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE;
  pyType->tp_doc = "destruct.DType objects";
  pyType->tp_methods = PyDType::pyMethods;
  pyType->tp_methods = pyMethods;
  pyType->tp_init = (initproc)PyDType::_init;
  pyType->tp_new = _new;
  pyType->tp_dealloc = (destructor)_dealloc;

  if (PyType_Ready(pyType) < 0)
    throw std::string("PyType ready error");
}

PyObject* PyDType::getType(PyDType::DPyObject* self, PyObject*args, PyObject* kwds)
{
  int typeId = self->pimpl->getType();
  return (DValueDispatchTable[typeId]->typeObject());
}

PyMethodDef PyDType::pyMethods[] = 
{
  {"getType", (PyCFunction)PyDType::getType, METH_NOARGS, "Return the type."},
  {NULL}
};

int PyDType::_init(PyDTypeT::DPyObject *self, PyObject *args, PyObject *kwds)
{
   int  typeId = Destruct::DType::DUnknownType;

   if (!PyArg_ParseTuple(args, "i", &typeId))
   {
     return (-1);
   }

   if ((typeId > Destruct::DType::DUnknownType) || (DValueDispatchTable[typeId] == NULL))
   { 
     PyErr_SetString(PyExc_RuntimeError, "Bad type id value");
     return (0);
   }

   self->pimpl = new Destruct::DType((Destruct::DType::Type_t)typeId);
   INIT_CHECK_ALLOC(self->pimpl)
 
   return (0);
}
