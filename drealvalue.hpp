#ifndef DESTRUCT_DREALVALUE_
#define DESTRUCT_DREALVALUE_

#include <string>
#include <sstream>
#include <iostream>
#include "dvalue.hpp"
#include "dobject.hpp"
#include "dunicodestring.hpp"
#include "dstream.hpp"

namespace Destruct
{

template <typename PlainType>
class RealValue : public TypedValue<PlainType>
{
public:
  RealValue(PlainType value) : __val(value)
  {
  }

  RealValue()
  {
  }

  ~RealValue()
  {
  }

  FinalValue* clone() const
  {
    return (new RealValue(*this));
  }

  DUnicodeString asUnicodeString() const
  {
    std::ostringstream os;
    os << std::hex << this->__val; //set as hex as options ?
    return (os.str());
  }

  DStream& serialize(DStream& os) const
  {
    os.write((char *)&this->__val, sizeof(this->__val));
    return (os);
  }

  DStream& unserialize(DStream& is)
  {
    is.read((char *)&this->__val, sizeof(this->__val));
    return (is);
  }

  operator PlainType() const
  {
    return (this->__val);
  }

  void set(DValue const& v)
  {
    this->__val = v.get<PlainType>();
  }

protected:
  PlainType __val;
};

/*
 * DUnicodeString class specialization
 */
  

template <> 
class RealValue<DUnicodeString> : public TypedValue<DUnicodeString>, public DUnicodeString
{
public:
  RealValue(DUnicodeString const& s) : std::basic_string<char>(s)
  {
  }

  RealValue(char const* s) : std::basic_string<char>(s)
  {
  }

  RealValue()
  {
  }

  FinalValue* clone() const
  {
    return (new RealValue(*this));
  }

  DUnicodeString asUnicodeString() const
  {
    DUnicodeString const& ref = '"' +  *this + '"';
    return (ref);
  }

  DStream& serialize(DStream& os) const
  {
    os.write((char *)this->c_str(), this->size()); // pascal string ? 00
    os.write("\x00", 1); //\x00\x00 in unicode 16 ? 
    return (os);
  }

  DStream& unserialize(DStream& is)
  {
    //XXX this is a pure implem so we will look for \x00 PascalString could be better for binary but maybe not for raw 
    char c = '\xff';
    std::string buffer;

    while (c != '\x00')
    {
      is.read(&c, sizeof(char)); 
      buffer += c;
    }
    *this = buffer;
    return (is);
  }

  operator std::string() const
  {
    DUnicodeString const &ref = *this;
    return (ref);
  }

  void set(DValue const& v)
  {
    *this = v.get<std::string>();
  }
};

/*
 * DObject specialization
 */
template <>
inline DUnicodeString RealValue<DObject* >::asUnicodeString() const
{
  return (this->__val->instanceOf()->name() + " *");
}

template <>
inline RealValue<DObject* >::RealValue(DObject* val) : __val(val)
{
  if (val)
    this->__val->addRef();
}


template <>
inline RealValue<DObject* >::RealValue() : __val(0) 
{
}

template <>
inline RealValue<DObject* >::~RealValue<DObject* >()
{
  if (this->__val)
    this->__val->destroy();
}

template <>
inline RealValue<DObject* >::operator DObject*() const
{
  if (this->__val)
    this->__val->addRef();
  return (this->__val);
}

template <>
inline void RealValue<DObject* >::set(DValue const& v)
{
  if (this->__val)
  {
    this->__val->destroy();
  }
  this->__val = v.get<DObject *>();
}

/*
 *  DUInt8 specialization (or asUnicodeString see him as char, 'c')
 */

template <>
inline DUnicodeString RealValue<DUInt8 >::asUnicodeString() const
{
  std::ostringstream os;
  os << static_cast<uint32_t>(this->__val);
  return (os.str());
}

template <>
inline DUnicodeString RealValue<DInt8 >::asUnicodeString() const
{
  std::ostringstream os;
  os << static_cast<int32_t>(this->__val); //test  with neg type, if we get it as hex it will be aligned to 32 bit (0xfffffF) 
  return (os.str());
}

}

#endif
