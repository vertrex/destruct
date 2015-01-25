#ifndef DSTRUCT_DVALUE_HPP_
#define DSTRUCT_DVALUE_HPP_

#include "dobject.hpp"
#include "dfunction.hpp"
#include "dunicodestring.hpp"
#include "protocol/dstreambase.hpp"

namespace Destruct
{

//class DValue;

//spliter les base value ds un autre fichier 

/*! \brief   This an interface, bae class for value,
 *
 *  all value implementation must inherit this class 
 *  this let manipulate value , value implemetation are in a separe class called DValue
 *  objet can store this basevalue class to get access to derived class by inheritance
 */
class BaseValue
{
public:
  virtual BaseValue* clone(DObject *) const = 0;
  virtual ~BaseValue() {};
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

/*! \brief inherit base value to create a new kind of value that call an internal function
 */
class FinalValue : public BaseValue
{
public:
  virtual BaseValue * clone(DObject *) const;
  virtual FinalValue * clone() const = 0;
 
  virtual DUnicodeString asUnicodeString() const = 0;
  virtual DStreamBase&  serialize(DStreamBase& os) const = 0;//XXX Better use an object protocol ?
  virtual DStreamBase&  unserialize(DStreamBase& is) = 0;   //XXX Better use an object protocol ?
  virtual DValue    getFinal() const;
protected:
  FinalValue();
  FinalValue(FinalValue const &);
  FinalValue& operator=(FinalValue const &);
};

/*! \brief provide copy and plain type conversion operator
 */
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

/*! \brief Value handle
 *
 * this is the value implementation to manipulate basevalue object allocate/delete embeded acess
 * through final value converter
 * it use a template class to specialize the get method 
 */
class DValue
{
public:
  DValue(FinalValue const &);
  DValue(DValue const &);
  explicit DValue (FinalValue* = NULL);
  ~DValue();

  DValue& operator=(const DValue&);

  DValue& replace(const DValue&);

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

  //operator std::string();
  friend DStreamBase& operator<<(DStreamBase& os, DValue& value);
  friend DStreamBase& operator>>(DStreamBase& is, DValue& value);
  //std::ostream& serialize (std::ostream& os) const;
  DUnicodeString asUnicodeString() const;
private:
  FinalValue*   __value;
};

/*
 * We specialize DValue because of refCount this shouldn't happen in normal case ...
 */
template <>
inline DObject* DValue::get<DObject* >() const
{
  if (this->__value)
  {
    TypedValue<DObject* > const &tv = dynamic_cast<TypedValue<DObject* > const &>(*this->__value);
    DObject* dobject = tv;
    if (dobject)
      dobject->addRef(); //add ref for getter so he doesn't have do to it but must call destroy
    return (dobject);
  }
  else
   return (NULL); // ret DNone !
}

template <>
inline DFunctionObject* DValue::get<DFunctionObject* >() const
{
  if (this->__value)
  {
    TypedValue<DFunctionObject* > const &tv = dynamic_cast<TypedValue<DFunctionObject* > const &>(*this->__value);
    DFunctionObject* dobject = tv;
    if (dobject)
      dobject->addRef(); //add ref for getter so he doesn't have do to it but must call destroy
    return (dobject);
  }
  else
   return (NULL); // ret DNone !
}

}
#endif
