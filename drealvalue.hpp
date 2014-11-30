#ifndef DESTRUCT_DREALVALUE_
#define DESTRUCT_DREALVALUE_

#include <string>
#include <sstream>
#include <iostream>

#include "dvalue.hpp"
#include "dnullobject.hpp"
#include "dfunction.hpp"
#include "protocol/dstreambase.hpp"

namespace Destruct
{
/*
* implemented typedvalue who inherit final value who inherite basevalue
* all specialize method by type must be implem here ! that's why we use template
*/

template <typename PlainType>
class RealValue : public TypedValue<PlainType>
{
public:
  RealValue(PlainType value) : __val(value)
  {
  }

  RealValue(DValue value) : __val(value.get<PlainType>())
  {
  }

  RealValue()
  {
  }

  RealValue(RealValue const & rhs) : __val(rhs.__val)
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
    //os << std::hex << this->__val; //set as hex in option ? 
    os << this->__val;
    return (os.str());
  }

  DStreamBase& serialize(DStreamBase& os) const
  {
    os.write((char *)&this->__val, sizeof(this->__val));
    return (os);
  }

  DStreamBase& unserialize(DStreamBase& is)
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

  DStreamBase& serialize(DStreamBase& os) const
  {
    DUInt32 size = this->size();
    os.write((char*)&size, sizeof(size));
    os.write(this->c_str(), this->size());

    //write Size then string best place here or deserializer / encoder ? 
    //os.write((char *)this->c_str(), this->size()); // pascal string ? 00
    //os.write("\x00", 1); //\x00\x00 in unicode 16 ? 
    //return (os);
    return (os);
  }

  DStreamBase& unserialize(DStreamBase& is)
  {
    DUInt32 size; 
    is.read((char*)&size, sizeof(size));
    char* data = new char[size + 1];
    is.read(data, size);
    data[size] = 0;
    *this = std::string(data);
    delete[] data;
    return (is);
    /*
     //read size then string ? best place here or in deserializer encoder ?
    //XXX this is a pure implem so we will look for \x00 PascalString could be better for binary but maybe not for raw 
    char c = '\xff';
    std::string buffer;

    is.read(&c, sizeof(char));
    while (c != '\x00')
    {  
      buffer += c;
      is.read(&c, sizeof(char));
    }

    *this = buffer;
    return (is); */
  }

  operator std::string() const
  {
    return (*this);
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
inline RealValue<DObject* >::RealValue() : __val(DNone) 
{
}

template <>
inline RealValue<DObject* >::RealValue(DObject* val) : __val(val)
{
  if (val)
    val->addRef();
}

template <>
inline RealValue<DObject*>::RealValue(DValue value) : __val(value.get<DObject*>())
{
}

template <>
inline RealValue<DObject* >::operator DObject*() const
{
  return (this->__val);
}

template <>
inline RealValue<DObject*>::RealValue(RealValue<DObject*> const & rhs) :  __val(rhs) //XXX __val(rhs.__val) ...
{
  if (this->__val)
    this->__val->addRef();
} 

template <>
inline FinalValue* RealValue<DObject* >::clone() const
{
  return  (new RealValue(*this));
}

template <>
inline void RealValue<DObject* >::set(DValue const& v)
{
  if (this->__val)
    this->__val->destroy();
  this->__val = v.get<DObject *>();
}

template <>
inline RealValue<DObject* >::~RealValue<DObject* >()
{
  if (this->__val)
    this->__val->destroy();
}

template <>
inline DUnicodeString RealValue<DObject* >::asUnicodeString() const
{
  if (this->__val->instanceOf())
    return (this->__val->instanceOf()->name() + " *");
  else 
    return std::string("DObject::asUnicodeString() can't get Instance of object !\n");
}

/*
 * DFunctionObject specialization
 */

template <>
inline RealValue<DFunctionObject* >::RealValue() : __val(0) //XXX init a 0 ? call(0) ? throw ? 
{
}

template <>
inline RealValue<DFunctionObject* >::RealValue(DFunctionObject* val) : __val(val)
{
  if (val)
    this->__val->addRef();
}

template <>
inline RealValue<DFunctionObject* >::operator DFunctionObject*() const
{
  return (this->__val);
}

template <>
inline RealValue<DFunctionObject*>::RealValue(RealValue<DFunctionObject*> const & rhs) :  __val(rhs)
{
  if (this->__val)
    this->__val->addRef();
} 

template <>
inline FinalValue* RealValue<DFunctionObject* >::clone() const
{
  return (new RealValue(*this));
}

template <>
inline void RealValue<DFunctionObject* >::set(DValue const& v)
{
  if (this->__val)
    this->__val->destroy();
  this->__val = v.get<DFunctionObject *>();
}

template <>
inline RealValue<DFunctionObject* >::~RealValue<DFunctionObject* >()
{
  if (this->__val)
  {
    this->__val->destroy();
  }
}

template <>
inline DUnicodeString RealValue<DFunctionObject* >::asUnicodeString() const
{
  return ("DFunctionObject"); //throw ?
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
