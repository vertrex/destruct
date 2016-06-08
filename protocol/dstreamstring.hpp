#ifndef DSTRUCT_DSTREAMSTRING_HPP_
#define DSTRUCT_DSTREAMSTRING_HPP_

#include <stdint.h>
#include <iostream>
#include <fstream>
#include <sstream>

#include "destruct.hpp"
#include "dcppobject.hpp"

namespace Destruct
{

class DStreamString : public DCppObject<DStreamString>
{
public:
  DStreamString(DStruct* dstruct, DValue const& args);
  DStreamString(const DStreamString& copy);

  DBuffer         read(DValue const& args); 
  DInt64          write(DValue const& args);
  DUnicodeString  str(void);
  void            clear(void);
 
protected: 
  ~DStreamString();
private:
  std::stringstream   __stream; 
public:
  RealValue<DFunctionObject* > _read, _write, _str, _clear;

  static size_t ownAttributeCount()
  {
    return (4);
  }

  static DAttribute* ownAttributeBegin()
  {
    static DAttribute  attributes[] = 
    {
       DAttribute(DType::DBufferType, "read",  DType::DInt64Type),
       DAttribute(DType::DInt64Type,  "write", DType::DBufferType),
       DAttribute(DType::DUnicodeStringType, "string", DType::DNoneType),
       DAttribute(DType::DNoneType, "clear", DType::DNoneType),
       //       size ->buffer size ?
       //       seek
    };
    return (attributes);
  }

  static DPointer<DStreamString>* memberBegin()
  {
    static DPointer<DStreamString> memberPointer[] = 
    {
       DPointer<DStreamString>(&DStreamString::_read, &DStreamString::read),
       DPointer<DStreamString>(&DStreamString::_write, &DStreamString::write),
       DPointer<DStreamString>(&DStreamString::_str, &DStreamString::str),
       DPointer<DStreamString>(&DStreamString::_clear, &DStreamString::clear),
    };
    return (memberPointer);
  }

  static DAttribute* ownAttributeEnd()
  {
    return (ownAttributeBegin() + ownAttributeCount());
  }

  static DPointer<DStreamString>*  memberEnd()
  {
    return (memberBegin() + ownAttributeCount());
  } 
};

}

#endif
