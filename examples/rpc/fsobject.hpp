#ifndef __FSOBJECT__
#define __FSOBJECT__

#include "protocol/dcppobject.hpp"

using namespace Destruct;

class File : public DCppObject<File>
{
public:
  File(DStruct* dstruct, DValue const& args);
  DUnicodeString        path(DValue const& args);

  RealValue<DUInt64 >           size;
  RealValue<DUnicodeString >    name;
  RealValue<DFunctionObject* >  _path;
 //parent? 
/*
 *  DStruct declaration
 */ 
  static size_t ownAttributeCount()
  {
    return (3);
  }

  static DAttribute* ownAttributeBegin()
  {
    static DAttribute  attributes[] = 
    {
      DAttribute(DType::DInt64Type,"size"), 
      DAttribute(DType::DUnicodeStringType, "name"),
      DAttribute(DType::DUnicodeStringType, "path", DType::DNoneType),
    };
    return (attributes);
  }

  static DPointer<File>* memberBegin()
  {
    static DPointer<File> memberPointer[] = 
    {
      DPointer<File>(&File::size),
      DPointer<File>(&File::name),
      DPointer<File>(&File::_path, &File::path),
    };
    return (memberPointer);
  }

  static DAttribute* ownAttributeEnd()
  {
    return (ownAttributeBegin() + ownAttributeCount());
  }

  static DPointer<File >*  memberEnd()
  {
    return (memberBegin() + ownAttributeCount());
  } 
};

class Directory : public DCppObject<Directory>
{
public:
  Directory(DStruct* dstruct, DValue const& args);
  DUnicodeString        path(DValue const& args);

  RealValue<DUnicodeString >    name;
  RealValue<DFunctionObject* >  _path;
  RealValue<DObject* >          children;
/*
 *  DStruct declaration
 */ 
  static size_t ownAttributeCount()
  {
    return (3);
  }

  static DAttribute* ownAttributeBegin()
  {
    static DAttribute  attributes[] = 
    {
      DAttribute(DType::DUnicodeStringType, "name"),
      DAttribute(DType::DUnicodeStringType, "path", DType::DNoneType),
      DAttribute(DType::DObjectType, "children"), 
    };
    return (attributes);
  }

  static DPointer<Directory>* memberBegin()
  {
    static DPointer<Directory> memberPointer[] = 
    {
      DPointer<Directory>(&Directory::name),
      DPointer<Directory>(&Directory::_path, &Directory::path),
      DPointer<Directory>(&Directory::children),
    };
    return (memberPointer);
  }

  static DAttribute* ownAttributeEnd()
  {
    return (ownAttributeBegin() + ownAttributeCount());
  }

  static DPointer<Directory >*  memberEnd()
  {
    return (memberBegin() + ownAttributeCount());
  } 
};

#endif
