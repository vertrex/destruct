#include "dunicodestring.hpp"

namespace Destruct
{

DUnicodeString::DUnicodeString() : __string("") 
{
}

DUnicodeString::DUnicodeString(const DUnicodeString& dunicodeString) : __string(dunicodeString.__string)
{
}

DUnicodeString::DUnicodeString(const std::string& string) : __string(string)
{
}

DUnicodeString::DUnicodeString(const char *chr) : __string(chr)
{
}

DUnicodeString::DUnicodeString(const char c) : __string("")
{
  this->__string += c;
}

DUnicodeString::~DUnicodeString() 
{
}

std::string DUnicodeString::string(void) const
{
  return (this->__string);
}

const char* DUnicodeString::c_str() const
{
  return (this->__string.c_str());
}

size_t DUnicodeString::size(void) const
{
  return (this->__string.size());
}

size_t DUnicodeString::find(const std::string& str) const
{
  return (this->__string.find(str)); 
}

DUnicodeString DUnicodeString::substr(size_t pos, size_t len) const
{
  return (this->__string.substr(pos, len));
}

DUnicodeString& DUnicodeString::operator=(const DUnicodeString& rhs)
{
  this->__string = rhs.string();
  return (*this);
} 

DUnicodeString& DUnicodeString::operator=(std::string rhs) 
{
  this->__string = rhs;
  return (*this);
}

DUnicodeString DUnicodeString::operator+(const DUnicodeString& rhs) 
{
  std::string tmp = this->__string +  rhs.string();
  return (DUnicodeString(tmp));
}

DUnicodeString& DUnicodeString::operator+=(const DUnicodeString& rhs)
{
  this->__string += rhs.string();
  return (*this);
}

}
