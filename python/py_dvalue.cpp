#include "py_dvalue.hpp"

template<>
PyTypeObject* PyDInt8T::pyType = NULL;

PyDInt8::PyDInt8()
{
  pyType = (PyTypeObject*)malloc(sizeof(basePyType));
  memcpy(pyType , &basePyType , sizeof(basePyType));

  PyMethodDef* pyMethods = (PyMethodDef*)malloc(sizeof(baseTypePyMethods));
  memcpy(pyMethods , &baseTypePyMethods , sizeof(baseTypePyMethods));
  pyMethods[0].ml_meth = (PyCFunction)this->getType;

  pyType->tp_base = &PyInt_Type;
  pyType->tp_name = "destruct.DInt8";
  pyType->tp_basicsize = sizeof(DPyObject);
  pyType->tp_init = (initproc)(PyDInt8::_init);
  pyType->tp_methods = pyMethods;
  if (PyType_Ready(pyType) < 0)
    throw std::string("PyType ready error");
}

Destruct::DValue PyDInt8::toDValue(PyObject* value) 
{
  if (PyInt_Check(value))
  {
    DInt8  fvalue = PyLong_AsLong(value);
    return Destruct::RealValue<DInt8>(fvalue);
  }
  throw std::string("Can't cast to DInt8");
}

PyObject*     PyDInt8::asPyObject(PyObject* self, int32_t attributeIndex)
{
  DInt8 value = ((PyDObject::DPyObject*)self)->pimpl->getValue(attributeIndex).get<DInt8>();
  return (PyObject_CallObject((PyObject*)PyDInt8::pyType  , Py_BuildValue("(O)", PyInt_FromLong(value))));
}

template<>
PyTypeObject* PyDInt16T::pyType = NULL;

PyDInt16::PyDInt16()
{
  pyType = (PyTypeObject*)malloc(sizeof(basePyType));
  memcpy(pyType , &basePyType , sizeof(basePyType));

  PyMethodDef* pyMethods = (PyMethodDef*)malloc(sizeof(baseTypePyMethods));
  memcpy(pyMethods , &baseTypePyMethods , sizeof(baseTypePyMethods));
  pyMethods[0].ml_meth = (PyCFunction)this->getType;

  pyType->tp_base = &PyInt_Type;
  pyType->tp_name = "destruct.DInt16";
  pyType->tp_basicsize = sizeof(DPyObject);
  pyType->tp_init = (initproc)(PyDInt16::_init);
  pyType->tp_methods = pyMethods;
  if (PyType_Ready(pyType) < 0)
    throw std::string("PyType ready error");
}

Destruct::DValue PyDInt16::toDValue(PyObject* value) 
{
  if (PyInt_Check(value))
  {
    DInt16  fvalue = PyLong_AsLong(value);
    return (Destruct::RealValue<DInt16>(fvalue));
  }
  throw std::string("Can't cast to DInt16");
}

PyObject*     PyDInt16::asPyObject(PyObject* self, int32_t attributeIndex)
{
  DInt16 value = ((PyDObject::DPyObject*)self)->pimpl->getValue(attributeIndex).get<DInt16>();
  return (PyObject_CallObject((PyObject*)PyDInt16::pyType  , Py_BuildValue("(O)", PyInt_FromLong(value))));
}

template<>
PyTypeObject* PyDInt32T::pyType = NULL;

PyDInt32::PyDInt32()
{
  pyType = (PyTypeObject*)malloc(sizeof(basePyType));
  memcpy(pyType , &basePyType , sizeof(basePyType));

  PyMethodDef* pyMethods = (PyMethodDef*)malloc(sizeof(baseTypePyMethods));
  memcpy(pyMethods , &baseTypePyMethods , sizeof(baseTypePyMethods));
  pyMethods[0].ml_meth = (PyCFunction)this->getType;

  pyType->tp_base = &PyInt_Type;
  pyType->tp_name = "destruct.DInt32";
  pyType->tp_basicsize = sizeof(DPyObject);
  pyType->tp_init = (initproc)(PyDInt32::_init);
  pyType->tp_methods = pyMethods;
  if (PyType_Ready(pyType) < 0)
    throw std::string("PyType ready error");
}

Destruct::DValue PyDInt32::toDValue(PyObject* value) 
{
  if (PyInt_Check(value))
  {
      DInt32  fvalue = PyLong_AsLong(value);
      return (Destruct::RealValue<DInt32>(fvalue));
  }
  throw std::string("Can't cast to DInt32");
}

PyObject*        PyDInt32::asPyObject(PyObject* self, int32_t attributeIndex)
{
  DInt32 value = ((PyDObject::DPyObject*)self)->pimpl->getValue(attributeIndex).get<DInt32>();
  return (PyObject_CallObject((PyObject*)PyDInt32::pyType  , Py_BuildValue("(O)", PyInt_FromLong(value))));
}

template<>
PyTypeObject* PyDInt64T::pyType = NULL;

PyDInt64::PyDInt64()
{
  pyType = (PyTypeObject*)malloc(sizeof(PyDInt64::basePyType));
  memcpy(pyType , &basePyType , sizeof(PyDInt64::basePyType));

  PyMethodDef* pyMethods = (PyMethodDef*)malloc(sizeof(baseTypePyMethods));
  memcpy(pyMethods , &baseTypePyMethods , sizeof(baseTypePyMethods));
  pyMethods[0].ml_meth = (PyCFunction)this->getType;

  pyType->tp_base = &PyLong_Type;
  pyType->tp_name = "destruct.DInt64";
  pyType->tp_basicsize = sizeof(DPyObject);
  pyType->tp_init = (initproc)(PyDInt64::_init);
  pyType->tp_methods = pyMethods;
  if (PyType_Ready(pyType) < 0)
    throw std::string("PyType ready error");
}

Destruct::DValue PyDInt64::toDValue(PyObject* value) 
{
  if (PyLong_Check(value) || PyInt_Check(value))
  {
    DInt64 fvalue = PyLong_AsLong(value);
    return (Destruct::RealValue<DInt64>(fvalue));
  }
  throw std::string("Can't cast to DInt64");
}

PyObject*     PyDInt64::asPyObject(PyObject* self, int32_t attributeIndex)
{
  DInt64 value = ((PyDObject::DPyObject*)self)->pimpl->getValue(attributeIndex).get<DInt64>();
  return (PyObject_CallObject((PyObject*)PyDInt64::pyType  , Py_BuildValue("(O)", PyLong_FromLong(value))));
}

template<>
PyTypeObject* PyDUInt8T::pyType = NULL;

PyDUInt8::PyDUInt8()
{
  pyType = (PyTypeObject*)malloc(sizeof(basePyType));
  memcpy(pyType , &basePyType , sizeof(basePyType));

  PyMethodDef* pyMethods = (PyMethodDef*)malloc(sizeof(baseTypePyMethods));
  memcpy(pyMethods , &baseTypePyMethods , sizeof(baseTypePyMethods));
  pyMethods[0].ml_meth = (PyCFunction)this->getType;

  pyType->tp_base = &PyInt_Type;
  pyType->tp_name = "destruct.DUInt8";
  pyType->tp_basicsize = sizeof(DPyObject);
  pyType->tp_init = (initproc)(PyDUInt8::_init);
  pyType->tp_methods = pyMethods;
  if (PyType_Ready(pyType) < 0)
    throw std::string("PyType ready error");
}

Destruct::DValue PyDUInt8::toDValue(PyObject* value) 
{
  if (PyInt_Check(value))
  {
    DUInt8  fvalue = PyLong_AsLong(value);
    return Destruct::RealValue<DUInt8>(fvalue);
  }
  throw std::string("Can't cast to DUInt8");
}

PyObject*     PyDUInt8::asPyObject(PyObject* self, int32_t attributeIndex)
{
  DUInt8 value = ((PyDObject::DPyObject*)self)->pimpl->getValue(attributeIndex).get<DUInt8>();
  return (PyObject_CallObject((PyObject*)PyDUInt8::pyType  , Py_BuildValue("(O)", PyInt_FromLong(value))));
}

template<>
PyTypeObject* PyDUInt16T::pyType = NULL;

PyDUInt16::PyDUInt16()
{
  pyType = (PyTypeObject*)malloc(sizeof(basePyType));
  memcpy(pyType , &basePyType , sizeof(basePyType));

  PyMethodDef* pyMethods = (PyMethodDef*)malloc(sizeof(baseTypePyMethods));
  memcpy(pyMethods , &baseTypePyMethods , sizeof(baseTypePyMethods));
  pyMethods[0].ml_meth = (PyCFunction)this->getType;

  pyType->tp_base = &PyInt_Type;
  pyType->tp_name = "destruct.DUInt16";
  pyType->tp_basicsize = sizeof(DPyObject);
  pyType->tp_init = (initproc)(PyDUInt16::_init);
  pyType->tp_methods = pyMethods;
  if (PyType_Ready(pyType) < 0)
    throw std::string("PyType ready error");
}

Destruct::DValue PyDUInt16::toDValue(PyObject* value) 
{
  if (PyInt_Check(value))
  {
    DUInt16  fvalue = PyLong_AsLong(value);
    return (Destruct::RealValue<DUInt16>(fvalue));
  }
  throw std::string("Can't cast to DUInt16");
}

PyObject*     PyDUInt16::asPyObject(PyObject* self, int32_t attributeIndex)
{
  DUInt16 value = ((PyDObject::DPyObject*)self)->pimpl->getValue(attributeIndex).get<DUInt16>();
  return (PyObject_CallObject((PyObject*)PyDUInt16::pyType  , Py_BuildValue("(O)", PyInt_FromLong(value))));
}

template<>
PyTypeObject* PyDUInt32T::pyType = NULL;

PyDUInt32::PyDUInt32()
{
  pyType = (PyTypeObject*)malloc(sizeof(basePyType));
  memcpy(pyType , &basePyType , sizeof(basePyType));

  PyMethodDef* pyMethods = (PyMethodDef*)malloc(sizeof(baseTypePyMethods));
  memcpy(pyMethods , &baseTypePyMethods , sizeof(baseTypePyMethods));
  pyMethods[0].ml_meth = (PyCFunction)this->getType;

  pyType->tp_base = &PyInt_Type;
  pyType->tp_name = "destruct.DUInt32";
  pyType->tp_basicsize = sizeof(DPyObject);
  pyType->tp_init = (initproc)(PyDUInt32::_init);
  pyType->tp_methods = pyMethods;
  if (PyType_Ready(pyType) < 0)
    throw std::string("PyType ready error");
}

Destruct::DValue PyDUInt32::toDValue(PyObject* value) 
{
  if (PyInt_Check(value))
  {
    DUInt32  fvalue = PyLong_AsLong(value);
    return (Destruct::RealValue<DUInt32>(fvalue));
  }
  throw std::string("Can't cast to DUInt32");
}

PyObject*     PyDUInt32::asPyObject(PyObject* self, int32_t attributeIndex)
{
  DUInt32 value = ((PyDObject::DPyObject*)self)->pimpl->getValue(attributeIndex).get<DUInt32>();
  return (PyObject_CallObject((PyObject*)PyDUInt32::pyType  , Py_BuildValue("(O)", PyInt_FromLong(value))));
}

template<>
PyTypeObject* PyDUInt64T::pyType = NULL;

PyDUInt64::PyDUInt64()
{
  pyType = (PyTypeObject*)malloc(sizeof(PyDUInt64::basePyType));
  memcpy(pyType , &basePyType , sizeof(PyDUInt64::basePyType));

  PyMethodDef* pyMethods = (PyMethodDef*)malloc(sizeof(baseTypePyMethods));
  memcpy(pyMethods , &baseTypePyMethods , sizeof(baseTypePyMethods));
  pyMethods[0].ml_meth = (PyCFunction)this->getType;

  pyType->tp_base = &PyLong_Type;
  pyType->tp_name = "destruct.DInt64";
  pyType->tp_basicsize = sizeof(DPyObject);
  pyType->tp_init = (initproc)(PyDUInt64::_init);
  pyType->tp_methods = pyMethods;
  if (PyType_Ready(pyType) < 0)
    throw std::string("PyType ready error");
}

Destruct::DValue PyDUInt64::toDValue(PyObject* value) 
{
  if (PyLong_Check(value) || PyInt_Check(value))
  {
      DUInt64 fvalue = PyLong_AsLong(value);
      return (Destruct::RealValue<DUInt64>(fvalue));
  }
  throw std::string("Can't cast to DUInt64");
}

PyObject*     PyDUInt64::asPyObject(PyObject* self, int32_t attributeIndex)
{
  DUInt64 value = ((PyDObject::DPyObject*)self)->pimpl->getValue(attributeIndex).get<DUInt64>();
  return (PyObject_CallObject((PyObject*)PyDUInt64::pyType  , Py_BuildValue("(O)", PyLong_FromLong(value))));
}

template<>
PyTypeObject* PyDUnicodeStringT::pyType = NULL;

PyDUnicodeString::PyDUnicodeString()
{
  pyType = (PyTypeObject*)malloc(sizeof(basePyType));
  memcpy(pyType , &basePyType , sizeof(basePyType));

  PyMethodDef* pyMethods = (PyMethodDef*)malloc(sizeof(baseTypePyMethods));
  memcpy(pyMethods , &baseTypePyMethods , sizeof(baseTypePyMethods));
  pyMethods[0].ml_meth = (PyCFunction)this->getType;

  pyType->tp_base = &PyString_Type;
  pyType->tp_name = "destruct.DUnicodeString";
  pyType->tp_basicsize = sizeof(DPyObject);
  pyType->tp_init = (initproc)(PyDUnicodeString::_init);
  pyType->tp_methods = pyMethods;
  if (PyType_Ready(pyType) < 0)
    throw std::string("PyType ready error");
}

Destruct::DValue PyDUnicodeString::toDValue(PyObject* value) 
{
  if (PyString_Check(value))
  {
    Destruct::DUnicodeString fvalue = std::string(PyString_AsString(value));
    return Destruct::RealValue<Destruct::DUnicodeString>(fvalue);
  }
  throw std::string("Can't cast to DUnicodeString");
}

PyObject*     PyDUnicodeString::asPyObject(PyObject* self, int32_t attributeIndex)
{
  Destruct::DUnicodeString value = ((PyDObject::DPyObject*)self)->pimpl->getValue(attributeIndex).get<Destruct::DUnicodeString>();
  return (PyObject_CallObject((PyObject*)PyDUnicodeString::pyType  , Py_BuildValue("(O)", PyString_FromString(value.c_str()))));
}
