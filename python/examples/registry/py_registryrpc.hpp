#ifndef _DESTRUCTMODULE_REGISTRYRPC_HPP_
#define _DESTRUCTMODULE_REGISTRYRPC_HPP_

#include <Python.h>
#include <frameobject.h>

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

  DObject* connect(std::string const& address, uint32_t port)
  {
    RegistryClient client(address, port);
    return (client.start());
  }
};

typedef PythonModule< class PyRegistryRPC, PyRegistryClient> PyRegistryRPCT;
class PyRegistryRPC : public PyRegistryRPCT 
{
public:
                      PyRegistryRPC();
  static PyMethodDef  pyMethods[];
  static PyTypeObject*  pyType(void);

  static PyObject*    connect(DPyObject* self, PyObject* args, PyObject *kwds);
};


static PyMethodDef destruct_methods[] = 
{
  { NULL, NULL, 0, NULL }
};

#endif
