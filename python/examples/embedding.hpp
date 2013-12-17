#ifndef EMBEDDING
#define EMBEDDING

#include "diterator.hpp"

class PythonInterpreter
{
public:
  PythonInterpreter();
  void loadModule(std::string path);
  void registerStructures(void);
  ~PythonInterpreter();
};

#endif
