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
//  DValue  seek(DValue const& args);
  
  ~DStream();
private:
  std::fstream  __stream; 
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

  static DPointer<DStream>* memberBegin()
  {
    static DPointer<DStream> memberPointer[] = 
    {
       DPointer<DStream>(&DStream::_read, &DStream::read),
       DPointer<DStream>(&DStream::_write, &DStream::write),
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

class DStreamCout : public DCppObject<DStreamCout>
{
public:
  DStreamCout(DStruct* dstruct, DValue const& args);
  DStreamCout(const DStreamCout& copy);

  DBuffer  read(DValue const& args); 
  DInt64   write(DValue const& args);
//  DValue  seek(DValue const& args);
  
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

class DStreamString : public DCppObject<DStreamString>
{
public:
  DStreamString(DStruct* dstruct, DValue const& args);
  DStreamString(const DStreamString& copy);

  DBuffer         read(DValue const& args); 
  DInt64          write(DValue const& args);
  DUnicodeString  str(void);
  void            clear(void);
  
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
