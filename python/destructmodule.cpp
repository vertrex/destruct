#include "destructmodule.hpp"
#include "py_destruct.hpp"
#include "py_dstruct.hpp"
#include "py_dattribute.hpp"
#include "py_dtype.hpp"
#include "py_dobject.hpp"
#include "py_dstream.hpp"
#include "py_dserialize.hpp"
#include "py_destruct_test.hpp"

#define DESTRUCT_ADD_MODULE(moduleType, moduleName)\
  Py_INCREF(moduleType);\
  PyModule_AddObject(module, moduleName, (PyObject*)moduleType);

static PyMethodDef destruct_methods[] = 
{
  { NULL, NULL, 0, NULL }
};


PyMODINIT_FUNC init_destruct(void)
{
  PyObject* module;

  PyDestruct::moduleInit();
  PyDStruct::moduleInit();
  PyDAttribute::moduleInit();
  PyDObject::moduleInit();
  PyDType::moduleInit();
  PyDStream::moduleInit();
  PyDSerialize::moduleInit();
  PyTest::moduleInit();

  if ((module = Py_InitModule3("_destruct", destruct_methods, "The destruct library")) == NULL)
    return;    

  DESTRUCT_ADD_MODULE(PyDestruct::pyType, "Destruct")
  DESTRUCT_ADD_MODULE(PyDStruct::pyType, "DStruct")
  DESTRUCT_ADD_MODULE(PyDAttribute::pyType, "DAttribute")

//type specialization
  DESTRUCT_ADD_MODULE(PyDInt8::pyType, "DInt8")
  DESTRUCT_ADD_MODULE(PyDInt16::pyType, "DInt16")
  DESTRUCT_ADD_MODULE(PyDInt32::pyType, "DInt32")
  DESTRUCT_ADD_MODULE(PyDInt64::pyType, "DInt64")
 
  DESTRUCT_ADD_MODULE(PyDUInt8::pyType, "DUInt8")
  DESTRUCT_ADD_MODULE(PyDUInt16::pyType, "DUInt16")
  DESTRUCT_ADD_MODULE(PyDUInt32::pyType, "DUInt32")
  DESTRUCT_ADD_MODULE(PyDUInt64::pyType, "DUInt64")
  DESTRUCT_ADD_MODULE(PyDUnicodeString::pyType, "DUnicodeString")
  DESTRUCT_ADD_MODULE(PyDObject::pyType, "DObject")
//end type specialization 

  DESTRUCT_ADD_MODULE(PyDType::pyType, "DType")
  DESTRUCT_ADD_MODULE(PyDStream::pyType, "DStream");
  DESTRUCT_ADD_MODULE(PyDSerialize::pyType, "DSerialize");
  DESTRUCT_ADD_MODULE(PyTest::pyType, "Test")

}

PyMethodDef   PythonTypeBaseModule::baseTypePyMethods[2] = 
{
  {"getType", NULL, METH_CLASS, "Return the type."},
  { NULL, NULL, 0, NULL }
};

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

