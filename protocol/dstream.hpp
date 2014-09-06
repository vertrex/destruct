#ifndef DSTRUCT_DSTREAM_HPP_
#define DSTRUCT_DSTREAM_HPP_

#include <stdint.h>
#include <iostream>
#include <fstream>
#include <sstream>

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
  DStream(DStruct* dstruct, DValue const& args);
  DStream(const DStream& copy);
  
  virtual DStream& operator>>(DStream& output);
  virtual DStream& operator<<(DStream& input);

  virtual DStream& operator>>(std::string& val); 
  virtual DStream& operator<<(std::string val);
  virtual DStream& operator<<(char val);
  virtual DStream& operator<<(StandardEndLine func);
  virtual DStream& read(char*  buff, uint32_t size);
  virtual DStream& write(const char* buff, uint32_t size);
  virtual bool fail(void);
  //protected:
  virtual ~DStream();
 // DStream(DStruct* dstruct);
private:
  std::fstream  __fstream; //if not fd !
 /*
 *  DStruct declaration
 */ 
public:
  DStream(DStruct* dstruct);
  RealValue<DFunctionObject* > _read;
  RealValue<DFunctionObject* > _write;

  virtual DInt64 write(DValue const& args);
  virtual DInt64 read(DValue const& args);
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
  DStream& operator>>(std::string& val);
  DStream& operator<<(std::string val);
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
  DStreamString(DStruct* dstruct, DValue const &args);
  DStreamString(const DStreamString& copy);
  DStream&          read(char*  buff, uint32_t size);
  DStream&          write(const char* buff, uint32_t size);
  const std::string str(void) const;
  void              clear(void);    
  //DStream& operator>>(std::string& val); 
  //DStream& operator<<(std::string val);
  //virtual DInt64 write(DValue const& args);
  //virtual DInt64 read(DValue const& args);
  //protected:
  ~DStreamString();
private:
  std::stringstream   __stream; 
};

}


#endif
