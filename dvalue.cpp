#include "dvalue.hpp"
#include "dobject.hpp"

#include "dserialize.hpp" //for dstream !

namespace Destruct
{

BaseValue* FinalValue::clone(DObject *) const
{
  return (clone());
}

DValue FinalValue::getFinal() const
{
  return (*this);
}

FinalValue::FinalValue()
{
}

FinalValue::FinalValue(FinalValue const& rhs) : BaseValue(rhs)
{
}

FinalValue& FinalValue::operator=(FinalValue const &rhs)
{
  BaseValue::operator =(rhs);
  return (*this);
}

DValue::DValue(FinalValue const& fv) : __value(fv.clone())
{
}

DValue::DValue(DValue const& rhs) : __value(rhs.__value ? rhs.__value->clone() : 0)
{
}

DValue::DValue(FinalValue *fv) : __value(fv)
{
}

DValue::~DValue()
{
  delete this->__value;
}

DValue& DValue::replace(const DValue& rhs)
{
  if (this->__value)
     delete this->__value;
  this->__value = rhs.__value->clone();

  return (*this);
}

DValue& DValue::operator=(const DValue& rhs)
{
  if(this->__value)
  {
    if (rhs.__value)
    {
      this->__value->set(rhs);
    }
    else
    {
      delete this->__value;
      this->__value = NULL;
    }
  }
  else
  {
    this->__value = (rhs.__value ? rhs.__value->clone() : NULL);
  }
  return (*this);
}

DUnicodeString DValue::asUnicodeString() const
{
  if (this->__value)
    return (this->__value->asUnicodeString());
  return (DUnicodeString());
}

DStream& operator<<(DStream& os, DValue& value) //asRaw() ? //from raw //python binding ? Value.serialize() Value.dserialize(stream)
{
  if (value.__value)
    return (value.__value->serialize(os));
  os.write(0, sizeof(uint8_t));
//  return os << 0; //XXX ? 
  return os;
}

DStream& operator>>(DStream& is, DValue& value)
{
  if (value.__value)
    return (value.__value->unserialize(is));
  return (is);
}

}
