#ifndef _DESTRUCT_MODULE_DTYPE_HPP_
#define _DESTRUCT_MODULE_DTYPE_HPP_

#include "dtype.hpp"
#include "destructmodule.hpp"
#include "py_dvalue.hpp"

typedef PythonModule< class PyDType, Destruct::DType > PyDTypeT;
class PyDType : public PyDTypeT 
{
public:
                        PyDType();
  static PyMethodDef    pyMethods[];

  static int            _init(DPyObject* self, PyObject* args, PyObject* kwds);
  static PyObject*      getType(DPyObject* self, PyObject* args, PyObject* kwds);
  static PyObject*      returnType(DPyObject* self, PyObject* args, PyObject* kwds);
  static PyObject*      argumentType(DPyObject* self, PyObject* args, PyObject* kwds);
};

#endif
