#ifndef DSTRUCT_DVALUE_HPP_
#define DSTRUCT_DVALUE_HPP_

#include "destruct.hpp"
#include "dobject.hpp"
#include "dfunction.hpp"
#include "dunicodestring.hpp"
#include "dbuffer.hpp"
//#include "protocol/dstreambase.hpp"
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
  EXPORT virtual BaseValue* clone(DObject *) const = 0;
  EXPORT virtual ~BaseValue() {};
  EXPORT virtual DValue getFinal() const = 0;
  EXPORT virtual void set(DValue const & v) = 0;
protected:
  EXPORT BaseValue() {}
  EXPORT BaseValue(BaseValue const &) {}
  EXPORT BaseValue& operator=(BaseValue const &)
  {
    return (*this);
  }  
};

/*! \brief inherit base value to create a new kind of value that call an internal function
 */
class FinalValue : public BaseValue
{
public:
  EXPORT virtual BaseValue * clone(DObject *) const;
  EXPORT virtual FinalValue * clone() const = 0;
 
  EXPORT virtual DUnicodeString asUnicodeString() const = 0;
  EXPORT virtual DBuffer        asDBuffer() const = 0;
  //EXPORT virtual DStreamBase&  serialize(DStreamBase& os) const = 0;//XXX Better use an object protocol ?
  //EXPORT virtual DStreamBase&  unserialize(DStreamBase& is) = 0;   //XXX Better use an object protocol ?
  EXPORT virtual DValue    getFinal() const;
protected:
  EXPORT FinalValue();
  EXPORT FinalValue(FinalValue const &);
  EXPORT FinalValue& operator=(FinalValue const &);
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
  //template <typename PlainType>
  //EXPORT DValue(RealValue<PlainType> v) : __value(v) {}
  EXPORT DValue(FinalValue const &);
  EXPORT DValue(DValue const &);
  EXPORT explicit DValue (FinalValue* = NULL);
  EXPORT ~DValue();

  EXPORT DValue& operator=(const DValue&);
  EXPORT DValue& replace(const DValue&);

  template <typename PlainType>// ok but could be ambigous in some case
  operator PlainType() const
  {
    return (this->get<PlainType>());
  }

  template <typename PlainType>
  EXPORT PlainType get() const
  {
    if (this->__value)
    {
      TypedValue<PlainType> const &tv = dynamic_cast<TypedValue<PlainType> const &>(*this->__value);
       //try catch bad_cast and rather return DEXception (cast error) so no need to check for dexception and bad cast everywhere ?
      //this can happen for exemple when object as a default constructor that take an argument and the argument is not of the right type
      //also need to type constructor argument 
      return (tv);
    }
    else
      return PlainType();
  }

  //operator std::string();
  //EXPORT friend DStreamBase& operator<<(DStreamBase& os, DValue& value);
  //EXPORT friend DStreamBase& operator>>(DStreamBase& is, DValue& value);
  //std::ostream& serialize (std::ostream& os) const;
  EXPORT DUnicodeString asUnicodeString() const;
  EXPORT DBuffer        asDBuffer() const;
private:
  FinalValue*   __value;
};

/*
 * We specialize DValue because of refCount this shouldn't happen in normal case ...
 */
/*
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
*/
}
#endif
