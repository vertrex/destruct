#ifndef __DESTRUCT_LOADER__
#define __DESTRUCT_LOADER__

#include <string>
#include <iostream>

#include "destruct.hpp"
//#include "dstructs.hpp"
//#include "dstruct.hpp"

#include "dcppobject.hpp"

#ifdef WIN32
#include "windows.h"
#else
#include <dlfcn.h>
#endif

namespace Destruct
{

// CImport / PyImport inherit CImport
class Import : public DCppObject<Import> 
{
public:
  Import(DStruct* dstruct, DValue const& args);
  Import(const Import& copy);

  ~Import();

  DUInt8                file(DValue const& filePath);
  DUInt8                directory(DValue const& directoryPath);
private:
  void                  __registerDStructs(std::vector<Destruct::DStruct*>&);
  void                  __showDestruct(void) const;
  Destruct::DStructs&   __destruct;

  std::vector<void*>    __libraries;
public:
  RealValue<DFunctionObject* > _file, _directory;

  static size_t ownAttributeCount()
  {
    return (2);
  }

  static DAttribute* ownAttributeBegin()
  {
    static DAttribute  attributes[] = 
    {
       DAttribute(DType::DUInt8Type, "file",  DType::DUnicodeStringType), 
       DAttribute(DType::DUInt8Type,  "directory", DType::DUnicodeStringType),
    };
    return (attributes);
  }

  static DPointer<Import>* memberBegin()
  {
    static DPointer<Import> memberPointer[] = 
    {
       DPointer<Import>(&Import::_file, &Import::file),
       DPointer<Import>(&Import::_directory, &Import::directory),
    };
    return (memberPointer);
  }

  static DAttribute* ownAttributeEnd()
  {
    return (ownAttributeBegin() + ownAttributeCount());
  }

  static DPointer<Import>*  memberEnd()
  {
    return (memberBegin() + ownAttributeCount());
  } 
};

}
#endif
