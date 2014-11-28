#include <typeinfo>

#include "destruct.hpp"
#include "dobject.hpp"
#include "drealvalue.hpp"
#include "dnullobject.hpp"
#include "dexception.hpp"

#include "py_dstruct.hpp"
#include "py_dtype.hpp"
#include "py_dmethodobject.hpp"

using Destruct::DUnicodeString;

template<>
PyTypeObject* PyDMethodObjectT::pyType = NULL;

Destruct::DValue PyDMethodObject::toDValue(PyObject* value) 
{
  if (PyObject_TypeCheck(value, PyDMethodObject::pyType))
  {
     return Destruct::RealValue<Destruct::DFunctionObject*>(((DPyObject*)value)->pimpl);
  }
  throw Destruct::DException("Can't cast to DMethodObject*");
}

/* 
 * Only used by py_dmethodobject Call 
 * Should implemented for method returing method but it's not implemented in Destruct now
*/
PyObject*     PyDMethodObject::asDValue(Destruct::DValue const& v)
{
  Py_RETURN_NONE;
}

PyObject*     PyDMethodObject::asPyObject(PyObject* _self, int32_t attributeIndex)
{
  PyDObject::DPyObject* self = (PyDObject::DPyObject*)_self;
  Destruct::DFunctionObject* value = self->pimpl->getValue(attributeIndex).get<Destruct::DFunctionObject*>();
 
  if (value == NULL)
    Py_RETURN_NONE;
// get ici type et sauvegarde le pointeur suffit au lieu de index + dobject (dobject qui est deja le this de l objet non ? en + ) 
//mais le gain de temps est negligeable apparement !
  PyDMethodObject::DPyObject* dmethodobject = (PyDMethodObject::DPyObject*)_PyObject_New(PyDMethodObject::pyType); 
  dmethodobject->pimpl = value;
  dmethodobject->index = attributeIndex;
  dmethodobject->dobject = self->pimpl;
 
  Py_INCREF(dmethodobject);

  return ((PyObject*)dmethodobject);
}

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
    throw Destruct::DException("PyType ready error");
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
    if (argumentTypeId != Destruct::DType::DObjectType)
    {
      argumentObject = NULL;
      std::string argumentString = "MethodObject argument must be a " + type.argumentName();
      PyErr_SetString(PyExc_TypeError, argumentString.c_str());
      return (0);
    }
    PyErr_Clear();
  }

//XXX cast DNone to pyobject ou must give None ? 
  DPythonMethodObject* pyMethod = dynamic_cast<DPythonMethodObject* >(self->pimpl);
  if (pyMethod)
    return (pyMethod->fastCall(argumentObject)); //check return ? if compatible ? permet de call avec n importe koi comme une variant ?

  PyThreadState *_save; 
  _save = PyEval_SaveThread();
  try
  {
    Destruct::DValue result;
    if (argumentTypeId == Destruct::DType::DObjectType && argumentObject == NULL)
    {
      result = self->pimpl->call(Destruct::RealValue<Destruct::DObject*>(Destruct::DNone));
      PyEval_RestoreThread(_save); 
      return (DValueDispatchTable[returnTypeId]->asDValue(result));
    }

    result = self->pimpl->call(DValueDispatchTable[argumentTypeId]->toDValue(argumentObject));
    PyEval_RestoreThread(_save); 
    return (DValueDispatchTable[returnTypeId]->asDValue(result));
  } 
  catch (Destruct::DException const& exception)
  {  
    PyEval_RestoreThread(_save); 
    PyErr_SetString(PyExc_TypeError, exception.error().c_str());
  }
  catch (std::bad_cast exception) //catch in cpp ?
  {
    PyEval_RestoreThread(_save); 
    std::string argumentString = "MethodObject must return a " + type.returnName();
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
 *  DPythonMethodObject : public DFunctionObject called by c++ code 
 */ 
Destruct::DValue DPythonMethodObject::call(void) const
{
  PyGILState_STATE gstate;
  gstate = PyGILState_Ensure();

  PyObject* pythonResult = PyObject_CallFunctionObjArgs(this->__pythonCallable, this->__self, NULL, NULL);

  if (!pythonResult)
  {
     const std::string error = PythonTypeBaseModule::pyErrorAsString();
     throw Destruct::DException(error);
  }

  PyGILState_Release(gstate);
  Destruct::DStruct* dstruct = ((PyDObject::DPyObject*)this->__self)->pimpl->instanceOf();
  Destruct::DType type = dstruct->attribute(this->__attributeIndex).type();

  //return DValueDispatchTable[this->__type.getReturnType()]->toDValue(pythonResult);
  return DValueDispatchTable[type.getReturnType()]->toDValue(pythonResult);
}

Destruct::DValue DPythonMethodObject::call(Destruct::DValue const& args) const
{
  PyGILState_STATE gstate;
  gstate = PyGILState_Ensure();

  PyObject* pythonResult = NULL;
  //Destruct::DType::Type_t argumentType = this->__type.getArgumentType();

//XXX sad but true :( car ca peut eter mutable !! ou alors check if mutable si vraiemnt trop lent !
  //must have if object->isMutable ... c peut etre mieux de l itnegrer directement ...
  //ca veut pas autant dire ds dobject on peut juste avoir une methode virtuel isMutable
  Destruct::DStruct* dstruct = ((PyDObject::DPyObject*)this->__self)->pimpl->instanceOf();
  Destruct::DType type = dstruct->attribute(this->__attributeIndex).type();
  Destruct::DType::Type_t argumentType = type.getArgumentType();

  if (argumentType != Destruct::DType::DNoneType)
  {
    PyObject* pyargs = DValueDispatchTable[argumentType]->asDValue(args);
    pythonResult = PyObject_CallFunctionObjArgs(this->__pythonCallable, this->__self, pyargs, NULL);
  }
  else
  { 
    pythonResult = PyObject_CallFunctionObjArgs(this->__pythonCallable, this->__self, NULL, NULL);
  }

  if (!pythonResult)
  {
     const std::string error = PythonTypeBaseModule::pyErrorAsString();
     throw Destruct::DException(error);
  }

  PyGILState_Release(gstate);
  return DValueDispatchTable[type.getReturnType()]->toDValue(pythonResult);
}

PyObject*       DPythonMethodObject::fastCall(PyObject * args) const
{
  return PyObject_CallFunctionObjArgs(this->__pythonCallable, this->__self, args, NULL);
}
