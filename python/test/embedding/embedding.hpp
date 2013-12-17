#ifndef EMBEDDING
#define EMBEDDING


class PythonInterpreter
{
public:
  PythonInterpreter();
  void loadModule(std::string path);
  void registerStructures(void);
  ~PythonInterpreter();
};

#endif
