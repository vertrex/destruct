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
    Output,
    Input
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

/*
 *  Basic stream object used for serialization test
 *      - Could be enhanced / replaced by file, buffer (data), socket protocol object for reflectivy
 */
//XXX this is dstream file interface other dont use __fstream
//DStreamFile 
/*
class DStream : public DCppObject<DStream> //DStreamFile deriver d'une DStream sans membre private
{
public:
  enum whence
  {
    Set,
    Current,
    End,
  };

  enum mode //AddAttribute(Enum()); 
  {
    Output,
    Input
  };

  typedef std::basic_ostream<char, std::char_traits<char> > CoutType;
  typedef CoutType& (*StandardEndLine)(CoutType&);

  EXPORT DStream(DStruct* dstruct);
  EXPORT DStream(DStruct* dstruct, DValue const& args);
  EXPORT DStream(const DStream& copy);
  
  EXPORT virtual DStream& operator>>(DStream& output); 
  EXPORT virtual DStream& operator<<(DStream& input);

  EXPORT virtual DStream& operator>>(DUnicodeString& val);  //=0 in base
  EXPORT virtual DStream& operator<<(DUnicodeString val); //=0 in base
  EXPORT virtual DStream& operator<<(char val); //=0 in base
  EXPORT virtual DStream& operator<<(StandardEndLine func);
  EXPORT virtual DStream& read(char*  buff, uint32_t size);
  EXPORT virtual DStream& write(const char* buff, uint32_t size);
  EXPORT virtual bool fail(void); //=0 in base

  EXPORT virtual DInt64  write(Destruct::DValue const& value);// { return (0);}; //sub clas me ? => call read/write as virtual 0 (comme ds dobject) 
  EXPORT virtual DInt64  read(Destruct::DValue const& value);// { return (0);};  //sub class me ? != can be dobject / dstruct etc not just int / string ..!
  //protected:
  EXPORT virtual ~DStream();
 // DStream(DStruct* dstruct);
private:
  std::fstream  __fstream; //if not fd !
public:
  RealValue<DFunctionObject* > _read;
  RealValue<DFunctionObject* > _write;

  //EXPORT virtual DInt64 write(DValue const& args); //=0 in base
  //EXPORT virtual DInt64 read(DValue const& args);  //=0 in base

  static size_t ownAttributeCount()
  {
    return (0);
  }

  static DAttribute* ownAttributeBegin()
  {
    static DAttribute  attributes[] = 
    {
       DAttribute(DType::DInt64Type,"read", DType::DBufferType), 
       DAttribute(DType::DInt64Type,"write",  DType::DBufferType),
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
*/

/*
class DStreamString : public DStream //Use destruct imheritance ??
{
public:
  EXPORT DStreamString(DStruct* dstruct, DValue const &args);
  EXPORT DStreamString(const DStreamString& copy);
  EXPORT DStream&          read(char*  buff, uint32_t size);
  EXPORT DStream&          write(const char* buff, uint32_t size);

  using DStream::write;
  using DStream::read;
  EXPORT const std::string str(void) const;
  EXPORT void              clear(void);    
  //DStream& operator>>(std::string& val); 
  //DStream& operator<<(std::string val);
  //virtual DInt64 write(DValue const& args);
  //virtual DInt64 read(DValue const& args);
  //protected:
  EXPORT ~DStreamString();
private:
  std::stringstream   __stream; 
};
*/
}

#endif
