#ifndef DESTRUCT_DREALVALUE_
#define DESTRUCT_DREALVALUE_

#include <string>
#include <sstream>
#include "dvalue.hpp"
#include "dobject.hpp"

#include "dunicodestring.hpp"


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
    os << this->__val;
    return (os.str());
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

template <>
inline DUnicodeString RealValue<DObject* >::asUnicodeString() const
{
  return (this->__val->instanceOf()->name() + " *");
}

/*
 * DObject specialization
 */

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

}

#endif
