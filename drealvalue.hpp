#ifndef DESTRUCT_DREALVALUE_
#define DESTRUCT_DREALVALUE_

#include <string>
#include <sstream>
#include <iostream>

#include "dvalue.hpp"
#include "dnullobject.hpp"
#include "dfunction.hpp"
#include "dbuffer.hpp"
#include "dexception.hpp"

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
  RealValue() : __val()
  {
  }

  RealValue(PlainType value) : __val(value)
  {
  }

  RealValue(DValue value) : __val(value.get<PlainType>())
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

  DBuffer       asDBuffer() const
  {
    //copy or not // XXX delte ?
    DBuffer buffer((uint8_t*)&this->__val, sizeof(this->__val));    
    return (buffer);
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
class RealValue<DUnicodeString> : public TypedValue<DUnicodeString>, public DUnicodeString //inherit ? use private copy ?
{
public:
  RealValue(DUnicodeString const& s) : DUnicodeString(s)
  {
  }

  RealValue(char const* s) : DUnicodeString(s)
  {
  }

  RealValue() : DUnicodeString("")
  {
  }

  FinalValue* clone() const
  {
    return (new RealValue(*this));
  }

  DUnicodeString asUnicodeString() const
  {
    DUnicodeString const ref('"' +  this->string() + '"');

    return (ref);
  }

  DBuffer       asDBuffer() const
  {
                                //copy or not ? XXX delete ?
    DBuffer buffer((uint8_t*)this->c_str(), (int32_t)this->size());

    return (buffer);
  }

  operator DUnicodeString() const
  {
    return (*this);
  }

  operator std::string() const
  {
    return (this->string());
  }

  void set(DValue const& v)
  {
    *this = v.get<DUnicodeString>();
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
inline RealValue<DObject* >::~RealValue()
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

template <>
inline DBuffer RealValue<DObject*>::asDBuffer() const
{
  throw DException("Can't convert DObject to DBuffer");
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
inline RealValue<DFunctionObject*>::RealValue(const RealValue<DFunctionObject*>& rhs) :  __val(rhs)
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
inline RealValue<DFunctionObject* >::~RealValue()
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

template<>
inline DBuffer  RealValue<DFunctionObject* >::asDBuffer() const
{
  throw DException("Can't convert DFunctionObject* as DBuffer");
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

/*
 * DSruct specialization
 */
template <>
inline FinalValue* RealValue<DStruct* >::clone() const
{
  return  (new RealValue(*this));
}

template <>
inline DUnicodeString RealValue<DStruct* >::asUnicodeString() const
{
  if (this->__val)
    return (this->__val->name());
  return ("");
}

template<>
inline DBuffer  RealValue<DStruct*>::asDBuffer() const
{
  throw DException("Can't convert DStruct as DBuffer");
}

/*
 * DBuffer specialization
 */
template <>
inline RealValue<DBuffer>::RealValue() : __val(DBuffer(NULL, 0))
{
}

template <>
inline DUnicodeString RealValue<DBuffer>::asUnicodeString() const
{
  return ("DBuffer[]"); //XXX size
}

template<>
inline DBuffer RealValue<DBuffer>::asDBuffer() const
{
  return (*this);
}

}

#endif
