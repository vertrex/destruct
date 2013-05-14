#ifndef DSTRUCT_DVALUE_HPP_
#define DSTRUCT_DVALUE_HPP_

#include <string>
#include "dunicodestring.hpp"
#include "dobject.hpp"

namespace Destruct
{

class DObject;
class DValue;

class BaseValue
{
public:
  virtual ~BaseValue() {};
  virtual BaseValue* clone(DObject *) const = 0;
  
  virtual DValue getFinal() const = 0;
  virtual void set(DValue const & v) = 0;

protected:
  BaseValue() {}
  BaseValue(BaseValue const &) {}
  BaseValue& operator=(BaseValue const &)
  {
    return (*this);
  }  
};

class ComputingValue : public BaseValue
{
public:
  ComputingValue(DObject* dobject);
protected:
  ComputingValue(ComputingValue const&);
  DObject* const __dobject;
};

class FinalValue : public BaseValue
{
public:
  virtual BaseValue * clone(DObject *) const;
  virtual FinalValue * clone() const = 0;
 
  virtual DUnicodeString asUnicodeString() const = 0;
  virtual DValue getFinal() const;

protected:
  FinalValue();
  FinalValue(FinalValue const &);
  FinalValue& operator=(FinalValue const &);
};

template <typename PlainType> 
class TypedValue : public FinalValue
{
public:
  virtual operator PlainType() const = 0;

protected:
  TypedValue() {}
  TypedValue(TypedValue const & rhs) : FinalValue(rhs) {}
  TypedValue& operator=(TypedValue const& rhs)
  {
    FinalValue::operator =(rhs);
   
    return (*this);
  }
};

class DValue
{
public:
  DValue(FinalValue const &);
  DValue(DValue const &);
  explicit DValue (FinalValue* = NULL);
  ~DValue();

  DValue& operator=(const DValue&);

  template <typename PlainType>
  PlainType get() const
  {
    if (this->__value)
    {
      TypedValue<PlainType> const &tv = dynamic_cast<TypedValue<PlainType> const &>(*this->__value);
      return (tv);
    }
    else
      return PlainType();
  }
  DUnicodeString asUnicodeString() const;
private:
  FinalValue*   __value;
};

/*
 *  We specialize DValue because of refCount this shouldn't happen in normal case ...
 */

template <>
inline DObject* DValue::get<DObject* >() const
{
  if (this->__value)
  {
    TypedValue<DObject* > const &tv = dynamic_cast<TypedValue<DObject* > const &>(*this->__value);
    DObject* dobject = tv;
    if (dobject)
      dobject->addRef();
    return (dobject);
  }
  else
   return NULL;
}

}
#endif
