#ifndef _DESTRUCTMODULE_REGISTRYRPC_HPP_
#define _DESTRUCTMODULE_REGISTRYRPC_HPP_

#include <Python.h>
#include <frameobject.h>

#include "destructmodule.hpp"
#include "loader.hpp"

#define DESTRUCT_ADD_MODULE(moduleType, moduleName)\
  Py_INCREF(moduleType);\
  PyModule_AddObject(module, moduleName, (PyObject*)moduleType);


class PyCLoader
{
public:
  PyCLoader() : __loader()
  {
    //Destruct::DType::init();
    //Registry::declare();
  };

  
  ~PyCLoader()
  {
    //Destruct::DType::clean();
  }

  void loadFile(std::string const& filePath)
  {
    std::cout << "Loading from python" << std::endl;
    this->__loader.loadFile(filePath);
  }
private:
  Loader       __loader;
};

typedef PythonModule< class PyLoader, PyCLoader > PyLoaderT;
class PyLoader : public PyLoaderT 
{
public:
                      PyLoader();
  static PyMethodDef  pyMethods[];
  static PyTypeObject*     pyType(void);

  static PyObject* loadFile(DPyObject* self, PyObject* args, PyObject *kwds);
};


static PyMethodDef destruct_methods[] = 
{
  { NULL, NULL, 0, NULL }
};

#endif
