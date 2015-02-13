#ifndef DESTRUCT_DUNICODESTRING_HPP_
#define DESTRUCT_DUNICODESTRING_HPP_

#include <string>
#include <iostream>
#include <sstream>

namespace Destruct
{

class DUnicodeString 
{
public:

DUnicodeString();
DUnicodeString(const DUnicodeString& dunicodeString);
DUnicodeString(const std::string& string);
DUnicodeString(const char *); 
DUnicodeString(const char);

~DUnicodeString(); 

std::string  string(void) const;
const char* c_str() const;
size_t size(void) const; 
size_t find(const std::string& str) const;
DUnicodeString substr(size_t pos, size_t len = std::string::npos) const;

DUnicodeString& operator=(const DUnicodeString& rhs); 
DUnicodeString& operator=(std::string rhs);
DUnicodeString& operator+=(const DUnicodeString& rhs);
DUnicodeString operator+(const DUnicodeString& rhs);

operator std::string() const
{
  return (this->string());
}

friend DUnicodeString operator+(const char* left, const DUnicodeString& rhs)
{
  std::string tmp = left + rhs.string();
  return (DUnicodeString(tmp));
}

friend std::ostream& operator<<(std::ostream& out, const DUnicodeString &dunicodeString)
{
  out << dunicodeString.string(); 
  return (out);
}

friend std::istream& operator>>(std::istream& in, DUnicodeString &dunicodeString)
{
  std::string tmp;
  in >> tmp;

  dunicodeString += tmp; //+= or = ?
  return (in);
}

friend bool operator<(const DUnicodeString &left, const DUnicodeString &right)
{
  return (left.string() < right.string()); 
}

friend bool operator>(const DUnicodeString &left, const DUnicodeString &right)
{
  return (left.string() > right.string());
}

friend bool operator==(const DUnicodeString &left, const DUnicodeString &right)
{
  return (left.string() == right.string());
}

friend bool operator!=(const DUnicodeString &left, const DUnicodeString &right)
{
  return (left.string() != right.string());
}

private:
  std::string __string;
};

}

#endif
