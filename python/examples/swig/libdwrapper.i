/*
 * DFF -- An Open Source Digital Forensics Framework
 * Copyright (C) 2009-2013 ArxSys
 * This program is free software, distributed under the terms of
 * the GNU General Public License Version 2. See the LICENSE file
 * at the top of the source tree.
 *  
 * See http://www.digital-forensic.org for more information about this
 * project. Please do not directly contact any of the maintainers of
 * DFF for assistance; the project provides a web site, mailing lists
 * and IRC channels for your use.
 * 
 * Author(s):
 *  Solal J. <sja@digital-forensic.org>
 */

#include "pyrun.swg"

%module(package="dff.api.dwrapper") libdwrapper

%include "std_string.i"
%include "std_vector.i"
%include "windows.i"
#ifndef WIN32
%include "stdint.i"
#else
%include "wstdint.i"
#endif

%typemap(in) (Destruct::DObject*) 
{
  if (PyObject_TypeCheck($input, PyDObject::pyType))
  {
    $1 = ((PyDObject::DPyObject*)$input)->pimpl;
    $1->addRef();
  }  
  else
    $1 = Destruct::DNone;
}

%typemap(out) (Destruct::DObject*)// =>set
{
  Py_XDECREF($result);

  PyDObject::DPyObject* dobjectObject = (PyDObject::DPyObject*)_PyObject_New(PyDObject::pyType);
  dobjectObject->pimpl = $1;
  $1->addRef();
  $result = (PyObject*)dobjectObject;
}

%typemap(out) (Destruct::DObject*, Destruct::DObject*)// =>set
{
  Py_XDECREF($result);

  PyDObject::DPyObject* dobjectObject = (PyDObject::DPyObject*)_PyObject_New(PyDObject::pyType);
  dobjectObject->pimpl = $1;
  $1->addRef();
  $result = (PyObject*)dobjectObject;
}

%typemap(in) (Destruct::DValue) 
{
  $1 = Destruct::DValue(PythonBaseModule::pyObjectToDValue($input));
}

//convertie un Dvalue en PyObject => set
%typemap(out) (Destruct::DValue)
{
  Py_XDECREF($result);
  $result = PythonBaseModule::dvalueAsPyObject($1);
}

%{
#include "../destruct/python/py_dvalue.hpp"
#include "../destruct/python/py_dobject.hpp"
#include "dwrapper.hpp"
#include <typeinfo>
%}
%include "dwrapper.hpp"
