#ifndef __DESTRUCT_LOADER__
#define __DESTRUCT_LOADER__

#include <string>
#include <iostream>

#include "destruct.hpp"
#include "dstructs.hpp"
#include "dstruct.hpp"

#ifdef WIN32
#include "windows.h"
#else
#include <dlfcn.h>
#endif

class Loader // CLoader / PyLoader inherit CLoader
{
public:
  EXPORT Loader(void);

  EXPORT bool  loadFile(const std::string& filePath);
  EXPORT bool  loadDirectory(const std::string& directoryPath);
  EXPORT void  registerDStructs(std::vector<Destruct::DStruct*>&);
private:
  void  __showDestruct(void) const;
  Destruct::DStructs&  __destruct;
};

#endif
