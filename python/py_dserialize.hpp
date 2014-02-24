#ifndef _DESTRUCT_MODULE_DSERIALIZE_HPP_
#define _DESTRUCT_MODULE_DSERIALIZE_HPP_

#include "destructmodule.hpp"
#include "protocol/dserialize.hpp"

typedef PythonModule< class PyDSerialize, Destruct::DSerialize> PyDSerializeT;
class PyDSerialize : public PyDSerializeT
{
public:
                     PyDSerialize();
  static PyMethodDef pyMethods[];

  static int         _init(DPyObject* self, PyObject* args, PyObject* kwds);
  static PyObject*   serialize(DPyObject* self, PyObject* args, PyObject* kwds);
  static PyObject*   deserialize(DPyObject* self, PyObject* args, PyObject* kwds);
};

#endif
