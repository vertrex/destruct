#ifndef __DESTRUCT_LOADER__
#define __DESTRUCT_LOADER__

#include <string>
#include <iostream>

#include "destruct.hpp"
#include "dcppobject.hpp"

#ifdef WIN32
#include "windows.h"
#else
#include <dlfcn.h>
#endif

namespace Destruct
{

/* CImport / PyImport inherit CImport
 * Import method Declare in C files
 */
class Import : public DCppObject<Import> 
{
public:
  Import(DStruct* dstruct, DValue const& args);
  Import(const Import& copy);

  ~Import();

  DUInt8                file(DValue const& filePath);
  DUInt8                directory(DValue const& directoryPath);
  void                  unload(void);
private:
  void                  __registerDStructs(std::vector<Destruct::DStruct*>&);
  Destruct::DStructs&   __destruct;

  std::vector<void*>    __libraries;
public:
  RealValue<DFunctionObject* > _file, _directory, _unload;

  static size_t ownAttributeCount()
  {
    return (3);
  }

  static DAttribute* ownAttributeBegin()
  {
    static DAttribute  attributes[] = 
    {
       DAttribute(DType::DUInt8Type, "file",  DType::DUnicodeStringType), 
       DAttribute(DType::DUInt8Type, "directory", DType::DUnicodeStringType),
       DAttribute(DType::DNoneType,  "unload", DType::DNoneType), //Must unload only a lib with provided name allowing to reload it
       //Must provide a way to access list of currently loaded library
    };
    return (attributes);
  }

  static DPointer<Import>* memberBegin()
  {
    static DPointer<Import> memberPointer[] = 
    {
       DPointer<Import>(&Import::_file, &Import::file),
       DPointer<Import>(&Import::_directory, &Import::directory),
       DPointer<Import>(&Import::_unload, &Import::unload),
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

  /** 
   *  Import is a singleton
   */
  static DObject* newObject(Destruct::DStruct* dstruct, DValue const& args)
  {
    static DObject* import = new Import(dstruct, args);
    import->addRef();
    return (import);
  }

  virtual DObject* clone() const
  {
    DObject* import = static_cast<DObject*>(const_cast<Import*>(this));
    import->addRef();
    return (import);
  } 
};

}
#endif
