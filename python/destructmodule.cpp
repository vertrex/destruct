#include <Python.h>
#include <frameobject.h>

#include "destructmodule.hpp"
#include "py_dstructs.hpp"
#include "py_dstruct.hpp"
#include "py_dattribute.hpp"
#include "py_dtype.hpp"
#include "py_dobject.hpp"
#include "py_dmethodobject.hpp"
#include "py_dnullobject.hpp"
//#include "py_dserialize.hpp"

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

  if ((module = Py_InitModule3("_destruct", destruct_methods, "The destruct library")) == NULL)
    return;    

  DESTRUCT_ADD_MODULE(PyDStructs::pyType(), "DStructs")
  DESTRUCT_ADD_MODULE(PyDStruct::pyType(), "DStruct")
  DESTRUCT_ADD_MODULE(PyDAttribute::pyType(), "DAttribute")

//type specialization
  DESTRUCT_ADD_MODULE(PyDInt8::pyType(), "DInt8")
  DESTRUCT_ADD_MODULE(PyDInt16::pyType(), "DInt16")
  DESTRUCT_ADD_MODULE(PyDInt32::pyType(), "DInt32")
  DESTRUCT_ADD_MODULE(PyDInt64::pyType(), "DInt64")
 
  DESTRUCT_ADD_MODULE(PyDUInt8::pyType(), "DUInt8")
  DESTRUCT_ADD_MODULE(PyDUInt16::pyType(), "DUInt16")
  DESTRUCT_ADD_MODULE(PyDUInt32::pyType(), "DUInt32")
  DESTRUCT_ADD_MODULE(PyDUInt64::pyType(), "DUInt64")
  DESTRUCT_ADD_MODULE(PyDUnicodeString::pyType(), "DUnicodeString")
  DESTRUCT_ADD_MODULE(PyDBuffer::pyType(), "DBuffer")
  DESTRUCT_ADD_MODULE(PyDOpaque::pyType(), "DOpaque")


  DESTRUCT_ADD_MODULE(PyDObject::pyType(), "DObject")
  DESTRUCT_ADD_MODULE(PyDMethodObject::pyType(), "DMethod")
  DESTRUCT_ADD_MODULE(PyDNullObject::pyType(), "DNone")
//end type specialization 

  DESTRUCT_ADD_MODULE(PyDType::pyType(), "DType")
}

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
  if (PyObject_TypeCheck(object, PyDObject::pyType()))
    return Destruct::RealValue<Destruct::DObject* >(((PyDObject::DPyObject*)object)->pimpl);
  else if (PyLong_Check(object) || PyInt_Check(object))
  {
    //std::cout << "Return default to DInt64 " << std::endl; //XXX
    return (Destruct::RealValue<DInt64>(PyLong_AsLong(object)));
  }
  else if (PyUnicode_Check(object))
  {
    PyObject* utf8Object = PyUnicode_AsUTF8String(object);
    //XXX check utf8Object is not null or raise py exception
    Destruct::DUnicodeString fvalue = std::string(PyString_AsString(utf8Object));
    // decref ytf8object
    return Destruct::RealValue<Destruct::DUnicodeString>(fvalue);
  }
  else if (PyString_Check(object)) 
    return Destruct::RealValue<Destruct::DUnicodeString>(std::string(PyString_AsString(object)));
  else if (PyObject_TypeCheck(object, PyDMethodObject::pyType()))
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
