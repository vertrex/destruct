#ifndef DSTRUCT_DSTREAM_HPP_
#define DSTRUCT_DSTREAM_HPP_

#include <stdint.h>
#include <iostream>
#include <fstream>
#include <sstream>

#include "destruct.hpp"
#include "dcppobject.hpp"
#include "protocol/dstreambase.hpp"

namespace Destruct
{


/*
 *  Basic stream object used for serialization test
 *      - Could be enhanced / replaced by file, buffer (data), socket protocol object for reflectivy
 */
//XXX this is dstream file interface other dont use __fstream
//DStreamFile 
class DStream : public DStreamBase,  public DCppObject<DStream> //DStreamFile deriver d'une DStream sans membre private
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
  EXPORT DStream(DStruct* dstruct, DValue const& args);
  EXPORT DStream(const DStream& copy);
  
  EXPORT virtual DStream& operator>>(DStream& output);
  EXPORT virtual DStream& operator<<(DStream& input);

  EXPORT virtual DStream& operator>>(DUnicodeString& val); 
  EXPORT virtual DStream& operator<<(DUnicodeString val);
  EXPORT virtual DStream& operator<<(char val);
  EXPORT virtual DStream& operator<<(StandardEndLine func);
  EXPORT virtual DStream& read(char*  buff, uint32_t size);
  EXPORT virtual DStream& write(const char* buff, uint32_t size);
  EXPORT virtual bool fail(void);
  //protected:
  EXPORT virtual ~DStream();
 // DStream(DStruct* dstruct);
private:
  std::fstream  __fstream; //if not fd !
 /*
 *  DStruct declaration
 */ 
public:
  EXPORT DStream(DStruct* dstruct);
  RealValue<DFunctionObject* > _read;
  RealValue<DFunctionObject* > _write;

  EXPORT virtual DInt64 write(DValue const& args);
  EXPORT virtual DInt64 read(DValue const& args);

  static size_t ownAttributeCount()
  {
    return (0);
  }

  static DAttribute* ownAttributeBegin()
  {
    static DAttribute  attributes[] = 
    {
       DAttribute(DType::DInt64Type,"read", DType::DObjectType), 
       DAttribute(DType::DInt64Type,"write",  DType::DObjectType),
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

class DStreamCout : public DStream //Base
{
public:
  DStreamCout(DStruct* dstruct, DValue const &args);
  DStreamCout(const DStreamCout& copy);
  DStream& operator>>(DUnicodeString& val);
  DStream& operator<<(DUnicodeString val);
  DStream& operator<<(char val);
  DStream& operator<<(StandardEndLine func);
  DStream& read(char*  buff, uint32_t size);
  DStream& write(const char* buff, uint32_t size);
  bool fail(void);
protected:
  ~DStreamCout();
};

class DStreamString : public DStream //Base
{
public:
  EXPORT DStreamString(DStruct* dstruct, DValue const &args);
  EXPORT DStreamString(const DStreamString& copy);
  EXPORT DStream&          read(char*  buff, uint32_t size);
  EXPORT DStream&          write(const char* buff, uint32_t size);
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

}

#endif
