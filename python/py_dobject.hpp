#ifndef _DESTRUCT_MODULE_DOBJECT_HPP_
#define _DESTRUCT_MODULE_DOBJECT_HPP_

#include "drealvalue.hpp"
#include "dsimpleobject.hpp"
#include "dnullobject.hpp"
#include "destructmodule.hpp"

typedef PythonModule< class PyDObject, Destruct::DObject > PyDObjectT;
class PyDObject : public PyDObjectT, public PythonTypeBaseModule
{
public:
                      PyDObject();
  static PyMethodDef  pyMethods[];

  static int          _init(DPyObject* self, PyObject* args, PyObject* kwds);
  static void         _dealloc(DPyObject* self);
  static PyObject*    _getattr(DPyObject* self, PyObject* name); 
  static int          _setattr(DPyObject* self, PyObject* name, PyObject *valueObject);
  static PyObject*    _repr(DPyObject* self);
  static int          _compare(DPyObject* self, DPyObject* other);

  static PyObject*    instanceOf(DPyObject* self);
  static PyObject*    getType(DPyObject* self, PyObject* args, PyObject* kwds);

  static PyObject*    getValue(DPyObject* self, PyObject* valueObject);

  static PyObject*    setValueObject(DPyObject* self, PyObject* args, PyObject* kwds);
  static PyObject*    setValue(DPyObject* self, const char* attributeName, PyObject* valueObject);
  static PyObject*    setValue(DPyObject* self, int32_t attributeIndex, PyObject* valueObject);

  PyObject*           typeObject();

  Destruct::DValue toDValue(PyObject* value) 
  {
     if (PyObject_TypeCheck(value, PyDObject::pyType))
       return Destruct::RealValue<Destruct::DObject* >(((DPyObject*)value)->pimpl);
     if (value == Py_None)
       return Destruct::RealValue<Destruct::DObject* >(Destruct::DNone); 
     throw std::string("Can't cast to DObject*");
  }

  PyObject*     asDValue(Destruct::DValue v)
  {
    Destruct::DObject*     value = v.get<Destruct::DObject*>();

    if (value == NULL || value == Destruct::DNone)
      Py_RETURN_NONE;
   
    Py_INCREF(pyType);
    PyDObject::DPyObject*  dobjectObject = (PyDObject::DPyObject*)_PyObject_New(PyDObject::pyType);
    dobjectObject->pimpl = value;

    return ((PyObject*)dobjectObject);
  }

  PyObject*     asPyObject(PyObject* self, int32_t attributeIndex)
  {
    Destruct::DObject*     value = ((PyDObject::DPyObject*)self)->pimpl->getValue(attributeIndex).get<Destruct::DObject*>();

    if (value == NULL || value == Destruct::DNone)
      Py_RETURN_NONE;
   
    Py_INCREF(pyType);
    PyDObject::DPyObject*  dobjectObject = (PyDObject::DPyObject*)_PyObject_New(PyDObject::pyType);
    dobjectObject->pimpl = value;

    return ((PyObject*)dobjectObject);
  }
};

#endif
