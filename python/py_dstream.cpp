#include "dexception.hpp"

#include "py_dstream.hpp"

#include <python2.7/Python.h>
#include <python2.7/structmember.h>

template<>
PyTypeObject* PyDStreamT::pyType = NULL;

PyDStream::PyDStream()
{
  pyType = (PyTypeObject*)malloc(sizeof(basePyType));
  memcpy(pyType , &basePyType , sizeof(basePyType));

  pyType->tp_name = "destruct.DStream";
  pyType->tp_basicsize = sizeof(PyDStream::DPyObject);
  pyType->tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE;
  pyType->tp_doc = "destruct.DStream objects";
  pyType->tp_methods = PyDStream::pyMethods;
  pyType->tp_init = (initproc)PyDStream::_init;
  pyType->tp_new = PyDStream::_new;
  pyType->tp_dealloc = (destructor)_dealloc;

  if (PyType_Ready(pyType) < 0)
    throw Destruct::DException("PyType ready error");

  PyDict_SetItemString(pyType->tp_dict, "Input", PyInt_FromLong(0));
  PyDict_SetItemString(pyType->tp_dict, "Output", PyInt_FromLong(1));
}

PyMethodDef PyDStream::pyMethods[] = 
{
  {NULL}
};

int PyDStream::_init(PyDStream::DPyObject* self, PyObject* args, PyObject* kwds)
{
  int           mode = 0;
  const char*   fileName = NULL;

  if (!PyArg_ParseTuple(args, "si", &fileName, &mode))
    return (-1);

  if (fileName == NULL)
  {
    PyErr_SetString(PyExc_ValueError, "file name must not be empty");
    return (-1);
  }

  self->pimpl = new Destruct::DStream(fileName, (Destruct::DStream::mode)mode);
  INIT_CHECK_ALLOC(self->pimpl);

  return (0);
}
