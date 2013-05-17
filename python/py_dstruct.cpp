#include "dstruct.hpp"
#include "dsimpleobject.hpp"
#include "dattribute.hpp"
#include "dobject.hpp"
#include "py_dstruct.hpp"
#include "py_dattribute.hpp"
#include "py_dobject.hpp"

template<>
PyTypeObject* PyDStructT::pyType = NULL;

PyDStruct::PyDStruct()
{
  pyType = (PyTypeObject*)malloc(sizeof(basePyType));
  memcpy(pyType , &basePyType , sizeof(basePyType));

  pyType->tp_name = "destruct.DStruct";
  pyType->tp_basicsize = sizeof(PyDStruct::DPyObject);
  pyType->tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE;
  pyType->tp_doc = "destruct.DStruct objects";
  pyType->tp_methods = PyDStruct::pyMethods;
  pyType->tp_methods = pyMethods;
  pyType->tp_init = (initproc)PyDStruct::_init;
  pyType->tp_new = PyDStruct::_new;
  pyType->tp_dealloc = (destructor)PyDStruct::_dealloc;
  pyType->tp_repr = (reprfunc)PyDStruct::repr;

  if (PyType_Ready(pyType) < 0)
    throw std::string("PyType ready error");
}

PyObject* PyDStruct::name(PyDStruct::DPyObject* self, PyObject* args, PyObject* kwds)
{
  CHECK_PIMPL 
  return (Py_BuildValue("s", self->pimpl->name().c_str())); 
}

PyObject* PyDStruct::repr(PyDStruct::DPyObject* self)
{
  std::string repr = "<destruct.DStruct." + self->pimpl->name() + " object>";
  return (PyString_FromString(repr.c_str()));
}

PyObject* PyDStruct::attributeCount(PyDStruct::DPyObject* self, PyObject* args, PyObject* kwds)
{
  size_t count = self->pimpl->attributeCount();
  return (Py_BuildValue("i", count)); 
}

PyObject* PyDStruct::findAttribute(PyDStruct::DPyObject* self, PyObject* args, PyObject* kwds)
{
  const char* dstructName;

  if (!PyArg_ParseTuple(args, "s", &dstructName))
    return (0);

  int index = self->pimpl->findAttribute(std::string(dstructName));

  if (index == -1)
    Py_RETURN_NONE;

  return(Py_BuildValue("i", index)); 
}

PyObject* PyDStruct::attribute(PyDStruct::DPyObject* self, PyObject* args, PyObject* kwds)
{
  size_t index;

  if (!PyArg_ParseTuple(args, "i", &index))
    return (0);

  if (index >= self->pimpl->attributeCount())
    Py_RETURN_NONE;

  Destruct::DAttribute* dattribute = new Destruct::DAttribute(self->pimpl->attribute(index));
  CHECK_ALLOC(dattribute)
  PyDAttribute::DPyObject* dattributeObject = (PyDAttribute::DPyObject*) _PyObject_New(PyDAttribute::pyType);
  dattributeObject->pimpl = dattribute;

  return(Py_BuildValue("O", dattributeObject));
}

PyObject* PyDStruct::addAttribute(PyDStruct::DPyObject* self, PyObject* args, PyObject* kwds)
{
  PyDAttribute::DPyObject* dattributeObject;

  if (!PyArg_ParseTuple(args, "O", &dattributeObject))
  {
    return (0);
  }

  if (!PyObject_TypeCheck(dattributeObject, PyDAttribute::pyType))
    return (0);

  //if FIX == 0?
  self->pimpl->addAttribute(*(dattributeObject->pimpl));
  Py_RETURN_NONE;
}

PyObject* PyDStruct::newObject(PyDStruct::DPyObject* self, PyObject* args, PyObject* kwds)
{
  Destruct::DObject*      dobject = self->pimpl->newObject();
  CHECK_ALLOC(dobject)

  PyDObject::DPyObject*   dobjectObject = (PyDObject::DPyObject*)_PyObject_New((PyTypeObject*)PyDObject::pyType);
  dobjectObject->pimpl = dobject;

  return ((PyObject*)dobjectObject);
}

PyMethodDef PyDStruct::pyMethods[] = 
{
  {"name", (PyCFunction)name, METH_NOARGS, "Return the name of the DStruct."},
  {"attributeCount", (PyCFunction)attributeCount, METH_NOARGS, "Return the number of attribute."},
  {"findAttribute", (PyCFunction)findAttribute, METH_VARARGS, "Search an index of an DAttribute object."},
  {"attribute", (PyCFunction)attribute, METH_VARARGS, "Search and return an DAttribute object.Return None if not found"},
  {"addAttribute", (PyCFunction)addAttribute, METH_VARARGS, "Add an attribute to the struct."},
  {"newObject", (PyCFunction)newObject, METH_VARARGS, "Return a DObject construct from this definition."},
  {NULL}
};

void PyDStruct::_dealloc(PyDStructT::DPyObject* self)
{
  self->pimpl = NULL; //XXX use refcount or copy ? once registred in Destruct it must not be deleted
  self->ob_type->tp_free((PyObject* )self);
}

/*
 *  Change input to DStruct(std::string base, std::string name) ?
 */
int PyDStruct::_init(PyDStructT::DPyObject *self, PyObject *args, PyObject *kwds)
{
   const char*                  name = NULL;
   Destruct::DStruct*           base = NULL;
   PyObject*                    baseObject = NULL;

   if (!PyArg_ParseTuple(args, "Os", &baseObject, &name))
     return (-1);

   if (baseObject && PyObject_TypeCheck(baseObject, PyDStruct::pyType))
   {
     Py_INCREF(baseObject);
     base = ((PyDStruct::DPyObject*)baseObject)->pimpl;
   }

   if (name == NULL || std::string(name) == "")
   {
     PyErr_SetString(PyExc_ValueError, "name must not be empty");
     return (-1);
   }

   self->pimpl = new Destruct::DStruct(base, std::string(name), Destruct::DSimpleObject::newObject);
   INIT_CHECK_ALLOC(self->pimpl)

   return (0);
}