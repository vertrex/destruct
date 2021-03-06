#ifndef __PYDESTRUCT_DVALUE_HPP__ 
#define __PYDESTRUCT_DVALUE_HPP__

#include "py_module.hpp"
#include "dvalue.hpp"
#include "drealvalue.hpp"
#include "dtype.hpp"

#include "py_dobject.hpp"
#include "py_dmethodobject.hpp"
#include "py_dnullobject.hpp"
#include "py_dstruct.hpp"

#include "py_dtype.hpp"

class PyDInt8 : public PythonTypeModule< PyDInt8, PyIntObject, Destruct::DType::DInt8Type > 
{
public:
  EXPORT           PyDInt8();
  PyObject*        asPyObject(PyObject* self, int32_t attributeIndex);
  PyObject*        asDValue(Destruct::DValue const& value);
  Destruct::DValue toDValue(PyObject* value);
};
typedef  PythonTypeModule< PyDInt8, PyIntObject, Destruct::DType::DInt8Type > PyDInt8T;

class PyDInt16 : public PythonTypeModule< PyDInt16, PyIntObject, Destruct::DType::DInt16Type >
{
public:
  EXPORT           PyDInt16();
  PyObject*        asPyObject(PyObject* self, int32_t attributeIndex);
  PyObject*        asDValue(Destruct::DValue const& value);
  Destruct::DValue toDValue(PyObject* value); 
};
typedef  PythonTypeModule< PyDInt16, PyIntObject, Destruct::DType::DInt16Type > PyDInt16T;

class PyDInt32 : public  PythonTypeModule< PyDInt32, PyIntObject, Destruct::DType::DInt32Type >
{
public:
  EXPORT           PyDInt32();
  PyObject*        asPyObject(PyObject* self, int32_t attributeIndex);
  PyObject*        asDValue(Destruct::DValue const& value);
  Destruct::DValue toDValue(PyObject* value); 
};
typedef  PythonTypeModule< PyDInt32, PyIntObject, Destruct::DType::DInt32Type > PyDInt32T;

class PyDInt64 : public   PythonTypeModule< PyDInt64, PyIntObject, Destruct::DType::DInt64Type >
{
public:
  EXPORT           PyDInt64();
  PyObject*        asPyObject(PyObject* self, int32_t attributeIndex);
  PyObject*        asDValue(Destruct::DValue const& value);
  Destruct::DValue toDValue(PyObject* value); 
};
typedef  PythonTypeModule< PyDInt64, PyIntObject, Destruct::DType::DInt64Type > PyDInt64T;

class PyDUInt8 : public PythonTypeModule< PyDUInt8, PyIntObject, Destruct::DType::DUInt8Type > 
{
public:
  EXPORT           PyDUInt8();
  PyObject*        asPyObject(PyObject* self, int32_t attributeIndex);
  PyObject*        asDValue(Destruct::DValue const& value);
  Destruct::DValue toDValue(PyObject* value); 
};
typedef  PythonTypeModule< PyDUInt8, PyIntObject, Destruct::DType::DUInt8Type > PyDUInt8T;

class PyDUInt16 : public PythonTypeModule< PyDUInt16, PyIntObject, Destruct::DType::DUInt16Type >
{
public:
  EXPORT           PyDUInt16();
  PyObject*        asPyObject(PyObject* self, int32_t attributeIndex);
  PyObject*        asDValue(Destruct::DValue const& value);
  Destruct::DValue toDValue(PyObject* value); 
};
typedef  PythonTypeModule< PyDUInt16, PyIntObject, Destruct::DType::DUInt16Type > PyDUInt16T;

class PyDUInt32 : public PythonTypeModule< PyDUInt32, PyIntObject, Destruct::DType::DUInt32Type >
{
public:
  EXPORT           PyDUInt32();
  PyObject*        asPyObject(PyObject* self, int32_t attributeIndex);
  PyObject*        asDValue(Destruct::DValue const& value);
  Destruct::DValue toDValue(PyObject* value); 
};
typedef  PythonTypeModule< PyDUInt32, PyIntObject, Destruct::DType::DUInt32Type > PyDUInt32T;

class PyDUInt64 : public PythonTypeModule< PyDUInt64, PyIntObject, Destruct::DType::DUInt64Type >
{
public:
  EXPORT           PyDUInt64();
  Destruct::DValue toDValue(PyObject* value); 
  PyObject*        asDValue(Destruct::DValue const& value);
  PyObject*        asPyObject(PyObject* self, int32_t attributeIndex);
};
typedef  PythonTypeModule< PyDUInt64, PyIntObject, Destruct::DType::DUInt64Type > PyDUInt64T;

class PyDUnicodeString : public PythonTypeModule< PyDUnicodeString, PyStringObject, Destruct::DType::DUnicodeStringType >
{
public:
  EXPORT           PyDUnicodeString();
  Destruct::DValue toDValue(PyObject* value); 
  PyObject*        asDValue(Destruct::DValue const& value);
  PyObject*        asPyObject(PyObject* self, int32_t attributeIndex);
};
typedef  PythonTypeModule< PyDUnicodeString, PyStringObject, Destruct::DType::DUnicodeStringType > PyDUnicodeStringT;

                                                      //XXX? PyBuffer
class PyDBuffer : public PythonTypeModule< PyDBuffer, PyObject, Destruct::DType::DBufferType>
{
public:
  EXPORT           PyDBuffer();
  Destruct::DValue toDValue(PyObject* value);
  PyObject*        asDValue(Destruct::DValue const& value);
  PyObject*        asPyObject(PyObject* self, int32_t attributeIndex);
};

class PyDOpaque : public PythonTypeModule< PyDOpaque, PyObject, Destruct::DType::DOpaqueType>
{
public:
  EXPORT           PyDOpaque();
  Destruct::DValue toDValue(PyObject* value);
  PyObject*        asDValue(Destruct::DValue const& value);
  PyObject*        asPyObject(PyObject* self, int32_t attributeIndex);
};

static PythonTypeBaseModule*  DValueDispatchTable[] =
{
  &PyDInt8::moduleInit(),
  &PyDInt16::moduleInit(),
  &PyDInt32::moduleInit(),
  &PyDInt64::moduleInit(),
 
  &PyDUInt8::moduleInit(),
  &PyDUInt16::moduleInit(),
  &PyDUInt32::moduleInit(),
  &PyDUInt64::moduleInit(),

  &PyDUnicodeString::moduleInit(),
  &PyDObject::moduleInit(),
  &PyDMethodObject::moduleInit(),
  &PyDNullObject::moduleInit(),

  &PyDStruct::moduleInit(),
  &PyDBuffer::moduleInit(),

  &PyDOpaque::moduleInit(),
  //&PyDUnknownType::moduleInit(), throw error unknown type 
  NULL,
};

#endif
