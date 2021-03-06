#ifndef _DESTRUCT_MODULE_DNULLOBJECT_HPP_
#define _DESTRUCT_MODULE_DNULLOBJECT_HPP_

#include "py_module.hpp"

#include "dnullobject.hpp"
#include "drealvalue.hpp"
#include "dexception.hpp"

typedef PythonModule< class PyDNullObject, Destruct::DNullObject > PyDNullObjectT;
class EXPORT PyDNullObject : public PyDNullObjectT, public PythonTypeBaseModule
{
public:
		              PyDNullObject();
  static PyMethodDef  pyMethods[];

  static PyTypeObject*  pyType(void);
  static int          _init(DPyObject* self, PyObject* args, PyObject* kwds);
  static void         _dealloc(DPyObject* self);
  //static PyObject*    _getattr(DPyObject* self, PyObject* name); 
  //static int          _setattr(DPyObject* self, PyObject* name, PyObject *valueObject);
  //static PyObject*    _repr(DPyObject* self);
  //static int          _compare(DPyObject* self, DPyObject* other);
  //static PyObject*    instanceOf(DPyObject* self);
  static PyObject*    getType(DPyObject* self, PyObject* args, PyObject* kwds);

  //static PyObject*    getValue(DPyObject* self, PyObject* valueObject);
  //static PyObject*    setValueObject(DPyObject* self, PyObject* args, PyObject* kwds);
  //static PyObject*    setValue(DPyObject* self, const char* attributeName, PyObject* valueObject);
  //static PyObject*    setValue(DPyObject* self, int32_t attributeIndex, PyObject* valueObject);

  PyObject*           typeObject();
//CONSTRUCTOR SANS NEW 
//FUNCTION RENVOIE SELF DNONE ETC>...
  Destruct::DValue toDValue(PyObject* value); 
  PyObject*     asDValue(Destruct::DValue const& v);
  PyObject*     asPyObject(PyObject* self, int32_t attributeIndex);
};

#endif
