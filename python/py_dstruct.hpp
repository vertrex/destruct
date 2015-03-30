#ifndef _DESTRUCT_MODULE_DSTRUCT_HPP_
#define _DESTRUCT_MODULE_DSTRUCT_HPP_

#include "py_module.hpp"

#include "destruct.hpp"
#include "dstruct.hpp"

typedef PythonModule<class PyDStruct, Destruct::DStruct> PyDStructT;
class EXPORT PyDStruct : public PyDStructT, public PythonTypeBaseModule
{
public:
                      PyDStruct();
  static PyMethodDef  pyMethods[];

  static PyTypeObject*  pyType(void);
  static int          _init(DPyObject* self, PyObject* args, PyObject* kwds);
  static void         _dealloc(DPyObject* self);
  static PyObject*    repr(DPyObject* self);

  static PyObject*    name(DPyObject* self, PyObject* args, PyObject* kwds);
  static PyObject*    attributeCount(DPyObject* self, PyObject* args, PyObject* kwds);
  static PyObject*    findAttribute(DPyObject* self, PyObject* args, PyObject* kwds);
  static PyObject*    attribute(DPyObject* self, PyObject* args, PyObject* kwds);
  static PyObject*    addAttribute(DPyObject* self, PyObject* args, PyObject* kwds);
  static PyObject*    newObject(DPyObject* self, PyObject* args, PyObject* kwds);

  static PyObject*    getType(DPyObject* self, PyObject* args, PyObject* kwds);
  PyObject*           typeObject();

  Destruct::DValue    toDValue(PyObject* value); 
  PyObject*           asDValue(Destruct::DValue const& v);
  PyObject*           asPyObject(PyObject* _self, int32_t attributeIndex);
};

#endif
