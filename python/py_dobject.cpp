#include "py_dobject.hpp"
#include "py_dstruct.hpp"
#include "py_dtype.hpp"

#include "dvalue.hpp"
#include "dstructs.hpp"
#include "dobject.hpp"
#include "drealvalue.hpp"
#include "dexception.hpp"

#include "protocol/dcppobject.hpp"
#include "protocol/diterator.hpp"
#include "protocol/dcontainer.hpp"
#include "protocol/dmutablestruct.hpp"
#include "protocol/dmutableobject.hpp"

using Destruct::DUnicodeString;

template<>
PySequenceMethods* PyDObjectT::pySequenceMethods = NULL;
template<>
PyMappingMethods* PyDObjectT::pyMappingMethods = NULL;

PyTypeObject*     PyDObject::pyType(void)
{
  static PyTypeObject* pyType = (PyTypeObject*)malloc(sizeof(basePyType));
  return (pyType);
}

Destruct::DValue PyDObject::toDValue(PyObject* value) 
{
  if (PyObject_TypeCheck(value, PyDObject::pyType()))
  {
    Destruct::DObject* v = ((DPyObject*)value)->pimpl;
    return Destruct::RealValue<Destruct::DObject* >(v);
  }
  if (value == Py_None)
    return Destruct::RealValue<Destruct::DObject* >(Destruct::DNone); 
  throw Destruct::DException("Can't cast to DObject*");
}

PyObject*     PyDObject::asDValue(Destruct::DValue const& v)
{
  Destruct::DObject*     value = v.get<Destruct::DObject*>();

  if (value == NULL || value == Destruct::DNone)
    Py_RETURN_NONE;
 
  PyTypeObject* pyType = PyDObject::pyType(); 
  Py_INCREF(pyType);
  PyDObject::DPyObject*  dobjectObject = (PyDObject::DPyObject*)_PyObject_New(pyType);
  value->addRef(); 
  dobjectObject->pimpl = value;

  return ((PyObject*)dobjectObject);
}

PyObject*     PyDObject::asPyObject(PyObject* self, int32_t attributeIndex)
{
  Destruct::DObject*     value = ((PyDObject::DPyObject*)self)->pimpl->getValue(attributeIndex).get<Destruct::DObject*>();

  if (value == NULL || value == Destruct::DNone)
    Py_RETURN_NONE;
  
  PyTypeObject* pyType = PyDObject::pyType(); 
  Py_INCREF(pyType);
  PyDObject::DPyObject*  dobjectObject = (PyDObject::DPyObject*)_PyObject_New(pyType);
  //incref ? // decref here 
  value->addRef(); 
  dobjectObject->pimpl = value; //don't addref because .get<> already add a Ref
  //Py_DECREF(dobjectObject) //XXX ;???? destroy refcount

  return ((PyObject*)dobjectObject);
}

PyDObject::PyDObject()
{
  PyTypeObject* pyType = PyDObject::pyType();
  memcpy(pyType, &basePyType, sizeof(basePyType));

  pyType->ob_type = &PyType_Type;
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

  /**
   *  This cause a real problem when doing if toto.a: print 'exists'
   *  it will be  return False because object.c PyObject_IsTrue will first 
   *  get result of the tp_as_mapping_length that will return 0 before
   *  testing other possiblities ! 
   *  sequence and method must not be mapped if it doesn't exist in the object
   *  we rather should use __map__ __seq__ dynamically
   */
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

  PyDStruct::DPyObject* dstructObject = (PyDStruct::DPyObject*)_PyObject_New(PyDStruct::pyType());
  dstructObject->pimpl = (Destruct::DStruct*)dstruct;
  
  return ((PyObject*)dstructObject);
}

PyObject* PyDObject::clone(PyDObject::DPyObject* self)
{
  CHECK_PIMPL
  Destruct::DObject* dobject = self->pimpl->clone();

  PyDObject::DPyObject* pyDObject = (PyDObject::DPyObject*)_PyObject_New(PyDObject::pyType());
  pyDObject->pimpl = (Destruct::DObject*)dobject;
   
  return ((PyObject*)pyDObject);
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
  else if (PyUnicode_Check(attributeObject))
  {
    attributeName = PyUnicode_AS_DATA(attributeObject); 
    attributeIndex = self->pimpl->instanceOf()->findAttribute(std::string(attributeName));
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
    DUnicodeString errorString = "destruct.DObject." + self->pimpl->instanceOf()->name();
    if (attributeName)
      errorString += std::string(" instance has no attribute '") + attributeName + "'";
    else
      errorString += " instance has no attribute at this index '";
    PyErr_SetString(PyExc_AttributeError, errorString.c_str()); 
    return (0);
  }

  size_t typeId = self->pimpl->instanceOf()->attribute(attributeIndex).type().getType();
  if ((typeId >= Destruct::DType::DUnknownType) || (DValueDispatchTable[typeId] == NULL))
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
  else if (PyUnicode_Check(attributeObject))
  {
    attributeName = PyUnicode_AS_DATA(attributeObject); 
    return (PyDObject::setValue(self, attributeName, valueObject));
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
    Destruct::DUnicodeString errorString = "destruct.DObject." + self->pimpl->instanceOf()->name() +  " instance has no attribute '" + attributeName + "'";

    
    PyErr_SetString(PyExc_AttributeError, errorString.c_str()); 

    return (0);
  }

  return (PyDObject::setValue(self, attributeIndex, valueObject));
}

PyObject*  PyDObject::setValue(PyDObject::DPyObject* self, int32_t attributeIndex, PyObject* valueObject)
{
  if (attributeIndex == -1 || attributeIndex >= (int32_t)self->pimpl->instanceOf()->attributeCount())
  {
    Destruct::DUnicodeString errorString = "destruct.DObject." + self->pimpl->instanceOf()->name() + " instance has no attribute at index '"; 
    errorString += attributeIndex; 
    errorString += std::string("'");
    PyErr_SetString(PyExc_AttributeError, errorString.c_str()); 
    return (0);
  }
  
  size_t typeId = self->pimpl->instanceOf()->attribute(attributeIndex).type().getType();
  try
  {
    if ((typeId >= Destruct::DType::DUnknownType) || (DValueDispatchTable[typeId] == NULL))
    { 
      PyErr_SetString(PyExc_RuntimeError, "Bad type id value");
      return (0);
    }
    
    if (typeId == Destruct::DType::DMethodType)
    {
      if (PyCallable_Check(valueObject))
      {
        //Special case get = vector.get  toto.setValue("func", get")
        //->get : c++ -> PyDMethodObject 
        //setValue PyDMethodObject -> DPythonMethodObject ! -> rewrapper une deuxieme fois en python must cast directly to c+
        if (PyObject_TypeCheck(valueObject, PyDMethodObject::pyType()))
        {
          PyDMethodObject::DPyObject* funcObject = (PyDMethodObject::DPyObject*)valueObject;
          self->pimpl->setValue(attributeIndex, Destruct::RealValue<Destruct::DFunctionObject*>(funcObject->pimpl));
        }
        else
        {
          Destruct::DFunctionObject* dpythonMethodObject = new DPythonMethodObject((PyObject*)self, valueObject, attributeIndex); //new donc doit XXX del si ecraser par un set value ou object detruit
          self->pimpl->setValue(attributeIndex, Destruct::RealValue<Destruct::DFunctionObject* >(dpythonMethodObject));
        }
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
  {"clone", (PyCFunction)clone, METH_NOARGS, "Return a clone of this object."},
  {"getType",  (PyCFunction)getType, METH_CLASS, "Return self DType::type."},
  {"getValue", (PyCFunction)getValue, METH_O, "Get value for attribute (passed as index or string)."},
  {"setValue", (PyCFunction)setValueObject, METH_VARARGS, "Set attribute value."},
  {"__dir__", (PyCFunction)_dir, METH_VARARGS, "Return object attributes"},
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

  Destruct::DStruct* dstruct = Destruct::DStructs::instance().find(std::string(dstructName));
  if (dstruct != NULL)
  {
    Py_ssize_t pos = 0;
    PyObject *key, *value = NULL;

    if (pyArgs != NULL)
    {
      //XXX Add init type ds dstruct ou DObject par default  ? XXX XXX XXX 
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

    while (PyDict_Next(((PyObject*)self)->ob_type->tp_dict, &pos, &key, &value))
    {
      int32_t attributeIndex = -1;
      const char* ckey = PyString_AsString(key);

      if ((attributeIndex = self->pimpl->instanceOf()->findAttribute(ckey)) != -1)
        setValue(self, attributeIndex, value);
    } 

    return (0);
  }
 //self incref ?

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
  {
    return (obj);
  }

  PyErr_Clear();
  obj = PyObject_GenericGetAttr((PyObject*)self, args);

  return (obj);
}

int  PyDObject::_setattr(PyDObject::DPyObject* self, PyObject* name, PyObject* valueObject)
{

  const char* cname = PyString_AsString(name);

  PyObject* res = setValue(self, cname, valueObject);
  if (res == Py_None)
  {
    return (0);
  }

  PyErr_Clear();
  int setAttrRes = PyObject_GenericSetAttr((PyObject*)self, name, valueObject);

  return (setAttrRes);
}

PyObject* PyDObject::_repr(PyDObject::DPyObject* self)
{
  CHECK_PIMPL
  Destruct::DUnicodeString repr = "<destruct.DObject." + self->pimpl->instanceOf()->name() + " object>";
  return (PyString_FromString(repr.c_str()));
}

/* This only make sense when pointer is equal */
int PyDObject::_compare(PyDObject::DPyObject* self, PyDObject::DPyObject* other)
{
  if ((self->pimpl - other->pimpl) == 0)
    return (0);
  if ((self->pimpl - other->pimpl) > 0)
    return (1);
  return (-1);
}

static int
merge_list_attr(PyObject* dict, PyObject* obj, const char *attrname)
{
    PyObject *list;
    int result = 0;

    assert(PyDict_Check(dict));
    assert(obj);
    assert(attrname);

    list = PyObject_GetAttrString(obj, attrname);
    if (list == NULL)
        PyErr_Clear();

    else if (PyList_Check(list)) {
        int i;
        for (i = 0; i < PyList_GET_SIZE(list); ++i) {
            PyObject *item = PyList_GET_ITEM(list, i);
            if (PyString_Check(item)) {
                result = PyDict_SetItem(dict, item, Py_None);
                if (result < 0)
                    break;
            }
        }
        if (Py_Py3kWarningFlag &&
            (strcmp(attrname, "__members__") == 0 ||
             strcmp(attrname, "__methods__") == 0)) {
            if (PyErr_WarnEx(PyExc_DeprecationWarning,
                           "__members__ and __methods__ not "
                           "supported in 3.x", 1) < 0) {
                Py_XDECREF(list);
                return -1;
            }
        }
    }

    Py_XDECREF(list);
    return result;
}

static int
merge_class_dict(PyObject* dict, PyObject* aclass)
{
    PyObject *classdict;
    PyObject *bases;

    assert(PyDict_Check(dict));
    assert(aclass);

    /* Merge in the type's dict (if any). */
    classdict = PyObject_GetAttrString(aclass, "__dict__");
    if (classdict == NULL)
        PyErr_Clear();
    else {
        int status = PyDict_Update(dict, classdict);
        Py_DECREF(classdict);
        if (status < 0)
            return -1;
    }

    /* Recursively merge in the base types' (if any) dicts. */
    bases = PyObject_GetAttrString(aclass, "__bases__");
    if (bases == NULL)
        PyErr_Clear();
    else {
        /* We have no guarantee that bases is a real tuple */
        Py_ssize_t i, n;
        n = PySequence_Size(bases); /* This better be right */
        if (n < 0)
            PyErr_Clear();
        else {
            for (i = 0; i < n; i++) {
                int status;
                PyObject *base = PySequence_GetItem(bases, i);
                if (base == NULL) {
                    Py_DECREF(bases);
                    return -1;
                }
                status = merge_class_dict(dict, base);
                Py_DECREF(base);
                if (status < 0) {
                    Py_DECREF(bases);
                    return -1;
                }
            }
        }
        Py_DECREF(bases);
    }
    return 0;
}

PyObject*  PyDObject::_dir(PyDObject::DPyObject* self, PyObject* args, PyObject *kwds )
{
    PyObject* obj = (PyObject*)self;
    PyObject *result = NULL;
    PyObject *dict = NULL;
    PyObject *itsclass = NULL;

    /* Get __dict__ (which may or may not be a real dict...) */
    dict = PyObject_GetAttrString(obj, "__dict__");
    if (dict == NULL) {
        PyErr_Clear();
        dict = PyDict_New();
    }
    else if (!PyDict_Check(dict)) {
        Py_DECREF(dict);
        dict = PyDict_New();
    }
    else {
        /* Copy __dict__ to avoid mutating it. */
        PyObject *temp = PyDict_Copy(dict);
        Py_DECREF(dict);
        dict = temp;
    }

    if (dict == NULL)
    {
       Py_XDECREF(itsclass);
       Py_XDECREF(dict);
       return result;
    }

    /* Merge in __members__ and __methods__ (if any).
     * This is removed in Python 3000. */
    if (merge_list_attr(dict, obj, "__members__") < 0)
    {
       Py_XDECREF(itsclass);
       Py_XDECREF(dict);
       return result;
    }
    if (merge_list_attr(dict, obj, "__methods__") < 0)
    {
       Py_XDECREF(itsclass);
       Py_XDECREF(dict);
       return result;
    }

    /* Merge in attrs reachable from its class. */
    itsclass = PyObject_GetAttrString(obj, "__class__");
    if (itsclass == NULL)
        /* XXX(tomer): Perhaps fall back to obj->ob_type if no
                       __class__ exists? */
        PyErr_Clear();
    else 
    {
      if (merge_class_dict(dict, itsclass) != 0)
      {

       Py_XDECREF(itsclass);
       Py_XDECREF(dict);
       return result;
      }
    }
    result = PyDict_Keys(dict);

    Destruct::DStruct* instance = self->pimpl->instanceOf();
    for (size_t index = 0; index < instance->attributeCount(); ++index)
    {
      Destruct::DUnicodeString name = instance->attribute(index).name(); 
      PyList_Append(result, PyString_FromString(name.c_str()));
    }
    /* fall through */
    Py_XDECREF(itsclass);
    Py_XDECREF(dict);
    return result;
}

PyObject* PyDObject::typeObject()
{
  PyTypeObject* pyTypeObject = pyType();
  Py_INCREF((PyObject*)pyTypeObject);
  return ((PyObject*)pyTypeObject);
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
  
    PyDObject::DPyObject*  dobjectObject = (PyDObject::DPyObject*)_PyObject_New(PyDObject::pyType());
    dobjectObject->pimpl = iterator;

    return ((PyObject*)dobjectObject);
  }
  catch (Destruct::DException const& exception)
  {
    const Destruct::DUnicodeString error = self->pimpl->instanceOf()->name() + " is not iterable\n" + exception.error();
    PyErr_SetString(PyExc_TypeError, error.c_str());
    return (NULL);
  }
}

PyObject* PyDObject::_iternext(PyDObject::DPyObject* self)
{
  try 
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
  catch (Destruct::DException const& exception) 
  {
    const Destruct::DUnicodeString error = self->pimpl->instanceOf()->name() + " is not iterable : " + exception.error();
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
  catch (Destruct::DException)// const& exception)
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
    Destruct::DMutableObject* argument = static_cast<Destruct::DMutableObject*>(Destruct::DStructs::instance().generate("DMutable"));
   
    Destruct::DType pushType = self->pimpl->instanceOf()->attribute("push").type();
    Destruct::DValue itemValue = DValueDispatchTable[pushType.getArgumentType()]->toDValue(item);

    argument->setValueAttribute(Destruct::DType::DInt64Type, "index", Destruct::RealValue<DInt64>(index));
    argument->setValueAttribute(pushType.getArgumentType(), "item", itemValue);
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
    std::cout << "_setmap del item not implemented" << std::endl; //XXX
    return (0);
  }
  else
  {
    try 
    {
// pour les vecteur
      Destruct::DMutableObject* argument = static_cast<Destruct::DMutableObject*>(Destruct::DStructs::instance().generate("DMutable"));
  
      Destruct::DType  getType = self->pimpl->instanceOf()->attribute("get").type();
      Destruct::DType::Type_t  valueType = getType.getReturnType();
      Destruct::DType::Type_t  keyType = getType.getArgumentType();

      Destruct::DValue key = DValueDispatchTable[keyType]->toDValue(_key);
      Destruct::DValue value = DValueDispatchTable[valueType]->toDValue(_value);

      argument->setValueAttribute(keyType, "index", key);
      argument->setValueAttribute(valueType, "value", value);

      self->pimpl->call("setItem", Destruct::RealValue<Destruct::DObject*>(argument));
      argument->destroy(); //seem to be not enough other must havbe forget to call destroy 
/*
pour les map set key& value(
      Destruct::DObject* item = self->pimpl->call("newItem").get<Destruct::DObject*>();
      Destruct::DStruct* itemStruct = item->instanceOf();
        
      Destruct::DType::Type_t keyType = itemStruct->attribute("key").type().getType();
      Destruct::DType::Type_t valueType = itemStruct->attribute("value").type().getType();

      Destruct::DValue key = DValueDispatchTable[keyType]->toDValue(_key);
      Destruct::DValue value = DValueDispatchTable[valueType]->toDValue(_value);

      item->setValue("key", key);
      item->setValue("value", value);

      self->pimpl->call("setItem", Destruct::RealValue<Destruct::DObject*>(item));
      item->destroy(); //
*/
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
