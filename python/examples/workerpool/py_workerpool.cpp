#include "destructmodule.hpp"
#include "py_dstruct.hpp"
#include "py_dobject.hpp"

#include "py_workerpool.hpp"

#include "destruct.hpp"
#include "protocol/dcppobject.hpp"
#include "examples/threading/workerpool.hpp"
#include "examples/threading/threading.hpp"


#ifdef WIN32
PyTypeObject PythonBaseModule::basePyType =
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

PyExamples::PyExamples()
{
  PyTypeObject* pyType = PyExamples::pyType();
  memcpy(pyType , &basePyType , sizeof(basePyType));

  pyType->tp_name = "destruct.workerpool";
  pyType->tp_basicsize = sizeof(PyExamples::DPyObject);
  pyType->tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE;
  pyType->tp_doc = "destruct.workerpool loader";
  pyType->tp_methods = PyExamples::pyMethods;
  pyType->tp_methods = pyMethods;
  pyType->tp_init = (initproc)_init;
  pyType->tp_new = _new;
  pyType->tp_dealloc = (destructor)_dealloc;

  if (PyType_Ready(pyType) < 0)
    throw std::string("PyType ready error");
}

PyTypeObject*     PyExamples::pyType(void)
{
    static PyTypeObject* pyType = (PyTypeObject*)malloc(sizeof(basePyType));
    return (pyType);
}

PyMethodDef PyExamples::pyMethods[] = 
{
  {NULL}
};

PyMODINIT_FUNC init_workerpool(void)
{
  PyObject* module;

  WorkerPool::Declare();

  PyExamples::moduleInit();
  //
  if ((module = Py_InitModule3("_workerpool", destruct_methods, "Load C++ worker pool")) == NULL)
    return;    
  //
 // DESTRUCT_ADD_MODULE(PyExamples::pyType, "WorkerPool")
}
