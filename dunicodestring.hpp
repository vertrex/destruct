#ifndef DESTRUCT_DUNICODESTRING_HPP_
#define DESTRUCT_DUNICODESTRING_HPP_

#include <string>
#include <iostream>
#include <sstream>

#include "destruct.hpp"

namespace Destruct
{

class DUnicodeString 
{
public:

EXPORT DUnicodeString();
EXPORT DUnicodeString(const DUnicodeString& dunicodeString);
EXPORT DUnicodeString(const std::string& string);
EXPORT DUnicodeString(const char *); 
EXPORT DUnicodeString(const char);

EXPORT ~DUnicodeString(); 

EXPORT std::string  string(void) const;
EXPORT const char* c_str() const;
EXPORT size_t size(void) const; 
EXPORT size_t find(const std::string& str) const;
EXPORT DUnicodeString substr(size_t pos, size_t len = std::string::npos) const;

EXPORT DUnicodeString& operator=(const DUnicodeString& rhs); 
EXPORT DUnicodeString& operator=(std::string rhs);
EXPORT DUnicodeString& operator+=(const DUnicodeString& rhs);
EXPORT DUnicodeString operator+(const DUnicodeString& rhs);

EXPORT operator std::string() const
{
  return (this->string());
}

EXPORT friend DUnicodeString operator+(const char* left, const DUnicodeString& rhs)
{
  std::string tmp = left + rhs.string();
  return (DUnicodeString(tmp));
}

EXPORT friend std::ostream& operator<<(std::ostream& out, const DUnicodeString &dunicodeString)
{
  out << dunicodeString.string(); 
  return (out);
}

EXPORT friend std::istream& operator>>(std::istream& in, DUnicodeString &dunicodeString)
{
  std::string tmp;
  in >> tmp;

  dunicodeString += tmp; //+= or = ?
  return (in);
}

EXPORT friend bool operator<(const DUnicodeString &left, const DUnicodeString &right)
{
  return (left.string() < right.string()); 
}

EXPORT friend bool operator>(const DUnicodeString &left, const DUnicodeString &right)
{
  return (left.string() > right.string());
}

EXPORT friend bool operator==(const DUnicodeString &left, const DUnicodeString &right)
{
  return (left.string() == right.string());
}

EXPORT friend bool operator!=(const DUnicodeString &left, const DUnicodeString &right)
{
  return (left.string() != right.string());
}
private:
  std::string __string;
};

}

#endif
