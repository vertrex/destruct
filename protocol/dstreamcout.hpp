#ifndef DSTRUCT_DSTREAMCOUT_HPP_
#define DSTRUCT_DSTREAMCOUT_HPP_

#include <stdint.h>
#include <iostream>
#include <fstream>
#include <sstream>

#include "destruct.hpp"
#include "dcppobject.hpp"

namespace Destruct
{

class DStreamCout : public DCppObject<DStreamCout>
{
public:
  DStreamCout(DStruct* dstruct, DValue const& args);
  DStreamCout(const DStreamCout& copy);

  DBuffer  read(DValue const& args); 
  DInt64   write(DValue const& args);
//  DValue  seek(DValue const& args);
 
protected: 
  ~DStreamCout();
public:
  RealValue<DFunctionObject* > _read;
  RealValue<DFunctionObject* > _write;

  static size_t ownAttributeCount()
  {
    return (2);
  }

  static DAttribute* ownAttributeBegin()
  {
    static DAttribute  attributes[] = 
    {
       DAttribute(DType::DBufferType, "read",  DType::DInt64Type), 
       DAttribute(DType::DInt64Type,  "write", DType::DBufferType),
    };
    return (attributes);
  }

  static DPointer<DStreamCout>* memberBegin()
  {
    static DPointer<DStreamCout> memberPointer[] = 
    {
       DPointer<DStreamCout>(&DStreamCout::_read, &DStreamCout::read),
       DPointer<DStreamCout>(&DStreamCout::_write, &DStreamCout::write),
    };
    return (memberPointer);
  }

  static DAttribute* ownAttributeEnd()
  {
    return (ownAttributeBegin() + ownAttributeCount());
  }

  static DPointer<DStreamCout>*  memberEnd()
  {
    return (memberBegin() + ownAttributeCount());
  } 
};

}
#endif
