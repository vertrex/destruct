#ifndef _DESTRUCT_MODULE_DMETHODOBJECT_HPP_
#define _DESTRUCT_MODULE_DMETHODOBJECT_HPP_

#include "dfunction.hpp"

class DPythonMethodObject : public Destruct::DFunctionObject
{
public:
  DPythonMethodObject(PyObject* self, PyObject* callable, Destruct::DType type) : __self(self), __pythonCallable(callable),  __type(type)
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
  Destruct::DType       __type;
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

  /*
   *  Used in DObject but need self, and type or attributeIndex because method keep a pointer to this and need to know there own argument
   */
  //force cast par une structure qui contiens self+ datatype ? + callable ?

  Destruct::DValue toDValue(PyObject* value) 
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
  PyObject*     asDValue(Destruct::DValue v)
  {
    std::cout << "PyDMethodObject:asDValue(DValue v) not implemented" << std::endl;
    Py_RETURN_NONE;
  }

  PyObject*     asPyObject(PyObject* _self, int32_t attributeIndex)
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
};

#endif
