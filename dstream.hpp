#ifndef DSTRUCT_DSTREAM_HPP_
#define DSTRUCT_DSTREAM_HPP_

#include <stdint.h>
#include <iostream>
#include <fstream>

namespace Destruct
{

/*
 *  Basic stream object used for serialization test
 *      - Could be enhanced / replaced by file, buffer (data), socket protocol object for reflectivy
 */

class DStream
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
    Input,
    Output
  };

  typedef std::basic_ostream<char, std::char_traits<char> > CoutType;
  typedef CoutType& (*StandardEndLine)(CoutType&);

  DStream();
  DStream(std::string filePath, mode _mode = Input); 
//  DStream(int32_t fd, mode _mode = mode::Input);  for python and C fd ? 
//how to inherit and create new dstream in python ? is it possible ? use it for dff node ? or use other classes ?
  virtual ~DStream();
  virtual DStream& operator>>(std::string& val); 
  virtual DStream& operator<<(std::string val);
  virtual DStream& operator<<(char val);
  virtual DStream& operator<<(StandardEndLine func);
  virtual DStream& read(char*  buff, uint32_t size);
  virtual DStream& write(const char* buff, uint32_t size);
  virtual bool fail(void);
private:
  std::fstream  __fstream; //if not fd !
};

class DStreamCout : public DStream
{
public:
  DStreamCout();
  ~DStreamCout();
  DStream& operator>>(std::string& val);
  DStream& operator<<(std::string val);
  DStream& operator<<(char val);
  DStream& operator<<(StandardEndLine func);
  DStream& read(char*  buff, uint32_t size);
  DStream& write(const char* buff, uint32_t size);
  bool fail(void);
};

static DStreamCout cout; //this is a strange implem as << and >> are implem for cout ( cin & cout implem)

//static DStreamFD cout(0)
//static DStreamFD cout(1)

}


#endif
