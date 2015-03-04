#include "py_destruct_test.hpp"
#include "py_dstruct.hpp"
#include "py_dobject.hpp"

#ifdef WIN32
EXPORT PyTypeObject PythonBaseModule::basePyType =
{
        PyObject_HEAD_INIT(NULL)
        0,
        0,
        0,                       /* tp_basicsize */
        0,                       /* tp_itemsize */
        0,                       /* tp_dealloc */
        0,                       /* tp_print */
        0,                       /* tp_getattr */
        0,                       /* tp_setattr */
        0,                       /* tp_compare */
        0,                       /* tp_repr */
        0,                       /* tp_as_number */
        0,                       /* tp_as_sequence */
        0,                       /* tp_as_mapping */
        0,                       /* tp_hash */
        0,                       /* tp_call */
        0,                       /* tp_str */
        0,                       /* tp_getattro */
        0,                       /* tp_setattro */
        0,                       /* tp_as_buffer */
        Py_TPFLAGS_DEFAULT |
                Py_TPFLAGS_BASETYPE,   /* tp_flags */
        0,                       /* tp_doc */
        0,                       /* tp_traverse */
        0,                       /* tp_clear */
        0,                       /* tp_richcompare */
        0,                       /* tp_weaklistoffset */
        0,                       /* tp_iter */
        0,                       /* tp_iternext */
        0,                       /* tp_method */
        0,                       /* tp_members */
        0,                       /* tp_getset */
        0,                       /* tp_base */
        0,                       /* tp_dict */
        0,                       /* tp_descr_get */
        0,                       /* tp_descr_set */
        0,                       /* tp_dictoffset */
        0,                       /* tp_init */
        0,                       /* tp_alloc */
        0,                       /* tp_new */
};
#endif

PyTest::PyTest()
{
  PyTypeObject* pyType = PyTest::pyType();
  memcpy(pyType , &basePyType , sizeof(basePyType));

  pyType->tp_name = "destruct.Test";
  pyType->tp_basicsize = sizeof(PyTest::DPyObject);
  pyType->tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE;
  pyType->tp_doc = "destruct.Test objects";
  pyType->tp_methods = PyTest::pyMethods;
  pyType->tp_methods = pyMethods;
  pyType->tp_init = (initproc)_init;
  pyType->tp_new = _new;
  pyType->tp_dealloc = (destructor)_dealloc;

  if (PyType_Ready(pyType) < 0)
    throw std::string("PyType ready error");
}

PyTypeObject*     PyTest::pyType(void)
{
  static PyTypeObject* pyType = (PyTypeObject*)malloc(sizeof(basePyType));
  return (pyType);
}

PyObject*    PyTest::run(PyTest::DPyObject* self, PyObject* args, PyObject *kwds)
{
//Py_BEGIN_ALLOW_THREADS //alow threading
  self->pimpl->run();
  //Py_END_ALLOW_THREADS

  Py_RETURN_NONE;
}

PyObject*   PyTest::showAttribute(PyTest::DPyObject* self, PyObject* args, PyObject *kwds)
{
  PyDStruct::DPyObject*       dstructObject;

  if (!PyArg_ParseTuple(args, "O", &dstructObject))
    return (0);

  if (!PyObject_TypeCheck(dstructObject, PyDStruct::pyType()))
    return (0);

  self->pimpl->showAttribute(dstructObject->pimpl);
  Py_RETURN_NONE;
}

PyObject*    PyTest::showObjectAttribute(PyTest::DPyObject* self, PyObject* args, PyObject *kwds)
{
  PyDObject::DPyObject*        dobjectObject;

  if (!PyArg_ParseTuple(args, "O", &dobjectObject))
    return (0);

  if (!PyObject_TypeCheck(dobjectObject, PyDObject::pyType()))
    return (0);

  self->pimpl->showObjectAttribute(dobjectObject->pimpl);
  Py_RETURN_NONE;
}


PyObject*    PyTest::setObjectValue(PyTest::DPyObject* self, PyObject* args, PyObject *kwds)
{ 
  PyDObject::DPyObject*        dobjectObject;

  if (!PyArg_ParseTuple(args, "O", &dobjectObject))
    return (0);

  if (!PyObject_TypeCheck(dobjectObject, PyDObject::pyType()))
    return (0);


  self->pimpl->setObjectValue(dobjectObject->pimpl);
  Py_RETURN_NONE;
}


PyObject*    PyTest::getObjectValue(PyTest::DPyObject* self, PyObject* args, PyObject *kwds)
{
  Destruct::DObject* dobject = self->pimpl->getObjectValue();

  PyDObject::DPyObject*  dobjectObject = (PyDObject::DPyObject*)_PyObject_New(PyDObject::pyType());
  dobjectObject->pimpl = dobject;

  Py_INCREF(dobjectObject);

  return (Py_BuildValue("O", dobjectObject));
}

PyMethodDef PyTest::pyMethods[] = 
{
  {"run", (PyCFunction)run, METH_NOARGS, "Run basic cpp test."},
  {"showAttribute", (PyCFunction)showAttribute, METH_VARARGS, "Show DStruct attribute."},
  {"showObjectAttribute", (PyCFunction)showObjectAttribute, METH_VARARGS, "Show DObject attribute and value."},
  {"setObjectValue", (PyCFunction)setObjectValue, METH_VARARGS, "Set default val to a DObject (Test transfer of python object to CPP)."},
  {"getObjectValue", (PyCFunction)getObjectValue, METH_NOARGS, "Get a DObject with setted value (Test transfer of CPP object to Python)."},
  {NULL}
};

PyMODINIT_FUNC init_dtest(void)
{
  PyObject* module;

  PyTest::moduleInit();

  if ((module = Py_InitModule3("_dtest", destruct_methods, "Test library for destruct")) == NULL)
    return;    

  DESTRUCT_ADD_MODULE(PyTest::pyType(), "Test")
}
