#include "py_dobject.hpp"
#include "dvalue.hpp"
#include "destruct.hpp"
#include "dobject.hpp"
#include "drealvalue.hpp"
#include "dexception.hpp"

#include "protocol/dcppobject.hpp"
#include "protocol/diterator.hpp"
#include "protocol/dcontainer.hpp"
#include "protocol/dmutablestruct.hpp"
#include "protocol/dmutableobject.hpp"

#include "py_dstruct.hpp"
#include "py_dtype.hpp"

using Destruct::DUnicodeString;

template<>
PyTypeObject* PyDObjectT::pyType = NULL;
template<>
PySequenceMethods* PyDObjectT::pySequenceMethods = NULL;
template<>
PyMappingMethods* PyDObjectT::pyMappingMethods = NULL;

Destruct::DValue PyDObject::toDValue(PyObject* value) 
{
  if (PyObject_TypeCheck(value, PyDObject::pyType))
    return Destruct::RealValue<Destruct::DObject* >(((DPyObject*)value)->pimpl);
  if (value == Py_None)
    return Destruct::RealValue<Destruct::DObject* >(Destruct::DNone); 
  throw Destruct::DException("Can't cast to DObject*");
}

PyObject*     PyDObject::asDValue(Destruct::DValue v)
{
  Destruct::DObject*     value = v.get<Destruct::DObject*>();

  if (value == NULL || value == Destruct::DNone)
    Py_RETURN_NONE;
   
  Py_INCREF(pyType);
  PyDObject::DPyObject*  dobjectObject = (PyDObject::DPyObject*)_PyObject_New(PyDObject::pyType);
  dobjectObject->pimpl = value;

  return ((PyObject*)dobjectObject);
}

PyObject*     PyDObject::asPyObject(PyObject* self, int32_t attributeIndex)
{
  Destruct::DObject*     value = ((PyDObject::DPyObject*)self)->pimpl->getValue(attributeIndex).get<Destruct::DObject*>();

  if (value == NULL || value == Destruct::DNone)
    Py_RETURN_NONE;
   
  Py_INCREF(pyType);
  PyDObject::DPyObject*  dobjectObject = (PyDObject::DPyObject*)_PyObject_New(PyDObject::pyType);
  dobjectObject->pimpl = value;

  return ((PyObject*)dobjectObject);
}

PyDObject::PyDObject()
{
  pyType = (PyTypeObject*)malloc(sizeof(basePyType));
  memcpy(pyType, &basePyType, sizeof(basePyType));

  pyType->tp_name = "destruct.DObject";
  pyType->tp_basicsize = sizeof(DPyObject);
  pyType->tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE;
  pyType->tp_doc = "destruct.DObject objects";
  pyType->tp_methods = PyDObject::pyMethods;
  pyType->tp_init = (initproc)PyDObject::_init;
  pyType->tp_new = PyDObject::_new;
  pyType->tp_dealloc = (destructor)PyDObject::_dealloc;
  pyType->tp_repr = (reprfunc)PyDObject::_repr;
  pyType->tp_getattro = (getattrofunc)PyDObject::_getattr;
  pyType->tp_setattro = (setattrofunc)PyDObject::_setattr;
  pyType->tp_compare = (cmpfunc)PyDObject::_compare; 

  pyType->tp_iter = (getiterfunc)PyDObject::_iter;
  pyType->tp_iternext = (iternextfunc)PyDObject::_iternext;

  pySequenceMethods = (PySequenceMethods*)malloc(sizeof(baseSequenceMethods));
  memcpy(pySequenceMethods, &baseSequenceMethods, sizeof(baseSequenceMethods)); //is empty can bzero
  pyType->tp_as_sequence = pySequenceMethods; //XXX
  pyType->tp_as_sequence->sq_length = (lenfunc)PyDObject::_length;
  pyType->tp_as_sequence->sq_item = (ssizeargfunc)PyDObject::_item;
  pyType->tp_as_sequence->sq_ass_item = (ssizeobjargproc)PyDObject::_setitem;

  pyMappingMethods = (PyMappingMethods*)malloc(sizeof(baseMappingMethods));
  memcpy(pyMappingMethods, &baseMappingMethods, sizeof(baseMappingMethods)); //is empty can bzero
  pyType->tp_as_mapping = pyMappingMethods;
  pyType->tp_as_mapping->mp_length = (lenfunc)PyDObject::_length;
  pyType->tp_as_mapping->mp_subscript = (binaryfunc)PyDObject::_map;
  pyType->tp_as_mapping->mp_ass_subscript  = (objobjargproc)PyDObject::_setmap; 

  if (PyType_Ready(pyType) < 0)
    throw Destruct::DException("PyType ready error");
}

PyObject* PyDObject::instanceOf(PyDObject::DPyObject* self)
{
  CHECK_PIMPL
  const Destruct::DStruct* dstruct = self->pimpl->instanceOf();

  PyDStruct::DPyObject* dstructObject = (PyDStruct::DPyObject*)_PyObject_New(PyDStruct::pyType);
  dstructObject->pimpl = (Destruct::DStruct*)dstruct;
   
  return ((PyObject*)dstructObject);
}

PyObject* PyDObject::getValue(PyDObject::DPyObject* self, PyObject* attributeObject)
{
  int32_t     attributeIndex = 0;
  const char* attributeName = NULL;


  CHECK_PIMPL
  if (PyInt_Check(attributeObject))
  {
    attributeIndex = PyLong_AsLong(attributeObject);
  }
  else if (PyString_Check(attributeObject))
  {
    attributeName = PyString_AsString(attributeObject); 
    attributeIndex = self->pimpl->instanceOf()->findAttribute(std::string(attributeName));
  }
  else
  {
    PyErr_SetString(PyExc_TypeError, "must be string or integer");
    return (NULL);
  }

  if (attributeIndex == -1 || attributeIndex >= (int32_t)self->pimpl->instanceOf()->attributeCount())
  {
    std::string errorString = "destruct.DObject." + self->pimpl->instanceOf()->name() +  " instance as no attribute '" + attributeName + "'";
    PyErr_SetString(PyExc_AttributeError, errorString.c_str()); 
    return (0);
  }

  size_t typeId = self->pimpl->instanceOf()->attribute(attributeIndex).type().getType();
  if ((typeId > Destruct::DType::DUnknownType) || (DValueDispatchTable[typeId] == NULL))
  { 
    PyErr_SetString(PyExc_RuntimeError, "Bad type id value");
    return (0);
  }

  return (DValueDispatchTable[typeId]->asPyObject((PyObject*)self, attributeIndex));
}

PyObject* PyDObject::setValueObject(PyDObject::DPyObject* self, PyObject* args, PyObject* kwds)
{
  PyObject*    valueObject;
  PyObject*    attributeObject;
  int32_t      attributeIndex = 0; 
  const char*  attributeName = NULL;

  if (!PyArg_ParseTuple(args, "OO", &attributeObject, &valueObject))
  {
    PyErr_SetString(PyExc_TypeError, "must be string or integer, and a compatible DType or Python Object");
    return (0);
  }
  CHECK_PIMPL

  if (PyInt_Check(attributeObject))
  {
    attributeIndex = PyLong_AsLong(attributeObject);
    return (PyDObject::setValue(self, attributeIndex, valueObject));
  }
  else if (PyString_Check(attributeObject))
  {
    attributeName = PyString_AsString(attributeObject); 
    return (PyDObject::setValue(self, attributeName, valueObject));
  }
  PyErr_SetString(PyExc_TypeError, "must be string or integer, and a compatible DType or Python Object");
  return (0);
}

PyObject* PyDObject::setValue(PyDObject::DPyObject* self, const char* attributeName, PyObject* valueObject)
{
  int32_t attributeIndex = self->pimpl->instanceOf()->findAttribute(std::string(attributeName));
  if (attributeIndex == -1 || attributeIndex >= (int32_t)self->pimpl->instanceOf()->attributeCount())
  {
    std::string errorString = "destruct.DObject." + self->pimpl->instanceOf()->name() +  " instance as no attribute '" + attributeName + "'";
    PyErr_SetString(PyExc_AttributeError, errorString.c_str()); 
    return (0);
  }

  return (PyDObject::setValue(self, attributeIndex, valueObject));
}

PyObject*  PyDObject::setValue(PyDObject::DPyObject* self, int32_t attributeIndex, PyObject* valueObject)
{
  if (attributeIndex == -1 || attributeIndex >= (int32_t)self->pimpl->instanceOf()->attributeCount())
  {
    std::string errorString = "destruct.DObject." + self->pimpl->instanceOf()->name() +  " instance as no attribute at index '"; 
    errorString += attributeIndex; 
    errorString += std::string("'");
    PyErr_SetString(PyExc_AttributeError, errorString.c_str()); 
    return (0);
  }
  
  size_t typeId = self->pimpl->instanceOf()->attribute(attributeIndex).type().getType();
  try
  {
    if ((typeId > Destruct::DType::DUnknownType) || (DValueDispatchTable[typeId] == NULL))
    { 
      PyErr_SetString(PyExc_RuntimeError, "Bad type id value");
      return (0);
    }
    
    if (typeId == Destruct::DType::DMethodType)
    {
      if (PyCallable_Check(valueObject))
      {
        Destruct::DFunctionObject* dpythonMethodObject = new DPythonMethodObject((PyObject*)self, valueObject, attributeIndex); //new donc doit XXX del si ecraser par un set value ou object detruit
        self->pimpl->setValue(attributeIndex, Destruct::RealValue<Destruct::DFunctionObject* >(dpythonMethodObject));
        Py_RETURN_NONE; 
      }
      PyErr_SetString(PyExc_TypeError, "setValue : object must be callable");
      return (0);
    }
    
    self->pimpl->setValue(attributeIndex, DValueDispatchTable[typeId]->toDValue(valueObject));
    Py_RETURN_NONE;
  }
  catch (Destruct::DException const& exception)
  {
    PyErr_SetString(PyExc_AttributeError, exception.error().c_str());
    return (0);
  }
}

PyMethodDef PyDObject::pyMethods[] = 
{
  {"instanceOf", (PyCFunction)instanceOf, METH_NOARGS, "Return the DStruct definition who generate this object."},
  {"getType",  (PyCFunction)getType, METH_CLASS, "Return self DType::type."},
  {"getValue", (PyCFunction)getValue, METH_O, "Get value for attribute (passed as index or string)."},
  {"setValue", (PyCFunction)setValueObject, METH_VARARGS, "Set attribute value."},
  { NULL }
};

int PyDObject::_init(PyDObjectT::DPyObject* self, PyObject *args, PyObject *kwds)
{
  self->pimpl = NULL;
  char* dstructName = NULL;
  PyObject*     pyArgs = NULL;

  if (!PyArg_ParseTuple(args, "s", &dstructName))
  {
    if (!PyArg_ParseTuple(args, "sO", &dstructName, &pyArgs))
    {
      PyErr_SetString(PyExc_TypeError, "must be string or string and DValue");
      return (-1);
    }
    PyErr_Clear();
  }

  Destruct::DStruct* dstruct = Destruct::Destruct::instance().find(std::string(dstructName));
  if (dstruct != NULL)
  {
    Py_ssize_t pos = 0;
    PyObject *key, *value = NULL;

    if (pyArgs != NULL)
    {
      //XXX
      //Destruct::DValue dstructArgs = Destruct::RealValue<Destruct::DObject*>(Destruct::DNone);
      Destruct::DValue dstructArgs = DValueDispatchTable[9]->toDValue(pyArgs);
      self->pimpl = dstruct->newObject(dstructArgs);
    }
    else
      self->pimpl = dstruct->newObject();

//XXX check tp_dict for each func/value ? (only func ?)
// if func is found set func to the c++ object with setvalue -> overiding the c++ function
// (i think for variable is not a problem as it done in init )
// then when a function will be called it will be set in c++ and python in our dict (getValue / setvalue of destruct)
// so it will return it before searching the dict so it will be the first one called 
// it s a kind of trick but it will work 
// other way is to do that then maybe replace the dict by a call to get attr etc... 
// or maybe delete them from de dict but we must get it first ... ?

//XXX IT WILL BE DONE AT EACH INIT or object() instantiation :*(
//    Better modify the dict ? But how to push the object on destruct def ? (the func) ?
//    Better call saul ? 

    while (PyDict_Next(((PyObject*)self)->ob_type->tp_dict, &pos, &key, &value))
    {
      int32_t attributeIndex = -1;
      const char* ckey = PyString_AsString(key);

      if ((attributeIndex = self->pimpl->instanceOf()->findAttribute(ckey)) != -1)
        setValue(self, attributeIndex, value);
    } 

    return (0);
  }

  std::string errorString = "Can't find " + std::string(dstructName) + " in Destruct database";
  PyErr_SetString(PyExc_RuntimeError, errorString.c_str());
  return (-1);
}

void PyDObject::_dealloc(PyDObjectT::DPyObject* self)
{
   if (self->pimpl)
   {
     self->pimpl->destroy();
     self->pimpl = NULL;
   }
   self->ob_type->tp_free((PyObject*)self);
}

/* 
 *  This control how inheritance between python and dobject work !
 *  If we want python dico to be the last called we must use it's function rather than ours !
 *  It's not sufficient as we must control initialization and binding for c++ at objet initialization !

 en d autre term si on renvoie en premier l objet du dict : 

        A --------> c++
        |
        |
        B --------> python

en python on aura bien access au func surcharger mais pas en c++ car elles auront pas replace celle du model 

2 cas :
        Structure Fixed -> all structure for now :
                           2 choix :
                                - on accepte que python rajoute des variable / function qui seront utilisable uniquement de py a py
                                - on accepte que python overide only les donnes du model Destruct (tres limitant pour les class)

                                - ds le cas ou le dico contient a l init un value definit par destruct il faut la remplacer !
                                  donc check le dico etc ... pour set les fonc pour etre accessible en c++

        Future : structure non-fixed -> pour l instant existe pas

 */

PyObject*    PyDObject::_getattr(PyDObject::DPyObject* self, PyObject* args)
{
  PyObject* obj = PyDObject::getValue(self, args);
  if (obj != NULL)
    return (obj);

  PyErr_Clear();
  return (PyObject_GenericGetAttr((PyObject*)self, args));
}

int  PyDObject::_setattr(PyDObject::DPyObject* self, PyObject* name, PyObject* valueObject)
{
  const char* cname = PyString_AsString(name);

  PyObject* res = setValue(self, cname, valueObject);
  if (res == Py_None)
    return (0);

  PyErr_Clear();
  return (PyObject_GenericSetAttr((PyObject*)self, name, valueObject));
}

PyObject* PyDObject::_repr(PyDObject::DPyObject* self)
{
  CHECK_PIMPL
  std::string repr = "<destruct.DObject." + self->pimpl->instanceOf()->name() + " object>";
  return (PyString_FromString(repr.c_str()));
}

/* This only make sense when pointer is equal */
int PyDObject::_compare(PyDObject::DPyObject* self, PyDObject::DPyObject* other)
{
  return ((int)(self->pimpl - other->pimpl));
}

PyObject* PyDObject::typeObject()
{
  Py_INCREF((PyObject*)pyType);
  return ((PyObject*)pyType);
}

PyObject* PyDObject::getType(PyDObject::DPyObject* self, PyObject* args, PyObject* kwds)
{
  return (PyInt_FromSize_t(Destruct::DType::DObjectType)); 
}

PyObject* PyDObject::_iter(PyDObject::DPyObject* self)
{
  CHECK_PIMPL
  
  try 
  {
    if (self->pimpl->instanceOf()->name() == "DIterator")
    {
      return ((PyObject*)self);
    }
    Destruct::DObject* iterator = self->pimpl->call("iterator").get<Destruct::DObject*>();

    PyDObject::DPyObject*  dobjectObject = (PyDObject::DPyObject*)_PyObject_New(PyDObject::pyType);
    dobjectObject->pimpl = iterator;

    return ((PyObject*)dobjectObject);
  }
  catch (Destruct::DException const& exception)
  {
    const std::string error = self->pimpl->instanceOf()->name() + " is not iterable\n" + exception.error();
    PyErr_SetString(PyExc_TypeError, error.c_str());
    return (NULL);
  }
}

PyObject* PyDObject::_iternext(PyDObject::DPyObject* self)
{
  Destruct::DIterator* iterator = dynamic_cast<Destruct::DIterator *>(self->pimpl);

  try {
 
  if (iterator != NULL)
  {
    Destruct::DFunctionObject* isDoneObject = iterator->isDoneObject;  
    DInt8 isDone(isDoneObject->call().get<DInt8>());
    
    if (!isDone)
    {
      Destruct::DValue result = ((Destruct::DFunctionObject*)iterator->currentItemObject)->call();
      ((Destruct::DFunctionObject*)iterator->nextObject)->call();
      Destruct::DAttribute attribute = self->pimpl->instanceOf()->attribute("currentItem");
      Destruct::DType::Type_t type = attribute.type().getReturnType();
      return (DValueDispatchTable[type]->asDValue(result));
    }
  }
  else
  {
    DInt8 isDone(self->pimpl->call("isDone").get<DInt8>());
    if (!isDone)
    {
      Destruct::DValue result = self->pimpl->call("currentItem");
      self->pimpl->call("nextItem");

      Destruct::DAttribute attribute = self->pimpl->instanceOf()->attribute("currentItem");
      Destruct::DType::Type_t type = attribute.type().getReturnType();
      return (DValueDispatchTable[type]->asDValue(result));
    }
  }
  } 
  catch (Destruct::DException const& exception) 
  {
    const std::string error = self->pimpl->instanceOf()->name() + " is not iterable : " + exception.error();
    PyErr_SetString(PyExc_TypeError, error.c_str());
  }

  return (NULL);
}

Py_ssize_t PyDObject::_length(PyDObject::DPyObject* self)
{
  try 
  { 
    return (self->pimpl->call("size").get<DUInt64>());
  }
  catch (Destruct::DException const& exception)
  {
    return (0);
  }
}

PyObject* PyDObject::_item(PyDObject::DPyObject* self, Py_ssize_t index)
{
  try
  { 
    Destruct::DValue result = self->pimpl->call("get", Destruct::RealValue<DUInt64>(index));
    Destruct::DType type = self->pimpl->instanceOf()->attribute("get").type(); 

    return (DValueDispatchTable[type.getReturnType()]->asDValue(result)); 
  }
  catch (Destruct::DException const& exception)
  {
    PyErr_SetString(PyExc_TypeError, exception.error().c_str());
    return (NULL);
  }
}

int PyDObject::_setitem(PyDObject::DPyObject* self, Py_ssize_t index, PyObject* item)
{
//XXX sert pu a rien !!
  try 
  {
    Destruct::DMutableObject* argument = static_cast<Destruct::DMutableObject*>(Destruct::Destruct::instance().generate("DMutable"));
   
    Destruct::DType pushType = self->pimpl->instanceOf()->attribute("push").type();
    Destruct::DValue itemValue = DValueDispatchTable[pushType.getArgumentType()]->toDValue(item);

    argument->setValueAttribute("index", Destruct::RealValue<DInt64>(index), Destruct::DType::DInt64Type);
    argument->setValueAttribute("item", itemValue, pushType.getArgumentType());
    self->pimpl->call("setItem", Destruct::RealValue<Destruct::DObject*>(argument));
    argument->destroy(); //seem to be not enough other must havbe forget to call destroy 
  }
  catch (Destruct::DException const& exception)
  {
    PyErr_SetString(PyExc_AttributeError, exception.error().c_str());
    return (-1);
  }

  return (0); 
}

int  PyDObject::_setmap(DPyObject* self, PyObject* _key, PyObject* _value)
{
  if (_key == NULL)
  {
    std::cout << "del item not implemented" << std::endl; //XXX
    return (0);
  }
  else
  {
    try 
    {
      Destruct::DMutableObject* argument = static_cast<Destruct::DMutableObject*>(Destruct::Destruct::instance().generate("DMutable"));
  
      Destruct::DType  getType = self->pimpl->instanceOf()->attribute("get").type();
      Destruct::DType::Type_t  valueType = getType.getReturnType();
      Destruct::DType::Type_t  keyType = getType.getArgumentType();

      Destruct::DValue key = DValueDispatchTable[keyType]->toDValue(_key);
      Destruct::DValue value = DValueDispatchTable[valueType]->toDValue(_value);

      argument->setValueAttribute("index", key, keyType);
      argument->setValueAttribute("value", value, valueType);

      self->pimpl->call("setItem", Destruct::RealValue<Destruct::DObject*>(argument));
      argument->destroy(); //seem to be not enough other must havbe forget to call destroy 
    }
    catch (Destruct::DException const& exception)
    {
      PyErr_SetString(PyExc_AttributeError, exception.error().c_str());
      return (-1);
    }

    return (0);
  }
}

PyObject* PyDObject::_map(PyDObject::DPyObject* self, register PyObject* _key)
{
  try
  {
    Destruct::DType  getType = self->pimpl->instanceOf()->attribute("get").type();
    Destruct::DType::Type_t  valueType = getType.getReturnType();
    Destruct::DType::Type_t  keyType = getType.getArgumentType();

    Destruct::DValue key = DValueDispatchTable[keyType]->toDValue(_key);
    Destruct::DValue value = self->pimpl->call("get", key);

    return (DValueDispatchTable[valueType]->asDValue(value)); 
  }
//doit rnevoyer typerror de python quand c une type error
// et keyerorr quand c une keyerror
// pas tout le temps une key error
//catch (Destruct::DContainerException const& exception) //herite dobject XXX
//{
//PyErr_SetString(PyExc_KeyError, exception.error().c_str());
//} 
  catch (Destruct::DException const& exception)
  {
    PyErr_SetString(PyExc_KeyError, exception.error().c_str());
    return (NULL);
  }
}
