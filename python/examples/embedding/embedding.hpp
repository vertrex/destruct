#ifndef EMBEDDING
#define EMBEDDING

class PythonInterpreter
{
public:
  PythonInterpreter();
  ~PythonInterpreter();

  void          import(void);
  PyObject*     getPythonObject(void);
  void          showObject(PyObject*);
private:
  PyObject*     __pyModule;
};

#endif
