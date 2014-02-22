#ifndef _DESTRUCT_MODULE_DMETHODOBJECT_HPP_
#define _DESTRUCT_MODULE_DMETHODOBJECT_HPP_

#include "dfunction.hpp"

class DPythonMethodObject : public Destruct::DFunctionObject
{
public:
  DPythonMethodObject(PyObject* self, PyObject* callable, int32_t attributeIndex) : __self(self), __pythonCallable(callable),  __attributeIndex(attributeIndex) 
  {
    Py_INCREF(__self);
    Py_INCREF(__pythonCallable);
  }

  ~DPythonMethodObject()
  {
    Py_DECREF(__self); 
    Py_DECREF(__pythonCallable);
  }

  Destruct::DValue      call(void) const;
  Destruct::DValue      call(Destruct::DValue const& args) const;
  PyObject*             fastCall(PyObject* args) const;
private:
  PyObject*             __self;
  PyObject*             __pythonCallable;
  int32_t               __attributeIndex; 
};

typedef PythonModule<class PyDMethodObject, Destruct::DFunctionObject > PyDMethodObjectT;
class PyDMethodObject : public PyDMethodObjectT, public PythonTypeBaseModule
{
public :
  struct DPyObject 
  {
    PyObject_HEAD;
    Destruct::DFunctionObject*  pimpl;
    Destruct::DObject*          dobject;
    int32_t                     index;
  };
                                PyDMethodObject();
  static PyMethodDef            pyMethods[];

  static int                    _init(DPyObject* self, PyObject* args, PyObject* kwds);
  static void                   _dealloc(DPyObject* self);
  static PyObject*              _getattr(DPyObject* self, PyObject* name); 
  static int                    _setattr(DPyObject* self, PyObject* name, PyObject *valueObject);
  static PyObject*              _repr(DPyObject* self);

  static PyObject*              call(PyObject* attributeObject, PyObject* args);
  static PyObject*              getType(DPyObject* self, PyObject* args, PyObject* kwds);
  PyObject*                     typeObject();

  Destruct::DValue toDValue(PyObject* value); 
  PyObject*     asDValue(Destruct::DValue v);
  PyObject*     asPyObject(PyObject* _self, int32_t attributeIndex);
};

#endif
