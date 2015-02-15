#include "py_dstruct.hpp"
#include "py_dobject.hpp"
#include "py_workerpool.hpp"

#include "protocol/dcppobject.hpp"
#include "examples/threading/workerpool.hpp"
#include "examples/threading/threading.hpp"

template<>
PyTypeObject* PyExamplesT::pyType = NULL;

PyExamples::PyExamples()
{
  pyType = (PyTypeObject*)malloc(sizeof(basePyType));
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

PyMethodDef PyExamples::pyMethods[] = 
{
  {NULL}
};

PyMODINIT_FUNC init_workerpool(void)
{
  PyObject* module;
  //

  WorkerPool::Declare();
  //Destruct::DStruct* workerPool = Destruct::makeNewDCpp<WorkerPool>("WorkerPool");
  //Destruct::DStructs::instance().registerDStruct(workerPool);


  PyExamples::moduleInit();
  //
  if ((module = Py_InitModule3("_workerpool", destruct_methods, "Load C++ worker pool")) == NULL)
    return;    
  //
  DESTRUCT_ADD_MODULE(PyExamples::pyType, "WorkerPool")
}
