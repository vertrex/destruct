#include "py_dstruct.hpp"
#include "py_dobject.hpp"
#include "py_loader.hpp"

PyTypeObject*     PyLoader::pyType(void)
{
  static PyTypeObject* pyType = (PyTypeObject*)malloc(sizeof(basePyType));
  return (pyType);
}

PyLoader::PyLoader()
{
  PyTypeObject* pyType = PyLoader::pyType();
  memcpy(pyType , &basePyType , sizeof(basePyType));

  pyType->tp_name = "destruct.Loader";
  pyType->tp_basicsize = sizeof(PyLoader::DPyObject);
  pyType->tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE;
  pyType->tp_doc = "destruct.Loader objects";
  pyType->tp_methods = PyLoader::pyMethods;
  pyType->tp_methods = pyMethods;
  pyType->tp_init = (initproc)_init;
  pyType->tp_new = _new;
  pyType->tp_dealloc = (destructor)_dealloc;

  if (PyType_Ready(pyType) < 0)
    throw std::string("PyType ready error");
}

PyObject* PyLoader::loadFile(PyLoader::DPyObject* self, PyObject* args, PyObject *kwds)
{
  PyObject* filePathObject;

  if (!PyArg_ParseTuple(args, "S", &filePathObject))
  {
    PyErr_SetString(PyExc_TypeError, "must be string and integer");
    return (0);
  }
  const char* filePath = PyString_AsString(filePathObject);
  self->pimpl->loadFile(filePath);

  Py_RETURN_NONE;
}

PyMethodDef PyLoader::pyMethods[] = 
{
  {"loadFile", (PyCFunction)loadFile, METH_VARARGS, "Load file."},
  {NULL}
};

PyMODINIT_FUNC init_loader(void)
{
  PyObject* module;

  PyLoader::moduleInit();

  if ((module = Py_InitModule3("_loader", destruct_methods, "Destruct C Loader")) == NULL)
    return;    

  DESTRUCT_ADD_MODULE(PyLoader::pyType(), "Loader")
}
