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

  RealValue&  operator=(RealValue const& rhs)
  {
    __val = rhs.__val;
    return (*this);
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
 * DObject class specialization
 */
template <>
class RealValue<DObject*> : public TypedValue<DObject*> 
{
public:
  RealValue() : __val(DNone) 
  {
  }

  RealValue(DObject* val) : __val(val)
  {
    if (this->__val)
      this->__val->addRef();
  }

  RealValue(DValue value) : __val(value.get<DObject*>())
  {
    if (this->__val)
      this->__val->addRef();
  }

  RealValue(RealValue<DObject*> const & rhs) :  __val(rhs.__val)
  {
    if (this->__val)
      this->__val->addRef();
  }
 
  ~RealValue()
  {
    if (this->__val)
      this->__val->destroy();
  }

  operator DObject*() const
  {
    return (this->__val); //return object as is, don't increment ref
  }

  RealValue<DObject*>& operator=(RealValue<DObject*> const& value)
  {
    if (this->__val != value.__val)
    {
      if (this->__val)
       this->__val->destroy();

      this->__val = value.__val;
      if (this->__val)
        this->__val->addRef();
    }

    return (*this);
  }

  FinalValue* clone() const
  {
    return  (new RealValue(*this)); //RealValue(DObject*) add a ref // != this->__val->clone()
  }

  void set(DValue const& v)
  {
    if (this->__val)
     this->__val->destroy();

    this->__val = v.get<DObject *>();
    if (this->__val)
      this->__val->addRef();
  }

  DUnicodeString asUnicodeString() const
  {
    if (this->__val->instanceOf())
      return (this->__val->instanceOf()->name() + " *");
    else 
      return std::string("DObject::asUnicodeString() can't get Instance of object !\n");
  }

  DBuffer asDBuffer() const
  {
    throw DException("Can't convert DObject to DBuffer");
  }
protected:
  DObject*      __val;
};

/*
 * DFunctionObject specialization
 */
template <>
class RealValue<DFunctionObject*> : public TypedValue<DFunctionObject*> 
{
public:
  RealValue() : __val(NULL)
  {
  }

  RealValue(DFunctionObject* val) : __val(val)
  {
    if (this->__val)
      this->__val->addRef();
  }

  RealValue(DValue value) : __val(value.get<DFunctionObject*>())
  {
    if (this->__val)
     this->__val->addRef();
  }

  RealValue(RealValue<DFunctionObject*> const & rhs) :  __val(rhs.__val)
  {
    if (this->__val)
      this->__val->addRef();
  }
 
  ~RealValue()
  {
    if (this->__val)
      this->__val->destroy();
  }

  operator DFunctionObject*() const
  {
    return (this->__val); //return as is, don't increment ref
  }

  RealValue<DFunctionObject*>& operator=(RealValue<DFunctionObject*> const& value)
  {
    if (this->__val)
     this->__val->destroy();

    this->__val = value.__val;
    if (this->__val)
      this->__val->addRef();

    return (*this);
  }

  FinalValue* clone() const
  {
    return (new RealValue(*this));
  }

  void set(DValue const& v)
  {
    if (this->__val)
     this->__val->destroy();

    this->__val = v.get<DFunctionObject *>();
    if (this->__val)
      this->__val->addRef();
  }

  DUnicodeString asUnicodeString() const
  {
    return ("DFunctionObject"); //throw ?
  }

  DBuffer  asDBuffer() const
  {
    throw DException("Can't convert DFunctionObject* as DBuffer");
  }
protected:
  DFunctionObject*      __val;
};

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
  return ("DBuffer[]"); //XXX size return DUnicodeString(buff, size) ?
}

template<>
inline DBuffer RealValue<DBuffer>::asDBuffer() const
{
  return (*this);
}

}

#endif
