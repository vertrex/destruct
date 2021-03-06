#ifndef _DESTRUCT_MODULE_DTYPE_HPP_
#define _DESTRUCT_MODULE_DTYPE_HPP_

#include "py_module.hpp"
#include "py_dvalue.hpp"

#include "dtype.hpp"

typedef PythonModule< class PyDType, Destruct::DType > PyDTypeT;
class PyDType : public PyDTypeT 
{
public:
                        PyDType();
  static PyMethodDef    pyMethods[];
  static PyTypeObject*  pyType(void);

  static int            _init(DPyObject* self, PyObject* args, PyObject* kwds);
  //static void           _dealloc(DPyObject* self);
  static PyObject*      _repr(DPyObject* self);

  static PyObject*      getType(DPyObject* self, PyObject* args, PyObject* kwds);
  static PyObject*      returnType(DPyObject* self, PyObject* args, PyObject* kwds);
  static PyObject*      argumentType(DPyObject* self, PyObject* args, PyObject* kwds);

  static PyObject*      name(DPyObject* self, PyObject* args, PyObject* kwds);
  static PyObject*      returnName(DPyObject* self, PyObject* args, PyObject* kwds);
  static PyObject*      argumentName(DPyObject* self, PyObject* args, PyObject* kwds);
};

#endif
