#ifndef _DESTRUCT_DESTRUCT_HPP_
#define _DESTRUCT_DESTRUCT_HPP_

#include "py_module.hpp"
#include "dstructs.hpp"

typedef PythonModule< class PyDStructs, Destruct::DStructs> PyDStructsT;
class PyDStructs : public PyDStructsT
{
public:
                      PyDStructs();
  static PyMethodDef  pyMethods[];

  static int          _init(DPyObject* self, PyObject* args, PyObject* kwds);
  static void         _dealloc(DPyObject* self);

  static PyObject*    count(DPyObject* self, PyObject* args, PyObject* kwds);
  static PyObject*    findObject(DPyObject* self, PyObject* args, PyObject* kwds);
  static PyObject*    find(DPyObject* self, const char* index);
  static PyObject*    find(DPyObject* self, size_t index);
  static PyObject*    registerDStruct(DPyObject* self, PyObject* args, PyObject* kwds);
};

#endif
