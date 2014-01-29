#include "dexception.hpp"

#include "py_dnullobject.hpp"

template<>
PyTypeObject* PyDNullObjectT::pyType = NULL;

PyDNullObject::PyDNullObject()
{
  pyType = (PyTypeObject*)malloc(sizeof(basePyType));
  memcpy(pyType , &basePyType , sizeof(basePyType));

  pyType->tp_name = "destruct.DNone"; //XXX PYDNone ? en python singleton plutot que module/class -> directement acessible plus simple ? voir comment on fait je crois qu on la fait ailleurs pour destruct oue je c pu quoi !
  pyType->tp_basicsize = sizeof(DPyObject);
  pyType->tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE; //XXX SINGELTONE like Destruct() car DNone ? et pas DNullbojecT???
  pyType->tp_doc = "destruct.DNone objects";
  //pyType->tp_methods = PyDNullObject::pyMethods;
  pyType->tp_methods = pyMethods;
  pyType->tp_init = (initproc)PyDNullObject::_init;
  pyType->tp_new = PyDNullObject::_new;
  //pyType->tp_dealloc = (destructor)PyDNullObject::_dealloc;
  //pyType->tp_repr = (reprfunc)PyDNullObject::_repr;
  //pyType->tp_getattro = (getattrofunc)PyDNullObject::_getattr;
  //pyType->tp_setattro = (setattrofunc)PyDNullObject::_setattr;
  //pyType->tp_compare = (cmpfunc)PyDNullObject::_compare; 

  if (PyType_Ready(pyType) < 0)
    throw Destruct::DException("PyType ready error");
}

PyMethodDef PyDNullObject::pyMethods[] = 
{
        //{"instanceOf", (PyCFunction)instanceOf, METH_NOARGS, "Return the DStruct definition who generate this object."},
  {"getType",  (PyCFunction)getType, METH_CLASS, "Return self DType::type."},
////{"getValue", (PyCFunction)getValue, METH_O, "Get value for attribute (passed as index or string)."},
////{"setValue", (PyCFunction)setValueObject, METH_VARARGS, "Set attribute value."},
  { NULL }
};

int PyDNullObject::_init(PyDNullObjectT::DPyObject* self, PyObject *args, PyObject *kwds)
{
  self->pimpl = Destruct::DNone;
  self->pimpl->addRef();
  return (0);
}

void PyDNullObject::_dealloc(PyDNullObjectT::DPyObject* self)
{
   self->pimpl->delRef();
   self->ob_type->tp_free((PyObject*)self);
}

PyObject* PyDNullObject::typeObject()
{
  Py_INCREF((PyObject*)pyType);
  return ((PyObject*)pyType);
}

PyObject* PyDNullObject::getType(PyDNullObject::DPyObject* self, PyObject* args, PyObject* kwds)
{
  return (PyInt_FromSize_t(Destruct::DType::DNoneType));
}
