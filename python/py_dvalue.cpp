#include "py_dvalue.hpp"

#define FAST_CONVERSION true 

PyDInt8::PyDInt8()
{
  PyTypeObject* pyType = PyDInt8::pyType();
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
    throw Destruct::DException("PyType ready error");
}

Destruct::DValue PyDInt8::toDValue(PyObject* value) 
{
  if (PyInt_Check(value))
  {
    DInt8  fvalue = (DInt8)PyLong_AsLong(value);
    return Destruct::RealValue<DInt8>(fvalue);
  }
  throw Destruct::DException(CAST_ERROR(DInt8));
}

PyObject*     PyDInt8::asDValue(Destruct::DValue const& v)
{
  DInt8 value = v;
#ifdef FAST_CONVERSION
  return (PyInt_FromLong(value));
#else 
  return (PyObject_CallObject((PyObject*)PyDInt8::pyType  , Py_BuildValue("(O)", PyInt_FromLong(value))));
#endif
}

PyObject*     PyDInt8::asPyObject(PyObject* self, int32_t attributeIndex)
{
  DInt8 value = ((PyDObject::DPyObject*)self)->pimpl->getValue(attributeIndex);
#ifdef FAST_CONVERSION
  return (PyInt_FromLong(value));
#else
  return (PyObject_CallObject((PyObject*)PyDInt8::pyType  , Py_BuildValue("(O)", PyInt_FromLong(value))));
#endif
}

PyDInt16::PyDInt16()
{
  PyTypeObject* pyType = PyDInt16::pyType();
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
    throw Destruct::DException("PyType ready error");
}

Destruct::DValue PyDInt16::toDValue(PyObject* value) 
{
  if (PyInt_Check(value))
  {
    DInt16  fvalue = (DInt16)PyLong_AsLong(value);
    return (Destruct::RealValue<DInt16>(fvalue));
  }
  throw Destruct::DException(CAST_ERROR(DInt16));
}

PyObject*     PyDInt16::asDValue(Destruct::DValue const& v)
{
  DInt16 value = v;
#ifdef FAST_CONVERSION
  return (PyInt_FromLong(value));
#else
  return (PyObject_CallObject((PyObject*)PyDInt16::pyType  , Py_BuildValue("(O)", PyInt_FromLong(value))));
#endif
}

PyObject*     PyDInt16::asPyObject(PyObject* self, int32_t attributeIndex)
{
  DInt16 value = ((PyDObject::DPyObject*)self)->pimpl->getValue(attributeIndex);
#ifdef FAST_CONVERSION
  return (PyInt_FromLong(value));
#else
  return (PyObject_CallObject((PyObject*)PyDInt16::pyType  , Py_BuildValue("(O)", PyInt_FromLong(value))));
#endif
}

PyDInt32::PyDInt32()
{
  PyTypeObject* pyType = PyDInt32::pyType();
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
    throw Destruct::DException("PyType ready error");
}

Destruct::DValue PyDInt32::toDValue(PyObject* value) 
{
  if (PyInt_Check(value))
  {
      DInt32  fvalue = PyLong_AsLong(value);
      return (Destruct::RealValue<DInt32>(fvalue));
  }
  throw Destruct::DException(CAST_ERROR(DInt32));
}

PyObject*        PyDInt32::asDValue(Destruct::DValue const& v)
{
  DInt32 value = v;
#ifdef FAST_CONVERSION
  return PyInt_FromLong(value);
#else
  return (PyObject_CallObject((PyObject*)PyDInt32::pyType  , Py_BuildValue("(O)", PyInt_FromLong(value))));
#endif
}

PyObject*        PyDInt32::asPyObject(PyObject* self, int32_t attributeIndex)
{
  DInt32 value = ((PyDObject::DPyObject*)self)->pimpl->getValue(attributeIndex);
#ifdef FAST_CONVERSION
  return (PyInt_FromLong(value));
#else
  return (PyObject_CallObject((PyObject*)PyDInt32::pyType  , Py_BuildValue("(O)", PyInt_FromLong(value))));
#endif
}

PyDInt64::PyDInt64()
{
  PyTypeObject* pyType = PyDInt64::pyType();
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
    throw Destruct::DException("PyType ready error");
}

Destruct::DValue PyDInt64::toDValue(PyObject* value) 
{
  if (PyLong_Check(value) || PyInt_Check(value))
  {
    DInt64 fvalue = PyLong_AsLong(value);
    return (Destruct::RealValue<DInt64>(fvalue));
  }
  throw Destruct::DException(CAST_ERROR(DInt64));
}

PyObject*     PyDInt64::asDValue(Destruct::DValue const& v)
{
  DInt64 value = v;
#ifdef FAST_CONVERSION
  return (PyLong_FromLong((long)value));
#else
  return (PyObject_CallObject((PyObject*)PyDInt64::pyType  , Py_BuildValue("(O)", PyLong_FromLong(value))));
#endif
}

PyObject*     PyDInt64::asPyObject(PyObject* self, int32_t attributeIndex)
{
  DInt64 value = ((PyDObject::DPyObject*)self)->pimpl->getValue(attributeIndex);
#ifdef FAST_CONVERSION
  return (PyLong_FromLong((long)value));
#else
  return (PyObject_CallObject((PyObject*)PyDInt64::pyType  , Py_BuildValue("(O)", PyLong_FromLong(value))));
#endif
}

PyDUInt8::PyDUInt8()
{
  PyTypeObject* pyType = PyDUInt8::pyType();
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
    throw Destruct::DException("PyType ready error");
}

Destruct::DValue PyDUInt8::toDValue(PyObject* value) 
{
  if (PyInt_Check(value))
  {
    DUInt8  fvalue = (DUInt8)PyLong_AsLong(value);
    return Destruct::RealValue<DUInt8>(fvalue);
  }
  throw Destruct::DException(CAST_ERROR(DUInt8));
}

PyObject*     PyDUInt8::asDValue(Destruct::DValue const& v)
{
  DUInt8 value = v;
#ifdef FAST_CONVERSION
  return (PyInt_FromLong((long)value));
#else
  return (PyObject_CallObject((PyObject*)PyDUInt8::pyType  , Py_BuildValue("(O)", PyInt_FromLong(value))));
#endif
}

PyObject*     PyDUInt8::asPyObject(PyObject* self, int32_t attributeIndex)
{
  DUInt8 value = ((PyDObject::DPyObject*)self)->pimpl->getValue(attributeIndex);
#ifdef FAST_CONVERSION
  return (PyInt_FromLong(value));
#else
  return (PyObject_CallObject((PyObject*)PyDUInt8::pyType  , Py_BuildValue("(O)", PyInt_FromLong(value))));
#endif
}

PyDUInt16::PyDUInt16()
{
  PyTypeObject* pyType = PyDUInt16::pyType();
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
    throw Destruct::DException("PyType ready error");
}

Destruct::DValue PyDUInt16::toDValue(PyObject* value) 
{
  if (PyInt_Check(value))
  {
    DUInt16  fvalue = (DUInt16)PyLong_AsLong(value);
    return (Destruct::RealValue<DUInt16>(fvalue));
  }
  throw Destruct::DException(CAST_ERROR(DUInt16));
}

PyObject*     PyDUInt16::asDValue(Destruct::DValue const& v)
{
  DUInt16 value = v;
#ifdef FAST_CONVERSION
  return (PyInt_FromLong((long)value));
#else
  return (PyObject_CallObject((PyObject*)PyDUInt16::pyType  , Py_BuildValue("(O)", PyInt_FromLong(value))));
#endif
}

PyObject*     PyDUInt16::asPyObject(PyObject* self, int32_t attributeIndex)
{
  DUInt16 value = ((PyDObject::DPyObject*)self)->pimpl->getValue(attributeIndex);
#ifdef FAST_CONVERSION
  return (PyInt_FromLong(value));
#else
  return (PyObject_CallObject((PyObject*)PyDUInt16::pyType  , Py_BuildValue("(O)", PyInt_FromLong(value))));
#endif
}

PyDUInt32::PyDUInt32()
{
  PyTypeObject* pyType = PyDUInt32::pyType();
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
    throw Destruct::DException("PyType ready error");
}

Destruct::DValue PyDUInt32::toDValue(PyObject* value) 
{
  if (PyInt_Check(value))
  {
    DUInt32  fvalue = PyLong_AsLong(value);
    return (Destruct::RealValue<DUInt32>(fvalue));
  }
  throw Destruct::DException(CAST_ERROR(DUInt32));
}

PyObject*     PyDUInt32::asDValue(Destruct::DValue const& v)
{
  DUInt32 value = v;
#ifdef FAST_CONVERSION
  return (PyInt_FromLong(value));
#else
  return (PyObject_CallObject((PyObject*)PyDUInt32::pyType  , Py_BuildValue("(O)", PyInt_FromLong(value))));
#endif
}

PyObject*     PyDUInt32::asPyObject(PyObject* self, int32_t attributeIndex)
{
  DUInt32 value = ((PyDObject::DPyObject*)self)->pimpl->getValue(attributeIndex);
#ifdef FAST_CONVERSION
  return (PyInt_FromLong(value));
#else
  return (PyObject_CallObject((PyObject*)PyDUInt32::pyType  , Py_BuildValue("(O)", PyInt_FromLong(value))));
#endif
}

PyDUInt64::PyDUInt64()
{
  PyTypeObject* pyType = PyDUInt64::pyType();
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
    throw Destruct::DException("PyType ready error");
}

Destruct::DValue PyDUInt64::toDValue(PyObject* value) 
{
  if (PyLong_Check(value) || PyInt_Check(value))
  {
      DUInt64 fvalue = PyLong_AsLong(value);
      return (Destruct::RealValue<DUInt64>(fvalue));
  }
  throw Destruct::DException(CAST_ERROR(DUInt64));
}

PyObject*     PyDUInt64::asDValue(Destruct::DValue const& v)
{
  DUInt64 value = v;
#ifdef FAST_CONVERSION
  return (PyInt_FromLong((long)value));
#else
  return (PyObject_CallObject((PyObject*)PyDUInt64::pyType  , Py_BuildValue("(O)", PyLong_FromLong(value))));
#endif
}

PyObject*     PyDUInt64::asPyObject(PyObject* self, int32_t attributeIndex)
{
  DUInt64 value = ((PyDObject::DPyObject*)self)->pimpl->getValue(attributeIndex);
#ifdef FAST_CONVERSION
  return (PyInt_FromLong((long)value));
#else
  return (PyObject_CallObject((PyObject*)PyDUInt64::pyType  , Py_BuildValue("(O)", PyLong_FromLong(value))));
#endif
}

PyDUnicodeString::PyDUnicodeString()
{
  PyTypeObject* pyType = PyDUnicodeString::pyType();
  memcpy(pyType , &basePyType , sizeof(basePyType));

  PyMethodDef* pyMethods = (PyMethodDef*)malloc(sizeof(baseTypePyMethods));
  memcpy(pyMethods , &baseTypePyMethods , sizeof(baseTypePyMethods));
  pyMethods[0].ml_meth = (PyCFunction)this->getType;

  pyType->tp_base = &PyUnicode_Type;
  pyType->tp_name = "destruct.DUnicodeString";
  pyType->tp_basicsize = sizeof(DPyObject);
  pyType->tp_init = (initproc)(PyDUnicodeString::_init);
  pyType->tp_methods = pyMethods;
  if (PyType_Ready(pyType) < 0)
    throw Destruct::DException("PyType ready error");
}

Destruct::DValue PyDUnicodeString::toDValue(PyObject* value) 
{
  if (PyUnicode_Check(value))
  {
    PyObject* utf8Object = PyUnicode_AsUTF8String(value);
    //XXX check utf8Object is not null or raise py exception
    Destruct::DUnicodeString fvalue = std::string(PyString_AsString(utf8Object));
    // decref ytf8object
    return Destruct::RealValue<Destruct::DUnicodeString>(fvalue);
  }
  if (PyString_Check(value))
  {
    Destruct::DUnicodeString fvalue = std::string(PyString_AsString(value));
    return Destruct::RealValue<Destruct::DUnicodeString>(fvalue);
  }
  throw Destruct::DException(CAST_ERROR(DUnicodeString));
}

PyObject*     PyDUnicodeString::asDValue(Destruct::DValue const& v)
{
  Destruct::DUnicodeString value = v;
#ifdef FAST_CONVERSION
  return (PyUnicode_FromStringAndSize(value.c_str(), value.size()));
#else
  return (PyObject_CallObject((PyObject*)PyDUnicodeString::pyType  , Py_BuildValue("(O)", PyUnicode_FromStringAndSize(value.c_str(), value.size()))));
#endif
}

PyObject*     PyDUnicodeString::asPyObject(PyObject* self, int32_t attributeIndex)
{
  Destruct::DUnicodeString value = ((PyDObject::DPyObject*)self)->pimpl->getValue(attributeIndex);
#ifdef FAST_CONVERSION
  return (PyUnicode_FromStringAndSize(value.c_str(), value.size()));
#else
  return (PyObject_CallObject((PyObject*)PyDUnicodeString::pyType  , Py_BuildValue("(O)", PyUnicode_FromStringAndSize(value.c_str(), value.size()))));
#endif
}

/**
 *  PyDBuffer
 */
PyDBuffer::PyDBuffer()
{
  PyTypeObject* pyType = PyDBuffer::pyType();
  memcpy(pyType , &basePyType , sizeof(basePyType));

  PyMethodDef* pyMethods = (PyMethodDef*)malloc(sizeof(baseTypePyMethods));
  memcpy(pyMethods , &baseTypePyMethods , sizeof(baseTypePyMethods));
  pyMethods[0].ml_meth = (PyCFunction)this->getType;

  pyType->tp_base = &PyBuffer_Type; //?
  pyType->tp_name = "destruct.DBuffer";
  pyType->tp_basicsize = sizeof(DPyObject);
  pyType->tp_init = (initproc)(PyDBuffer::_init);
  pyType->tp_methods = pyMethods;
  if (PyType_Ready(pyType) < 0)
    throw Destruct::DException("PyType ready error");
}

Destruct::DValue PyDBuffer::toDValue(PyObject* value) 
{
  if (PyObject_CheckBuffer(value))
  {
    Py_buffer pyBuffer;
    PyObject_GetBuffer(value, &pyBuffer, PyBUF_SIMPLE);
    Destruct::DBuffer fvalue((uint8_t*)pyBuffer.buf, (int32_t)pyBuffer.len);
    PyBuffer_Release(&pyBuffer);
    return Destruct::RealValue<Destruct::DBuffer>(fvalue);
  }
  if (PyByteArray_Check(value))
  {
    Destruct::DBuffer fvalue((uint8_t*)PyByteArray_AsString(value), (int32_t)PyByteArray_Size(value));
    return Destruct::RealValue<Destruct::DBuffer>(fvalue);
  }
  //throw Destruct::DException("Can't cast " + std::string(value->ob_type->tp_name) + " to DBuffer");
  throw Destruct::DException(CAST_ERROR(DBuffer));
}

PyObject*     PyDBuffer::asDValue(Destruct::DValue const& v)
{
  Destruct::DBuffer value = v;
#ifdef FAST_CONVERSION
  return (PyByteArray_FromStringAndSize((char*)value.data(), value.size()));
#else
  return (PyObject_CallObject((PyObject*)PyDBuffer::pyType  , Py_BuildValue("(O)", PyByteArray_FromStringAndSize((char*)value.data(), value.size()))));
#endif
}

PyObject*     PyDBuffer::asPyObject(PyObject* self, int32_t attributeIndex)
{
  Destruct::DBuffer value = ((PyDObject::DPyObject*)self)->pimpl->getValue(attributeIndex);
#ifdef FAST_CONVERSION
  return (PyByteArray_FromStringAndSize((char*)value.data(), value.size()));
#else
  return (PyObject_CallObject((PyObject*)PyDBuffer::pyType, Py_BuildValue("(O)", PyByteArray_FromStringAndSize((char*)value.data(), value.size()))));
#endif
}

/**
 *  PyDOpaque
 */
PyDOpaque::PyDOpaque()
{
  PyTypeObject* pyType = PyDOpaque::pyType();
  memcpy(pyType , &basePyType , sizeof(basePyType));

  PyMethodDef* pyMethods = (PyMethodDef*)malloc(sizeof(baseTypePyMethods));
  memcpy(pyMethods , &baseTypePyMethods , sizeof(baseTypePyMethods));
  pyMethods[0].ml_meth = (PyCFunction)this->getType;

  pyType->tp_base = &PyBuffer_Type; //?
  pyType->tp_name = "destruct.DOpaque";
  pyType->tp_basicsize = sizeof(DPyObject);
  pyType->tp_init = (initproc)(PyDOpaque::_init);
  pyType->tp_methods = pyMethods;
  if (PyType_Ready(pyType) < 0)
    throw Destruct::DException("PyType ready error");
}

Destruct::DValue PyDOpaque::toDValue(PyObject* value) 
{
  if (PyCapsule_CheckExact(value))
  {
    void* pointer = PyCapsule_GetPointer(value, NULL); //handle only one pointer
    return Destruct::RealValue<DOpaque>(pointer);
  }
  throw Destruct::DException(CAST_ERROR(DOpaque));
}

PyObject*     PyDOpaque::asDValue(Destruct::DValue const& v)
{
  DOpaque value = v.get<DOpaque>();
#ifdef FAST_CONVERSION
  return (PyCapsule_New(value, NULL, NULL));
#else
  return (PyObject_CallObject((PyObject*)PyDOpaque::pyType  , Py_BuildValue("(O)", PyCapsule_New(value, NULL, NULL))));
#endif
}

PyObject*     PyDOpaque::asPyObject(PyObject* self, int32_t attributeIndex)
{
  DOpaque value = ((PyDObject::DPyObject*)self)->pimpl->getValue(attributeIndex).get<DOpaque>();
#ifdef FAST_CONVERSION
  return (PyCapsule_New(value, NULL, NULL));
#else
  return (PyObject_CallObject((PyObject*)PyDOpaque::pyType, Py_BuildValue("(O)", PyCapsule_New(value, NULL, NULL))));
#endif
}
