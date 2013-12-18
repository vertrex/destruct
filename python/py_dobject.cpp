#include "py_dobject.hpp"
#include "destruct.hpp"
#include "dobject.hpp"
#include "drealvalue.hpp"
#include "dexception.hpp"

#include "dmemberpointer.hpp"
#include "dcppobject.hpp"
#include "protocol/diterator.hpp"
#include "protocol/dcontainer.hpp"

#include "py_dstruct.hpp"
#include "py_dtype.hpp"

using Destruct::DUnicodeString;

template<>
PyTypeObject* PyDObjectT::pyType = NULL;

PyDObject::PyDObject()
{
  pyType = (PyTypeObject*)malloc(sizeof(basePyType));
  memcpy(pyType , &basePyType , sizeof(basePyType));

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

/* XXX test iterator */
  pyType->tp_iter = (getiterfunc)PyDObject::_iter;
  pyType->tp_iternext = (iternextfunc)PyDObject::_iternext;

  if (PyType_Ready(pyType) < 0)
    throw std::string("PyType ready error");
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
    errorString += attributeIndex + "'";
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
    
//XXX ca casse tous ce booooo code 
    if (typeId == Destruct::DType::DMethodType)
    {
      if (PyCallable_Check(valueObject))  ///XXX set a callable function 
      {
        Destruct::DFunctionObject* dpythonMethodObject = new DPythonMethodObject((PyObject*)self, valueObject, self->pimpl->instanceOf()->attribute(attributeIndex).type()); //new donc doit XXX del ! objet tous ca ! refcount ?
        self->pimpl->setValue(attributeIndex, Destruct::RealValue<Destruct::DFunctionObject* >(dpythonMethodObject));  //?XXX DFUNCTION SET VALUE  to DVALUE ?
        Py_RETURN_NONE; 
      }
      throw std::string("Can't cast to DMethod"); //XXX
    }
    else
      self->pimpl->setValue(attributeIndex, DValueDispatchTable[typeId]->toDValue(valueObject));
    Py_RETURN_NONE;
  }
  //catch (std::string e)//text with d exce[tion
  //{
  //PyErr_SetString(PyExc_AttributeError, e.c_str());
  //return (0);
  //}
 //XXX
  catch (Destruct::DException exception)
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

  if (!PyArg_ParseTuple(args, "s", &dstructName))
  {
    PyErr_SetString(PyExc_TypeError, "must be string or integer, and a compatible DType or Python Object");
    return (-1);
  }

  Destruct::DStruct* dstruct = Destruct::Destruct::instance().find(std::string(dstructName));
  if (dstruct != NULL)
  {
    Py_ssize_t pos = 0;
    PyObject *key, *value = NULL;

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

/* XXX iterator implem **/

// XXX call la factory 
PyObject* PyDObject::_iter(PyDObject::DPyObject* self) //herite de py DContainer ? directement possible ? sans tous reecrre pour DObject comme ca seul les DCOntainer on une methode iter 
{
//USE DYNAMIC CST ET HERITAGE OUR LES PERF ? et si marche pas utilise call poru etre sur ?
//si je cast l hertiage marchera encore ? ou ca va call les method c ++ ?
  if (self->pimpl)
  {
   
     //XXX PERD L HERITAGE PYTHON XXX XXX XXX XXX XXX XXX / car c pas un CPP Object ! de toute il pourais psa le savoir !

    Destruct::IContainer* container = dynamic_cast<Destruct::IContainer* >(self->pimpl);
    Destruct::DObject* iterator = NULL;
    container = NULL; //XXX container jamais NULL ! a cause heritage qui herite voir avec un vrai pure iterator sans destruct mais c pas possible :)
    if (container)     
    {
       std::cout << "PYTHON FIND AND CONVERT TO DCONTAINER // DITERABLE !!! " << std::endl; 
      std::cout << "PyDObject::_iter return CPP Container of " << self->pimpl->instanceOf()->name() << std::endl; //XXX REUTNR TJRS ICI 
      iterator = container->iterator();
    }
    else
    {
            //std::cout << "PyDObject::_iter return DObject container of " << self->pimpl->instanceOf()->name() << std::endl;
      try 
      {
        Destruct::DValue value = self->pimpl->call("iterator", Destruct::RealValue<Destruct::DObject*>(Destruct::DNone));  //XXX si pas d iterator ??? renvoyer problem XXX 
        iterator = value.get<Destruct::DObject*>();
      }
      catch (std::string error)
      {
        std::cout <<  error << std::endl;
      }
    }

    if (iterator)
    {
      PyDObject::DPyObject*  dobjectObject = (PyDObject::DPyObject*)_PyObject_New(PyDObject::pyType);
      dobjectObject->pimpl = iterator;
      return ((PyObject*)dobjectObject);
    }
  }
  //return (NULL); //XXX return proper error
  Py_RETURN_NONE;
}

PyObject* PyDObject::_iternext(PyDObject::DPyObject* self)
{
   Destruct::DObject* iteratorObject = self->pimpl;

   Destruct::RealValue<DInt8> isDone(iteratorObject->call("isDone", Destruct::RealValue<Destruct::DObject* >(Destruct::DNone)).get<DInt8>());

   if (!isDone)
   {
     Destruct::DValue result = iteratorObject->call("currentItem", Destruct::RealValue<Destruct::DObject* >(Destruct::DNone));
    iteratorObject->call("next", Destruct::RealValue<Destruct::DObject* >(Destruct::DNone));
   return PyString_FromString(result.get<DUnicodeString>().c_str());
   //return PythonBaseModule::dvalueAsPyObject(result); //slow TEMPLATE ETC ? 
   }
  
   return NULL;
//if isDone raise iter iteration !!!!!!!!!!!!!!!!!!!!
//return PyDString::asPyObject(result);
   //XXX pouvoir TEMPLATE car ca pass de 30 a 5 sec. !!! donc les call a cote c que dalle !!!!
}
