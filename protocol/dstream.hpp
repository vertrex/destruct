#ifndef DSTRUCT_DSTREAM_HPP_
#define DSTRUCT_DSTREAM_HPP_

#include <stdint.h>
#include <iostream>
#include <fstream>
#include <sstream>

#include "destruct.hpp"
#include "dcppobject.hpp"

namespace Destruct
{

class DStream : public DCppObject<DStream>
{
public:
  enum whence
  {
    Set,
    Current,
    End,
  };

  enum mode
  {
    Output, //Write
    Input   //Read
  };

  DStream(DStruct* dstruct, DValue const& args);
  DStream(const DStream& copy);

  DBuffer  read(DValue const& args); 
  DInt64   write(DValue const& args);
  void     flush(void);
  void     seek(DValue const& args);
  DUInt64  size(void);
  DUInt64  tell(void);
//DValue  seek(DValue const& args);
  //tell implement for regf  

protected: 
  ~DStream();
private:
  std::fstream  __stream; 
public:
  RealValue<DFunctionObject* > _read, _write, _flush, _seek, _size, _tell;

  static size_t ownAttributeCount()
  {
    return (6);
  }

  static DAttribute* ownAttributeBegin()
  {
    static DAttribute  attributes[] = 
    {
       DAttribute(DType::DBufferType, "read",  DType::DInt64Type), 
       DAttribute(DType::DInt64Type,  "write", DType::DBufferType),
       DAttribute(DType::DNoneType, "flush", DType::DNoneType),
       DAttribute(DType::DUInt64Type, "size", DType::DUInt64Type),
       DAttribute(DType::DNoneType, "seek", DType::DUInt64Type),
       DAttribute(DType::DUInt64Type, "tell", DType::DNoneType),
    };
    return (attributes);
  }

  static DPointer<DStream>* memberBegin()
  {
    static DPointer<DStream> memberPointer[] = 
    {
       DPointer<DStream>(&DStream::_read, &DStream::read),
       DPointer<DStream>(&DStream::_write, &DStream::write),
       DPointer<DStream>(&DStream::_flush, &DStream::flush),
       DPointer<DStream>(&DStream::_size, &DStream::size),
       DPointer<DStream>(&DStream::_seek, &DStream::seek),
       DPointer<DStream>(&DStream::_tell, &DStream::tell),
    };
    return (memberPointer);
  }

  static DAttribute* ownAttributeEnd()
  {
    return (ownAttributeBegin() + ownAttributeCount());
  }

  static DPointer<DStream>*  memberEnd()
  {
    return (memberBegin() + ownAttributeCount());
  } 
};

}

#endif
