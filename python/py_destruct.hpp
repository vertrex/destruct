#ifndef _DESTRUCT_DESTRUCT_HPP_
#define _DESTRUCT_DESTRUCT_HPP_

#include "destruct.hpp"
#include "destructmodule.hpp"

typedef PythonModule< class PyDestruct, Destruct::Destruct> PyDestructT;
class PyDestruct : public PyDestructT
{
public:
                      PyDestruct();
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
