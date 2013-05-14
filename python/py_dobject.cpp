#include "py_dobject.hpp"
#include "destruct.hpp"
#include "dobject.hpp"
#include "drealvalue.hpp"
#include "py_dstruct.hpp"
#include "py_dtype.hpp"
#include "dfuncvalue.hpp"

using Destruct::DUnicodeString;

template<>
PyTypeObject* PyDObjectT::pyType = NULL;

PyDObject::PyDObject()
{
  pyType = (PyTypeObject*)malloc(sizeof(basePyType));
  memcpy(pyType , &basePyType , sizeof(basePyType));

  pyType->tp_name = "destruct.DObject";
  pyType->tp_basicsize = sizeof(DPyObject);
  pyType->tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE;
  pyType->tp_doc = "destruct.DObject objects";
  pyType->tp_methods = PyDObject::pyMethods;
  pyType->tp_methods = pyMethods;
  pyType->tp_init = (initproc)PyDObject::_init;
  pyType->tp_new = PyDObject::_new;
  pyType->tp_dealloc = (destructor)PyDObject::_dealloc;
  pyType->tp_repr = (reprfunc)PyDObject::_repr;
  pyType->tp_getattro = (getattrofunc)PyDObject::_getattr;
  pyType->tp_setattro = (setattrofunc)PyDObject::_setattr;
  pyType->tp_compare = (cmpfunc)PyDObject::_compare; 
 
  if (PyType_Ready(pyType) < 0)
    throw std::string("PyType ready error");
}

PyObject* PyDObject::instanceOf(PyDObject::DPyObject* self)
{
  CHECK_PIMPL
  const Destruct::DStruct* dstruct = self->pimpl->instanceOf();

  PyDStruct::DPyObject* dstructObject = (PyDStruct::DPyObject*)_PyObject_New(PyDStruct::pyType);
  dstructObject->pimpl = (Destruct::DStruct*)dstruct;
   
  return ((PyObject*)dstructObject);
}
PyObject* PyDObject::getValue(PyDObject::DPyObject* self, PyObject* attributeObject)
{
  int32_t     attributeIndex = 0;
  const char* attributeName = NULL;


  CHECK_PIMPL
  if (PyInt_Check(attributeObject))
  {
    attributeIndex = PyLong_AsLong(attributeObject);
  }
  else if (PyString_Check(attributeObject))
  {
    attributeName = PyString_AsString(attributeObject); 
    attributeIndex = self->pimpl->instanceOf()->findAttribute(std::string(attributeName));
  }
  else
  {
    PyErr_SetString(PyExc_TypeError, "must be string or integer");
    return (NULL);
  }

  if (attributeIndex == -1 || attributeIndex >= (int32_t)self->pimpl->instanceOf()->attributeCount())
  {
    std::string errorString = "GET destruct.DObject." + self->pimpl->instanceOf()->name() +  " instance as no attribute '" + attributeName + "'";
    PyErr_SetString(PyExc_AttributeError, errorString.c_str()); 
    return (0);
  }

  size_t typeId = self->pimpl->instanceOf()->attribute(attributeIndex).type().getType();
  if ((typeId > Destruct::DType::DUnknownType) || (DValueDispatchTable[typeId] == NULL))
  { 
    PyErr_SetString(PyExc_RuntimeError, "Bad type id value");
    return (0);
  }

  return (DValueDispatchTable[typeId]->asPyObject((PyObject*)self, attributeIndex));
}

PyObject* PyDObject::setValueObject(PyDObject::DPyObject* self, PyObject* args, PyObject* kwds)
{
  PyObject*    valueObject;
  PyObject*    attributeObject;
  int32_t      attributeIndex = 0; 
  const char*  attributeName = NULL;

  if (!PyArg_ParseTuple(args, "OO", &attributeObject, &valueObject))
  {
    PyErr_SetString(PyExc_TypeError, "must be string or integer, and a compatible DType or Python Object");
    return (0);
  }
  CHECK_PIMPL
  if (PyInt_Check(attributeObject))
  {
    attributeIndex = PyLong_AsLong(attributeObject);
    return (PyDObject::setValue(self, attributeIndex, valueObject));
  }
  else if (PyString_Check(attributeObject))
  {
    attributeName = PyString_AsString(attributeObject); 
    return (PyDObject::setValue(self, attributeName, valueObject));
  }
  PyErr_SetString(PyExc_TypeError, "must be string or integer, and a compatible DType or Python Object");
  return (0);
}

PyObject* PyDObject::setValue(PyDObject::DPyObject* self, const char* attributeName, PyObject* valueObject)
{
  int32_t attributeIndex = self->pimpl->instanceOf()->findAttribute(std::string(attributeName));
  if (attributeIndex == -1 || attributeIndex >= (int32_t)self->pimpl->instanceOf()->attributeCount())
  {
    std::string errorString = "destruct.DObject." + self->pimpl->instanceOf()->name() +  " instance as no attribute '" + attributeName + "'";
    PyErr_SetString(PyExc_AttributeError, errorString.c_str()); 
    return (0);
  }

  return (PyDObject::setValue(self, attributeIndex, valueObject));
}

PyObject*  PyDObject::setValue(PyDObject::DPyObject* self, int32_t attributeIndex, PyObject* valueObject)
{
  if (attributeIndex == -1 || attributeIndex >= (int32_t)self->pimpl->instanceOf()->attributeCount())
  {
    std::string errorString = "destruct.DObject." + self->pimpl->instanceOf()->name() +  " instance as no attribute at index '"; 
    errorString += attributeIndex + "'";
    PyErr_SetString(PyExc_AttributeError, errorString.c_str()); 
    return (0);
  }
  
  size_t typeId = self->pimpl->instanceOf()->attribute(attributeIndex).type().getType();
  try
  {
    if ((typeId > Destruct::DType::DUnknownType) || (DValueDispatchTable[typeId] == NULL))
    { 
      PyErr_SetString(PyExc_RuntimeError, "Bad type id value");
      return (0);
    }
    self->pimpl->setValue(attributeIndex, DValueDispatchTable[typeId]->toDValue(valueObject));
    Py_RETURN_NONE;
  }
  catch (std::string e)
  {
    PyErr_SetString(PyExc_AttributeError, e.c_str());
    return (0);
  }
}

PyObject* PyDObject::replaceValueObject(PyDObject::DPyObject* self, PyObject* args, PyObject* kwds)
{
  PyObject*    valueObject;
  PyObject*    attributeObject;
  int32_t      attributeIndex = 0; 
  const char*  attributeName = NULL;

  if (!PyArg_ParseTuple(args, "OO", &attributeObject, &valueObject))
  {
    PyErr_SetString(PyExc_TypeError, "must be string or integer, and a compatible DType or Python Object");
    return (0);
  }

  CHECK_PIMPL 
  if (PyInt_Check(attributeObject))
  {
    attributeIndex = PyLong_AsLong(attributeObject);
    return (PyDObject::replaceValue(self, attributeIndex, valueObject));
  }
  else if (PyString_Check(attributeObject))
  {
    attributeName = PyString_AsString(attributeObject);
    return (PyDObject::replaceValue(self, attributeName, valueObject)); 
  }
  else
  {
    PyErr_SetString(PyExc_TypeError, "must be string or integer, and a compatible DType or Python Object");
    return (0);
  }
}

PyObject* PyDObject::replaceValue(PyDObject::DPyObject* self, const char* attributeName, PyObject* valueObject)
{
  int32_t attributeIndex = self->pimpl->instanceOf()->findAttribute(std::string(attributeName));
  if (attributeIndex == -1 || attributeIndex >= (int32_t)self->pimpl->instanceOf()->attributeCount())
  {
    std::string errorString = "destruct.DObject." + self->pimpl->instanceOf()->name() +  " instance as no attribute '" + attributeName + "'";
    PyErr_SetString(PyExc_AttributeError, errorString.c_str()); 
    return (0);
  }
  return (PyDObject::replaceValue(self, attributeIndex, valueObject));
}

PyObject* PyDObject::replaceValue(PyDObject::DPyObject* self, int32_t attributeIndex, PyObject* valueObject)
{
  if (!PyCallable_Check(valueObject)) 
  {
    PyErr_SetString(PyExc_TypeError, "parameter must be callable");
    return (0);
  }
  Py_XINCREF(valueObject);

  if (attributeIndex == -1 || attributeIndex >= (int32_t)self->pimpl->instanceOf()->attributeCount())
  {
    std::string errorString = "destruct.DObject." + self->pimpl->instanceOf()->name() +  " instance as no attribute at index '"; 
    errorString += attributeIndex+ "'";
    PyErr_SetString(PyExc_AttributeError, errorString.c_str()); 
    return (0);
  }

  Destruct::DObject* obj = self->pimpl;
  Destruct::DSimpleObject* simple = dynamic_cast<Destruct::DSimpleObject*>(obj);

  if (simple == NULL)
  {
    PyErr_SetString(PyExc_RuntimeError, "Can't cast private implementation to DSimpleObject");
    return (0); 
  }

  simple->replaceValue(attributeIndex, DPythonFunctionValue<DUnicodeString>(self, valueObject)); 
  Py_RETURN_NONE;
}


PyMethodDef PyDObject::pyMethods[] = 
{
  {"instanceOf", (PyCFunction)instanceOf, METH_NOARGS, "Return the DStruct definition who generate this object."},
  {"getType",  (PyCFunction)getType, METH_CLASS, "Return self DType::type."},
  {"getValue", (PyCFunction)getValue, METH_O, "Get value for attribute (passed as index or string)."},
  {"setValue", (PyCFunction)setValueObject, METH_VARARGS, "Set attribute value."},
  {"replaceValue", (PyCFunction)replaceValueObject, METH_VARARGS, "Replace attribute value."},
  { NULL }
};

int PyDObject::_init(PyDObjectT::DPyObject* self, PyObject *args, PyObject *kwds)
{
  self->pimpl = NULL;
  char* dstructName = NULL;

  if (!PyArg_ParseTuple(args, "s", &dstructName))
  {
    PyErr_SetString(PyExc_TypeError, "must be string or integer, and a compatible DType or Python Object");
    return (-1);
  }
  Destruct::DStruct* dstruct = Destruct::Destruct::instance().find(std::string(dstructName));
  if (dstruct != NULL)
  {
    self->pimpl = dstruct->newObject();
    return (0);
  }

  std::string errorString = "Can't find " + std::string(dstructName) + " in Destruct database";
  PyErr_SetString(PyExc_RuntimeError, errorString.c_str());
  return (-1);
}

void PyDObject::_dealloc(PyDObjectT::DPyObject* self)
{
   if (self->pimpl)
   {
     self->pimpl->destroy();
     self->pimpl = NULL;
   }
   self->ob_type->tp_free((PyObject*)self);
}

PyObject*    PyDObject::_getattr(PyDObject::DPyObject* self, PyObject* name)
{
  PyObject* obj = PyDObject::getValue(self, name);
  if (obj != NULL)
    return obj; 

  PyErr_Clear();
  return (PyObject_GenericGetAttr((PyObject*)self, name));
}

int  PyDObject::_setattr(PyDObject::DPyObject* self, PyObject* name, PyObject *valueObject)
{
  const char* cname = PyString_AsString(name);

  if (PyCallable_Check(valueObject)) 
  {
    PyObject* res = replaceValue(self, cname, valueObject);     
    if (res == Py_None)
      return (0);
    return (-1);
  }
  
  PyObject* res = setValue(self, cname, valueObject);
  if (res == Py_None)
    return (0);
  return (-1);
}

PyObject* PyDObject::_repr(PyDObject::DPyObject* self)
{
  CHECK_PIMPL
  std::string repr = "<destruct.DObject." + self->pimpl->instanceOf()->name() + " object>";
  return (PyString_FromString(repr.c_str()));
}

int PyDObject::_compare(PyDObject::DPyObject* self, PyDObject::DPyObject* other)
{
  /* This only have sense when pointer is equal */
  return ((int)(self->pimpl - other->pimpl));
}

PyObject* PyDObject::typeObject()
{
  Py_INCREF((PyObject*)pyType);
  return ((PyObject*)pyType);
}

PyObject* PyDObject::getType(PyDObject::DPyObject* self, PyObject* args, PyObject* kwds)
{
  return (PyInt_FromSize_t(Destruct::DType::DObjectType)); 
}
