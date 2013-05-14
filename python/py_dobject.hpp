#ifndef _DESTRUCT_MODULE_DOBJECT_HPP_
#define _DESTRUCT_MODULE_DOBJECT_HPP_

#include "drealvalue.hpp"
#include "dsimpleobject.hpp"
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
  static PyObject*    replaceValueObject(DPyObject* self, PyObject* args, PyObject* kwds);
  static PyObject*    replaceValue(DPyObject* self, const char* attributeName, PyObject* valueObject);
  static PyObject*    replaceValue(DPyObject* self, int32_t attributeIndex, PyObject* valueObject);

  PyObject*           typeObject();

  Destruct::DValue toDValue(PyObject* value) 
  {
     if (PyObject_TypeCheck(value, PyDObject::pyType))
       return Destruct::RealValue<Destruct::DObject* >(((DPyObject*)value)->pimpl);

     if (value == Py_None)
     {
       return Destruct::RealValue<Destruct::DObject* >(NULL);
     }
     throw std::string("Can't cast to DObject*");
  }

  PyObject*     asPyObject(PyObject* self, int32_t attributeIndex)
  {
    Destruct::DObject*     value = ((PyDObject::DPyObject*)self)->pimpl->getValue(attributeIndex).get<Destruct::DObject*>();

    if (value == NULL)
      Py_RETURN_NONE;
   
    Py_INCREF(pyType);
    PyDObject::DPyObject*  dobjectObject = (PyDObject::DPyObject*)_PyObject_New(PyDObject::pyType);
    dobjectObject->pimpl = value;
    Py_INCREF(dobjectObject);// XXX ?

    return ((PyObject*)dobjectObject);
  }
};

/*
 *   This is a test implementation for 'std::string func(DObject*)' only
 *   This should be changed for a more geric implementation or by supporting method instead of 'attribute-method' 
 */

template <typename ReturnPlainType>
class DPythonFunctionValue : public Destruct::ComputingValue
{
  typedef PyObject* FunctionType;
public:
  DPythonFunctionValue(PyDObject::DPyObject* dobjectObject, FunctionType getFunction) : ComputingValue(dobjectObject->pimpl), __getFunction(getFunction)
  {
  }

  DPythonFunctionValue(Destruct::DObject* dobject, FunctionType getFunction) : ComputingValue(dobject), __getFunction(getFunction)
  {
  }

  Destruct::BaseValue* clone(Destruct::DObject* dobject) const
  {
    return (new DPythonFunctionValue(dobject, this->__getFunction));
  }

  Destruct::DValue getFinal() const
  {
    PyObject* resultObject = PyObject_CallObject(this->__getFunction, NULL);
    char* result = PyString_AsString(resultObject);

    return (Destruct::RealValue<std::string>(result));
  }

  void set(Destruct::DValue const & v)
  {
    throw "not supported";
  }

private:
  FunctionType            __getFunction;
};

#endif
