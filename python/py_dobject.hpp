#ifndef _DESTRUCT_MODULE_DOBJECT_HPP_
#define _DESTRUCT_MODULE_DOBJECT_HPP_

#include "py_module.hpp"

#include "drealvalue.hpp"
#include "dsimpleobject.hpp"
#include "dnullobject.hpp"
#include "dexception.hpp"

typedef PythonModule< class PyDObject, Destruct::DObject > PyDObjectT;
class EXPORT PyDObject : public PyDObjectT, public PythonTypeBaseModule
{
public:
	                PyDObject();
  static PyTypeObject*  pyType(void);

  static PyMethodDef    pyMethods[];
 
  static int            _init(DPyObject* self, PyObject* args, PyObject* kwds);
  static void           _dealloc(DPyObject* self);
  static PyObject*      _getattr(DPyObject* self, PyObject* name); 
  static int            _setattr(DPyObject* self, PyObject* name, PyObject *valueObject);
  static PyObject*      _repr(DPyObject* self);
  static int            _compare(DPyObject* self, DPyObject* other);
  static PyObject*      _dir(DPyObject* self, PyObject* args, PyObject* kwds);

  static PyObject*      refCount(DPyObject* self);
  static PyObject*      addRef(DPyObject* self);
  static PyObject*      destroy(DPyObject* self);
  static PyObject*      instanceOf(DPyObject* self);
  static PyObject*      clone(DPyObject* self);

  static PyObject*      getType(DPyObject* self, PyObject* args, PyObject* kwds);

  static PyObject*      getValue(DPyObject* self, PyObject* valueObject);

  static PyObject*      setValueObject(DPyObject* self, PyObject* args, PyObject* kwds);
  static PyObject*      setValue(DPyObject* self, const char* attributeName, PyObject* valueObject);
  static PyObject*      setValue(DPyObject* self, int32_t attributeIndex, PyObject* valueObject);

  static PyObject*      _iter(DPyObject* self);
  static PyObject*      _iternext(DPyObject* self);
  static Py_ssize_t     _length(DPyObject* self);
  static PyObject*      _item(DPyObject* self, Py_ssize_t index);
  static int            _setitem(DPyObject* self, Py_ssize_t index, PyObject* item); 
  static int            _setmap(DPyObject* self, PyObject* v, PyObject* w); 
  static PyObject*      _map(PyDObject::DPyObject* self, register PyObject* key);

  PyObject*             typeObject();
  
  Destruct::DValue      toDValue(PyObject* value); 
  PyObject*             asDValue(Destruct::DValue const& v);
  PyObject*             asPyObject(PyObject* self, int32_t attributeIndex);
};

#endif
