#ifndef _DESTRUCTMODULE_REGISTRYRPC_HPP_
#define _DESTRUCTMODULE_REGISTRYRPC_HPP_

#include <python2.7/Python.h>
#include <python2.7/frameobject.h>

#include "destructmodule.hpp"
#include "registry.hpp"
#include "registryclient.hpp"

#define DESTRUCT_ADD_MODULE(moduleType, moduleName)\
  Py_INCREF(moduleType);\
  PyModule_AddObject(module, moduleName, (PyObject*)moduleType);


class PyRegistryClient
{
public:
  PyRegistryClient()
  {
    Destruct::DType::init();
    Registry::declare();
  };

  
  ~PyRegistryClient()
  {
    Destruct::DType::clean();
  }

  DObject* connect(void)
  {
    std::cout << "creating client" << std::endl;
    RegistryClient client("127.0.0.1", 0xdff);
    std::cout << "client start()" << std::endl;
    return (client.start());
  }
  //conect(DPyObject* self, PyObject* args, PyObject *kwds)
  //{
  //
  //}
};

typedef PythonModule< class PyRegistryRPC, PyRegistryClient> PyRegistryRPCT;
class PyRegistryRPC : public PyRegistryRPCT 
{
public:
                      PyRegistryRPC();
  static PyMethodDef  pyMethods[];

  static PyObject*    connect(DPyObject* self, PyObject* args, PyObject *kwds);
  //static PyObject*    showAttribute(DPyObject* self, PyObject* args, PyObject *kwds);
  //static PyObject*    showObjectAttribute(DPyObject* self, PyObject* args, PyObject *kwds);
  //static PyObject*    setObjectValue(DPyObject* self, PyObject* args, PyObject *kwds);
  //static PyObject*    getObjectValue(DPyObject* self, PyObject* args, PyObject *kwds);
};


static PyMethodDef destruct_methods[] = 
{
  { NULL, NULL, 0, NULL }
};

#endif
