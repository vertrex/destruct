#include "py_dserialize.hpp"
#include "py_dstream.hpp"
#include "py_dstruct.hpp"
#include "py_dobject.hpp"

template<>
PyTypeObject* PyDSerializeT::pyType = NULL;

PyDSerialize::PyDSerialize() //DSerializers XXX rename PyDSerialize.serialize serialize.serialize ... 
{
  pyType = (PyTypeObject*)malloc(sizeof(basePyType));
  memcpy(pyType , &basePyType , sizeof(basePyType));

  pyType->tp_name = "destruct.DSerialize";
  pyType->tp_basicsize = sizeof(PyDSerialize::DPyObject);
  pyType->tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE;
  pyType->tp_doc = "destruct.DSerialize objects";
  pyType->tp_methods = PyDSerialize::pyMethods;
  pyType->tp_methods = pyMethods;
  pyType->tp_init = (initproc)PyDSerialize::_init;
  pyType->tp_new = PyDSerialize::_new;
  pyType->tp_dealloc = (destructor)_dealloc;

  if (PyType_Ready(pyType) < 0)
    throw Destruct::DException("PyType ready error");
}

PyObject* PyDSerialize::serialize(PyDSerialize::DPyObject* self, PyObject* args, PyObject* kwds)
{
  PyDStream::DPyObject* dstream;
  PyObject*             object;

  if (!PyArg_ParseTuple(args, "OO", &dstream, &object))
  {
    PyErr_SetString(PyExc_TypeError, "must be dstream, and a dstruct or dobject.");
    return (0);
  }

  bool result = false;

  if (PyObject_TypeCheck(object, PyDStruct::pyType))
    result = self->pimpl->serialize(*dstream->pimpl, *((PyDStruct::DPyObject*)object)->pimpl);
  else if (PyObject_TypeCheck(object, PyDObject::pyType))
    result = self->pimpl->serialize(*dstream->pimpl, *((PyDObject::DPyObject*)object)->pimpl);

  if (result)
    Py_RETURN_TRUE;
  else
    Py_RETURN_FALSE;
}

PyObject* PyDSerialize::deserialize(PyDSerialize::DPyObject* self, PyObject* args, PyObject* kwds)
{
  PyDStream::DPyObject* dstream = NULL;
  PyObject*             object = NULL;

  if (!PyArg_ParseTuple(args, "O|O", &dstream, &object))
  {
    PyErr_SetString(PyExc_TypeError, "must be dstream, and dobject or none.");
    return (0);
  } 

  if (object == NULL)
  {
    Destruct::DStruct* dstruct = self->pimpl->deserialize(*dstream->pimpl); 
    if (dstruct == NULL)
      Py_RETURN_NONE;
    else
    {
      PyDStruct::DPyObject* dstructObject = (PyDStruct::DPyObject*) _PyObject_New(PyDStruct::pyType);
      dstructObject->pimpl = dstruct;

      return ((PyObject*)dstructObject);
    }
  }

  if (PyObject_TypeCheck(object, PyDObject::pyType))
  {
    bool result = self->pimpl->deserialize(*(dstream->pimpl), *((PyDObject::DPyObject*)object)->pimpl);

    if (result)
      Py_RETURN_TRUE;
    else
      Py_RETURN_FALSE;
  } 

  PyErr_SetString(PyExc_TypeError, "must be dstream, and dobject or none.");
  return (0);
}

PyMethodDef PyDSerialize::pyMethods[] = 
{
  {"serialize", (PyCFunction)PyDSerialize::serialize, METH_VARARGS, "Serialize DObject & DStruct."},
  {"deserialize", (PyCFunction)PyDSerialize::deserialize, METH_VARARGS, "Deserialize DObject & DStruct."},
  {NULL}
};

int PyDSerialize::_init(PyDSerializeT::DPyObject* self, PyObject* args, PyObject* kwds)
{
  const char*   type = NULL;

  if (!PyArg_ParseTuple(args, "s", &type))
    return (-1);

  if (type == NULL)
  {
    PyErr_SetString(PyExc_ValueError, "Serializer type must not be empty");
    return (-1);
  }

  self->pimpl = Destruct::DSerializers().to(type);

  if (self->pimpl == NULL)
  {
    PyErr_SetString(PyExc_ValueError, std::string("DSerializer for type : \"" + std::string(type) + "\" not found.").c_str());
    return (-1); 
  }

  return (0);
}
