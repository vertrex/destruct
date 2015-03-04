#ifndef _DESTRUCT_MODULE_DSTRUCT_HPP_
#define _DESTRUCT_MODULE_DSTRUCT_HPP_

#include "py_module.hpp"
#include "dstruct.hpp"

typedef PythonModule< class PyDStruct, Destruct::DStruct> PyDStructT;
class PyDStruct : public PyDStructT
{
public:
                      PyDStruct();
  static PyMethodDef  pyMethods[];

  static int          _init(DPyObject* self, PyObject* args, PyObject* kwds);
  static void         _dealloc(DPyObject* self);
  static PyObject*    repr(DPyObject* self);

  static PyObject*    name(DPyObject* self, PyObject* args, PyObject* kwds);
  static PyObject*    attributeCount(DPyObject* self, PyObject* args, PyObject* kwds);
  static PyObject*    findAttribute(DPyObject* self, PyObject* args, PyObject* kwds);
  static PyObject*    attribute(DPyObject* self, PyObject* args, PyObject* kwds);
  static PyObject*    addAttribute(DPyObject* self, PyObject* args, PyObject* kwds);
  static PyObject*    newObject(DPyObject* self, PyObject* args, PyObject* kwds);
};

#endif
