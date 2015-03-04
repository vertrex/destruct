#ifndef _DESTRUCT_MODULE_DATTRIBUTE_HPP_
#define _DESTRUCT_MODULE_DATTRIBUTE_HPP_

#include "py_module.hpp"
#include "dattribute.hpp"

typedef PythonModule< class PyDAttribute, Destruct::DAttribute > PyDAttributeT;
class PyDAttribute : public PyDAttributeT 
{
public:
                      PyDAttribute();
  static PyMethodDef  pyMethods[];

  static int          _init(DPyObject* self, PyObject* args, PyObject* kwds);
  static PyObject*    name(DPyObject* self, PyObject* args, PyObject* kwds);
  static PyObject*    dtype(DPyObject* self, PyObject* args, PyObject* kwds);
};

#endif
