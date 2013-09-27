#ifndef _DESTRUCT_MODULE_DSTREAM_HPP_
#define _DESTRUCT_MODULE_DSTREAM_HPP_

#include "dstream.hpp"
#include "destructmodule.hpp"

typedef PythonModule< class PyDStream, Destruct::DStream> PyDStreamT;
class PyDStream : public PyDStreamT
{
public:
  static DPyObject      pyObject;
  static PyMethodDef    pyMethods[];

                        PyDStream();
  static int            _init(DPyObject* self, PyObject* args, PyObject* kwds);
  //static PyDStream&   read(char* buff, uint32_t size);
  //static PyDStream&   write(const char* buff, uint32_t size);
};

#endif
