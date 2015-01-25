#include "dattribute.hpp"
#include "dstruct.hpp"
#include "destruct.hpp"
#include "dtype.hpp"

namespace Destruct
{

DAttribute::DAttribute(const DType::Type_t typeId,  const DUnicodeString& _name, const DUnicodeString& description) : __name(_name), __type(typeId), __description(description)
{
}

DAttribute::DAttribute(const DType::Type_t returnTypeId, const DUnicodeString& _name, const DType::Type_t argumentTypeId, const DUnicodeString& description) : __name(_name), __type(DType::DMethodType, returnTypeId, argumentTypeId), __description(description)
{
}

DAttribute::DAttribute(const DType::Type_t returnTypeId, const DUnicodeString& _name, const DType::Type_t argumentTypeId, const DType::Type_t typeId, const DUnicodeString& description) : __name(_name), __type(typeId, returnTypeId, argumentTypeId), __description(description)
{
}

const DUnicodeString&     DAttribute::name(void) const
{
  return (this->__name);
}

const DUnicodeString&     DAttribute::description(void) const
{
  return (this->__description);
}

DType                     DAttribute::type(void) const
{
  return (this->__type);
}

}
