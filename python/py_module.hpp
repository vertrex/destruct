#ifndef __DESTRUCT_PYTHON_MODULE__
#define __DESTRUCT_PYTHON_MODULE__

#include <Python.h>
#include "destruct.hpp"
#include "dvalue.hpp"

#define CAST_ERROR(x) "Can't cast " + std::string(value->ob_type->tp_name) + " to " #x

#define CHECK_PIMPL\
  if (self->pimpl == NULL)\
  {\
    PyErr_SetString(PyExc_RuntimeError, "Private implementation pointer's is NULL.");\
    return (0);\
  }

#define CHECK_ALLOC(obj)\
  if (obj == NULL)\
    return (PyErr_NoMemory());

#define INIT_CHECK_ALLOC(obj)\
  if (obj == NULL)\
  {\
    PyErr_NoMemory();\
    return (-1);\
  }

class EXPORT PythonBaseModule
{
public:
  PythonBaseModule()
  {
  }

  static PyTypeObject        basePyType; 

  static PySequenceMethods   baseSequenceMethods;
  static PyMappingMethods    baseMappingMethods;
  static const std::string   pyErrorAsString(void);

  static Destruct::DValue    pyObjectToDValue(PyObject* object);
  static PyObject*           dvalueAsPyObject(Destruct::DValue const& value);
  //static const std::string   pyErrorAsString(void);

  static int pyTracebackInternalAsString(PyTracebackObject* tb, std::string& errorMessage, long limit);
  static int pyTracebackAsString(PyObject* tb, std::string& errorMessage);
};

template < typename FinalType, typename PlainType >
class EXPORT PythonModule : public PythonBaseModule
{
public: 
  struct DPyObject 
  {
     PyObject_HEAD;
     PlainType*    pimpl;
  };
 
  static DPyObject          pyObject;
  static PySequenceMethods* pySequenceMethods;
  static PyMappingMethods*  pyMappingMethods;

  static FinalType& moduleInit()
  {
    static FinalType instance;
    return (instance);
  }

  static PyObject*    _new(PyTypeObject* type, PyObject* args, PyObject* kwds) 
  {
    DPyObject*  self;

    self = (DPyObject*)type->tp_alloc(type, 0);
    return ((PyObject*)self);
  };

  static int          _init(DPyObject* self, PyObject* args, PyObject* kwds) 
  {
    self->pimpl = new PlainType();
    if (self->pimpl == NULL)
    {
      PyErr_NoMemory();
      return (-1);
    }

    return (0);
  };

  static void         _dealloc(DPyObject* self) 
  {
    if (self->pimpl != NULL)
    {
      delete self->pimpl;
      self->pimpl = NULL;
    }
    self->ob_type->tp_free((PyObject*)self);
  }
};

class EXPORT PythonTypeBaseModule : public PythonBaseModule
{
public:
  static PyMethodDef        baseTypePyMethods[2];

  virtual PyObject*         asPyObject(PyObject* self, int32_t attributeIndex) = 0;
  virtual PyObject*         asDValue(Destruct::DValue const& value) = 0;
  virtual PyObject*         typeObject(void) = 0 ;
  virtual Destruct::DValue  toDValue(PyObject* value) = 0;
};

template < typename FinalType, typename PythonObject , int __typeId>
class EXPORT PythonTypeModule : public PythonTypeBaseModule
{
public:
  struct DPyObject 
  {
    PythonObject  __base;
  };

  static DPyObject     pyObject;

  static PyTypeObject*     pyType(void)
  {
    static PyTypeObject* pyType = (PyTypeObject*)malloc(sizeof(basePyType));
    return (pyType);
  }

  static int    _init(PyObject* self, PyObject* args, PyObject* kwds)
  {
    PyTypeObject* pyType = PythonTypeModule<FinalType, PythonObject, __typeId>::pyType();
    if (pyType->tp_base->tp_init((PyObject *)self, args, kwds) < 0)
      return (-1);
    return (0);
  }

  static FinalType& moduleInit() 
  {
    static FinalType instance;
    return (instance);
  }
 
  PyObject* typeObject(void)
  {
    PyTypeObject* pyType = PythonTypeModule<FinalType, PythonObject, __typeId>::pyType();
    if (pyType != NULL)
    {
      Py_INCREF((PyObject*)pyType);
      return ((PyObject*)pyType);
    }
    Py_RETURN_NONE;
  }

  static PyObject* getType(DPyObject *self, PyObject *args, PyObject *kwds)
  {
    return (PyInt_FromSize_t(__typeId));
  }
};

#endif
