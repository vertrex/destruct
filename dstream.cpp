#include "dstream.hpp"

namespace Destruct
{


DStream::DStream()
{

}

DStream::DStream(std::string filePath, mode _mode)
{
  if (_mode == mode::Input)
    this->__fstream.open(filePath, std::iostream::in | std::iostream::binary);
  else
    this->__fstream.open(filePath, std::iostream::out | std::iostream::binary | std::iostream::trunc);
}

DStream::~DStream()
{
  this->__fstream.close(); 
}

DStream& DStream::operator>>(std::string& val) 
{
  this->__fstream >> val;
  return (*this);
}

DStream& DStream::operator<<(std::string val) 
{
  this->__fstream << val;
  return (*this);
}
  
DStream& DStream::operator<<(char val)
{
  this->__fstream << val;
  return (*this);
}

DStream& DStream::operator<<(StandardEndLine func)
{
  func(this->__fstream);
  return (*this);
}

DStream& DStream::read(char*  buff, uint32_t size)
{
  this->__fstream.read((char*)buff, size);
  return (*this);
}

DStream& DStream::write(const char* buff, uint32_t size) 
{
  this->__fstream.write((const char*)buff, size);
  return (*this);
} 

bool DStream::fail(void)
{
  return (this->__fstream.fail()); 
}

/*
 * DStreamCout
*/

DStreamCout::DStreamCout()
{
}

DStreamCout::~DStreamCout()
{
}

DStream& DStreamCout::operator>>(std::string& val) 
{
  std::cin >> val;
  return (*this);
}

DStream& DStreamCout::operator<<(std::string val) 
{
  std::cout << val;
  return (*this);
}
  

DStream& DStreamCout::operator<<(char val)
{
  std::cout << val;
  return (*this);
}

typedef std::basic_ostream<char, std::char_traits<char> > CoutType;
typedef CoutType& (*StandardEndLine)(CoutType&);
  
DStream& DStreamCout::operator<<(StandardEndLine func)
{
  func(std::cout);
  return (*this);
}

DStream& DStreamCout::read(char*  buff, uint32_t size)
{
  //XXX std::cin >>
  return (*this);
}

DStream& DStreamCout::write(const char* buff, uint32_t size) 
{
  std::cout << std::string(buff, size);
  return (*this);
} 

bool DStreamCout::fail(void)
{
  return (std::cout.fail());
}

}
