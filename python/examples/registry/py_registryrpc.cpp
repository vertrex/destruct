#include "py_dstruct.hpp"
#include "py_dobject.hpp"
#include "py_registryrpc.hpp"

PyTypeObject*     PyRegistryRPC::pyType(void)
{
    static PyTypeObject* pyType = (PyTypeObject*)malloc(sizeof(basePyType));
    return (pyType);
}

PyRegistryRPC::PyRegistryRPC()
{
  PyTypeObject* pyType = PyRegistryRPC::pyType();
  memcpy(pyType , &basePyType , sizeof(basePyType));

  pyType->tp_name = "destruct.RegistryRPC";
  pyType->tp_basicsize = sizeof(PyRegistryRPC::DPyObject);
  pyType->tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE;
  pyType->tp_doc = "destruct.RegistryRPC objects";
  pyType->tp_methods = PyRegistryRPC::pyMethods;
  pyType->tp_methods = pyMethods;
  pyType->tp_init = (initproc)_init;
  pyType->tp_new = _new;
  pyType->tp_dealloc = (destructor)_dealloc;

  if (PyType_Ready(pyType) < 0)
    throw std::string("PyType ready error");
}

PyObject*    PyRegistryRPC::connect(PyRegistryRPC::DPyObject* self, PyObject* args, PyObject *kwds)
{
  PyObject* addressObject;
  uint32_t port;

  if (!PyArg_ParseTuple(args, "SI", &addressObject, &port))
  {
    PyErr_SetString(PyExc_TypeError, "must be string and integer");
    return (0);
  }
  const char* address = PyString_AsString(addressObject);
  std::cout << "connection on " << std::string(address) << "  " << port << std::endl;
  Destruct::DObject* dobject = self->pimpl->connect(address, port);
  PyDObject::DPyObject*  dobjectObject = (PyDObject::DPyObject*)_PyObject_New(PyDObject::pyType());
  dobjectObject->pimpl = dobject;
  Py_INCREF(dobjectObject);

  return (Py_BuildValue("O", dobjectObject));
}

PyMethodDef PyRegistryRPC::pyMethods[] = 
{
  {"connect", (PyCFunction)connect, METH_VARARGS, "Connect to server."},
  {NULL}
};

PyMODINIT_FUNC init_registryrpc(void)
{
  PyObject* module;

  PyRegistryRPC::moduleInit();

  if ((module = Py_InitModule3("_registryrpc", destruct_methods, "Registry RPC client")) == NULL)
    return;    

  DESTRUCT_ADD_MODULE(PyRegistryRPC::pyType(), "RegistryRPC")
}
