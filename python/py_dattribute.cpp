#include "dstruct.hpp"
#include "dsimpleobject.hpp"
#include "py_dattribute.hpp"
#include "py_dtype.hpp"
#include "dattribute.hpp"

template<>
PyTypeObject* PyDAttributeT::pyType = NULL;

PyDAttribute::PyDAttribute()
{
  pyType = (PyTypeObject*)malloc(sizeof(basePyType));
  memcpy(pyType , &basePyType , sizeof(basePyType));

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
    throw std::string("PyType ready error");
}

PyObject* PyDAttribute::name(PyDAttribute::DPyObject* self, PyObject* args, PyObject* kwds)
{
  return (Py_BuildValue("s", self->pimpl->name().c_str())); 
}

PyObject* PyDAttribute::dtype(PyDAttribute::DPyObject* self, PyObject*args, PyObject* kwds)
{
  Destruct::DType* dtype = new Destruct::DType(self->pimpl->type());
  CHECK_ALLOC(dtype) 

  PyDType::DPyObject* dtypeObject = (PyDType::DPyObject*) _PyObject_New(PyDType::pyType);
  dtypeObject->pimpl = dtype;

  return (Py_BuildValue("O", dtypeObject));
}

PyMethodDef PyDAttribute::pyMethods[] = 
{
  {"name", (PyCFunction)PyDAttribute::name, METH_NOARGS, "Return the name of the attribute."},
  {"dtype", (PyCFunction)PyDAttribute::dtype, METH_NOARGS, "Return the type of the attribute."},
  {NULL}
};

int PyDAttribute::_init(PyDAttributeT::DPyObject *self, PyObject *args, PyObject *kwds)
{
   int                          typeId = Destruct::DType::DUnknownType;
   const char*                  name = NULL;
   PyObject*                    dtypeObject;

   if (PyArg_ParseTuple(args, "sO", &name, &dtypeObject))
   {
     if (!PyType_Check(dtypeObject))
       return (-1);

     PyObject* objectTypeId = PyObject_CallMethod(dtypeObject, (char *)"getType", NULL);

     typeId = PyInt_AsLong(objectTypeId);
     self->pimpl = new Destruct::DAttribute(std::string(name), (Destruct::DType::Type_t)typeId);
     INIT_CHECK_ALLOC(self->pimpl)

     return (0);
   }

   return (-1);
}
