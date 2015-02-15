#ifndef __DESTRUCT_LOADER__
#define __DESTRUCT_LOADER__

#include <string>
#include <iostream>

#include "dstructs.hpp"
#include "dstruct.hpp"

class Loader // CLoader / PyLoader inherit CLoader
{
public:
  Loader(void);

  bool  loadFile(const std::string& filePath);
  bool  loadDirectory(const std::string& directoryPath);
  void  registerDStructs(std::vector<Destruct::DStruct*>&);
private:
  void  __showDestruct(void) const;

  Destruct::DStructs&  __destruct;
};

#endif
