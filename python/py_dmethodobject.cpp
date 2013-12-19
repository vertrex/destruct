#include <typeinfo>

#include "destruct.hpp"
#include "dobject.hpp"
#include "drealvalue.hpp"
#include "dnullobject.hpp"
#include "py_dstruct.hpp"
#include "py_dtype.hpp"
#include "py_dmethodobject.hpp"

using Destruct::DUnicodeString;

template<>
PyTypeObject* PyDMethodObjectT::pyType = NULL;

PyDMethodObject::PyDMethodObject()
{
  pyType = (PyTypeObject*)malloc(sizeof(basePyType));
  memcpy(pyType , &basePyType , sizeof(basePyType));

  pyType->tp_name = "destruct.DMethodObject";
  pyType->tp_basicsize = sizeof(PyDMethodObject::DPyObject);
  pyType->tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE;
  pyType->tp_doc = "destruct.DMethodObject objects";
  pyType->tp_methods = PyDMethodObject::pyMethods;
  pyType->tp_init = (initproc)PyDMethodObject::_init;
  pyType->tp_new = PyDMethodObject::_new;
  pyType->tp_dealloc = (destructor)PyDMethodObject::_dealloc;
  pyType->tp_repr = (reprfunc)PyDMethodObject::_repr;
  pyType->tp_call = (ternaryfunc)PyDMethodObject::call;

  if (PyType_Ready(pyType) < 0)
    throw std::string("PyType ready error");
}

PyObject* PyDMethodObject::call(PyObject* _self, PyObject* args)
{
  PyDMethodObject::DPyObject* self = (PyDMethodObject::DPyObject*) _self;

  if (self->dobject == NULL || self->pimpl == NULL)
  {
    PyErr_SetString(PyExc_RuntimeError, "Private implementation pointer's is NULL.");
    return (0);
  } 
 
//Optim PyObjectCall("getType"); //si le type est deja connue on peut le convertir direct ? c koi le plus rapide ?
  PyObject* argumentObject = NULL;
  Destruct::DType type = self->dobject->instanceOf()->attribute(self->index).type();
  Destruct::DType::Type_t argumentTypeId = type.getArgumentType();  
  Destruct::DType::Type_t returnTypeId = type.getReturnType();

  if (argumentTypeId != Destruct::DType::DNoneType && !PyArg_ParseTuple(args, "O", &argumentObject))
  {
    std::string argumentString = "must be a " + type.argumentName();
    PyErr_SetString(PyExc_TypeError, argumentString.c_str());
    return (0);
  }

  DPythonMethodObject* pyMethod = dynamic_cast<DPythonMethodObject* >(self->pimpl);
  if (pyMethod)
    return (pyMethod->fastCall(argumentObject)); //check return ? if compatible ? permet de call avec n importe koi comme une variant ?
  
  try
  {
    return (DValueDispatchTable[returnTypeId]->asDValue(self->pimpl->call(DValueDispatchTable[argumentTypeId]->toDValue(argumentObject))));
  } 
  catch (std::string error)
  {  
    PyErr_SetString(PyExc_TypeError, error.c_str());
  }
  catch (std::bad_cast exception)
  {
    std::string argumentString = "must be a " + type.argumentName();
    PyErr_SetString(PyExc_TypeError, argumentString.c_str()); 
  }
  return (0);
}

/*
 *  self is the attributeObject passed by call
 *  args is tuple with first argument PyDObject::call, self argument
 */

PyMethodDef PyDMethodObject::pyMethods[] = 
{
  {"getType",  (PyCFunction)getType, METH_CLASS, "Return self DType::type."},
  {"call", (PyCFunction)call, METH_VARARGS, "Call method."},
  { NULL }
};

int PyDMethodObject::_init(PyDMethodObject::DPyObject* self, PyObject *args, PyObject *kwds)
{
  self->pimpl = NULL;
  printf("init object\n");
 
  return (0);
}

void PyDMethodObject::_dealloc(PyDMethodObject::DPyObject* self)
{
  if (self->pimpl)
  {
    self->pimpl->destroy();
    self->pimpl = NULL;
  }
  self->ob_type->tp_free((PyObject*)self);
}

PyObject* PyDMethodObject::_repr(PyDMethodObject::DPyObject* self)
{
  CHECK_PIMPL
  if (self->dobject == NULL)
  {
    PyErr_SetString(PyExc_RuntimeError, "Private implementation pointer's is NULL.");\
    return (0);
  }

  const Destruct::DStruct* dstruct =  self->dobject->instanceOf();
  Destruct::DAttribute attribute = dstruct->attribute(self->index);
  Destruct::DType  type = attribute.type();

  std::string methodName = type.returnName() + " " + dstruct->name() + "::" + attribute.name() + "(" + type.argumentName() + ")";

  return (PyString_FromString(methodName.c_str()));
}

PyObject* PyDMethodObject::typeObject()
{
  Py_INCREF((PyObject*)pyType);
  return ((PyObject*)pyType);
}

PyObject* PyDMethodObject::getType(PyDMethodObject::DPyObject* self, PyObject* args, PyObject* kwds)
{
  return (PyInt_FromSize_t(Destruct::DType::DMethodType)); 
}

/*   
 *  DPythonMethodObject
 */ 

Destruct::DValue DPythonMethodObject::call(Destruct::DValue const& args) const
{

  PyGILState_STATE gstate;
  gstate = PyGILState_Ensure();

  PyObject* pythonResult = NULL;
  Destruct::DType::Type_t argumentType = this->__type.getArgumentType();

  if (argumentType != Destruct::DType::DNoneType)
  {
    PyObject* pyargs = DValueDispatchTable[argumentType]->asDValue(args);
    pythonResult = PyObject_CallFunctionObjArgs(this->__pythonCallable, this->__self, pyargs, NULL);
  }
  else
    pythonResult = PyObject_CallFunctionObjArgs(this->__pythonCallable, this->__self, NULL, NULL);

  if (!pythonResult)
  {
     
     const std::string error = PythonTypeBaseModule::pyErrorAsString();
     std::cout << error << std::endl;
     throw error;
  }

  PyGILState_Release(gstate);
  return DValueDispatchTable[this->__type.getReturnType()]->toDValue(pythonResult);
}

PyObject*       DPythonMethodObject::fastCall(PyObject * args) const
{
  return PyObject_CallFunctionObjArgs(this->__pythonCallable, this->__self, args, NULL);
}
