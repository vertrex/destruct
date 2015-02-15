#include <python2.7/Python.h>
#include <python2.7/frameobject.h>

#include "destructmodule.hpp"
#include "py_dstructs.hpp"
#include "py_dstruct.hpp"
#include "py_dattribute.hpp"
#include "py_dtype.hpp"
#include "py_dobject.hpp"
#include "py_dmethodobject.hpp"
#include "py_dnullobject.hpp"
#include "py_dserialize.hpp"

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

  PyEval_InitThreads();

  PyDStructs::moduleInit();
  PyDStruct::moduleInit();
  PyDAttribute::moduleInit();
  PyDObject::moduleInit();
  PyDMethodObject::moduleInit();
  PyDNullObject::moduleInit();
  PyDType::moduleInit();
  //PyDStream::moduleInit();
  PyDSerialize::moduleInit();
  //PyTest::moduleInit();

  if ((module = Py_InitModule3("_destruct", destruct_methods, "The destruct library")) == NULL)
    return;    

  DESTRUCT_ADD_MODULE(PyDStructs::pyType, "DStructs")
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
  DESTRUCT_ADD_MODULE(PyDMethodObject::pyType, "DMethod")
  DESTRUCT_ADD_MODULE(PyDNullObject::pyType, "DNone")
//end type specialization 

  DESTRUCT_ADD_MODULE(PyDType::pyType, "DType")
  DESTRUCT_ADD_MODULE(PyDSerialize::pyType, "DSerialize");
  //DESTRUCT_ADD_MODULE(PyTest::pyType, "Test")
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

PySequenceMethods PythonBaseModule::baseSequenceMethods =
{
    0,  /* inquiry sq_length;              __len__ */
    0,  /* binaryfunc sq_concat;           __add__ */
    0,  /* intargfunc sq_repeat;           __mul__ */
    0,  /* intargfunc sq_item;             __getitem__ */
    0,  /* intintargfunc sq_slice;         __getslice__ */
    0,  /* intobjargproc sq_ass_item;      __setitem__ */
    0,  /* intintobjargproc sq_ass_slice;  __setslice__ */
};

PyMappingMethods PythonBaseModule::baseMappingMethods =
{
 0, /* mp_length*/
 0, /* mp_subscript*/
 0, /* mp_ass_subscript*/
};

int PythonBaseModule::pyTracebackInternalAsString(PyTracebackObject* tb, std::string& errorMessage, long limit)
{
  int err = 0;
  long depth = 0;
  PyTracebackObject *tb1 = tb;

  while (tb1 != NULL) 
  {
    depth++;
    tb1 = tb1->tb_next;
  }
  while (tb != NULL && err == 0) 
  {
    if (depth <= limit) 
    {
      std::stringstream errorStream;

      errorStream << "  File \"" << std::string(PyString_AsString(tb->tb_frame->f_code->co_filename)) << "\"";
      errorStream << ", line " << tb->tb_lineno;
      errorStream <<  ", in " << std::string(PyString_AsString(tb->tb_frame->f_code->co_name)) << "\n";
      errorMessage += errorStream.str();
    }
    depth--;
    tb = tb->tb_next;
    err = PyErr_CheckSignals();
  }

  return (err);
}

int PythonBaseModule::pyTracebackAsString(PyObject* tb, std::string& errorMessage)
{
  int err;
  PyObject* limitv;
  long limit = 1000;
  
  if (tb == NULL)
    return (0); 
  if (!PyTraceBack_Check(tb))
  {
    errorMessage += "System Error. Bad argument to internal function.";
    return (-1);
  }
  limitv = PySys_GetObject((char*)"tracebacklimit");
  if (limitv && PyInt_Check(limitv))
  {
    limit = PyInt_AsLong(limitv);
    if (limit <= 0)
      return (0);
  }
  errorMessage += "Traceback (most recent call last):\n";
  err = pyTracebackInternalAsString((PyTracebackObject*)tb, errorMessage, limit);

  return (err);
}

const std::string  PythonBaseModule::pyErrorAsString(void) 
{
  PyObject* exception, *v, *tb;
  int err = 0;
  std::string errorMessage = "";

  PyErr_Fetch(&exception, &v, &tb);
  if (exception == NULL)
  {
    PyErr_Clear();
    return std::string("Can't get python exception : unknow type\n"); 
  }
  PyErr_NormalizeException(&exception, &v, &tb);
  if (exception == NULL)
  {
    PyErr_Clear();
    return std::string("Can't get python exception : unknow type\n"); 
  }

  if (tb && tb != Py_None)
    err = pyTracebackAsString(tb, errorMessage);

  if (!err)
  {
    if(PyExceptionClass_Check(exception))
    {
      PyObject* moduleName;
      char* className = PyExceptionClass_Name(exception);
      if (className != NULL)
      {
        char* dot = strrchr(className, '.');
        if (dot != NULL)
          className = dot+1;
      }

      moduleName = PyObject_GetAttrString(exception, "__module__");
      if (moduleName == NULL)
        errorMessage += std::string("<unknown>");
      else 
      {
        char * modstr = PyString_AsString(moduleName);
        if (modstr && strcmp(modstr, "exceptions"))
        {
          errorMessage += std::string(modstr);
          errorMessage += std::string(".");
        }
        Py_DECREF(moduleName);
      } 
      if (className == NULL)
        errorMessage += std::string("<unknown>");
      else
        errorMessage += std::string(className);
    }
    else
    {
       PyObject* s = PyObject_Str(exception);
       errorMessage += std::string(PyString_AsString(s));
       Py_XDECREF(s);
    }
  }
 
  if (err == 0 && (v != Py_None))
  {
    PyObject* s = PyObject_Str(v);
    
    if (s == NULL)
      err = 1;
    else if (!PyString_Check(s) || PyString_GET_SIZE(s) != 0)
       errorMessage += ": "; 
    errorMessage += std::string(PyString_AsString(s));
    Py_XDECREF(s);
  }

  Py_XDECREF(exception);
  Py_XDECREF(v);
  Py_XDECREF(tb);
  PyErr_Clear();
 
  return (errorMessage);
}

//XXX optimized ? 
Destruct::DValue PythonBaseModule::pyObjectToDValue(PyObject* object)
{
  PyObject* objectTypeId = PyObject_CallMethod(object, (char *)"getType", NULL);
  if (objectTypeId != NULL)
  {
    long typeId = PyLong_AsLong(objectTypeId);
    if (typeId < Destruct::DType::DUnknownType)
      return Destruct::DValue(DValueDispatchTable[typeId]->toDValue(object));// -de check mais 1 call en plus possibilite de faire mieux avec unarg static ? objectTypeId.type ?
  }
  PyErr_Clear();
    //XXX a utiliser pour forcer le type int8, int16, ... car pas gerer la !      
//table:yDObject::pyType pyoject->type ? dynamic 
  if (PyObject_TypeCheck(object, PyDObject::pyType))
    return Destruct::RealValue<Destruct::DObject* >(((PyDObject::DPyObject*)object)->pimpl);
  else if (PyLong_Check(object) || PyInt_Check(object))
  {
    //std::cout << "Return default to DInt64 " << std::endl; //XXX
    return (Destruct::RealValue<DInt64>(PyLong_AsLong(object)));
  }
  else if (PyString_Check(object)) 
    return Destruct::RealValue<Destruct::DUnicodeString>(std::string(PyString_AsString(object)));
  else if (PyObject_TypeCheck(object, PyDMethodObject::pyType))
    return Destruct::RealValue<Destruct::DFunctionObject*>(((PyDMethodObject::DPyObject*)object)->pimpl);
  return (Destruct::RealValue<Destruct::DObject*>(Destruct::DNone));
}

PyObject* PythonBaseModule::dvalueAsPyObject(Destruct::DValue const& value)
{
  size_t typeId = 0;

  for (; typeId < 12; typeId++)
  {
    try
    {
      return DValueDispatchTable[typeId]->asDValue(value);
      break;
    }
    catch(...)
    {
    }
  }

 Py_RETURN_NONE;
}
