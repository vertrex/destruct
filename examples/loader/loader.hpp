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

class EXPORT Loader // CLoader / PyLoader inherit CLoader
{
public:
  Loader(void);
  ~Loader();

  bool  loadFile(const std::string& filePath);
  bool  loadDirectory(const std::string& directoryPath);
  void  registerDStructs(std::vector<Destruct::DStruct*>&);
private:
  void  __showDestruct(void) const;
  Destruct::DStructs&  __destruct;
  //ifdef WIN32
  std::vector<void*>   __libraries;

};

#endif
