#include "dattribute.hpp"
#include "dstruct.hpp"
#include "destruct.hpp"

namespace Destruct
{

DAttribute::DAttribute(const std::string& _name, const DType::Type_t typeId, const DUnicodeString& description) : __name(_name), __type(typeId), __description(description)
{
}

DAttribute::DAttribute(const std::string& _name, const DType::Type_t typeId, const DType::Type_t returnTypeId,  const DType::Type_t argumentTypeId, const DUnicodeString& description) : __name(_name), __type(typeId, returnTypeId, argumentTypeId), __description(description)
{
}

//DAttribute::DAttribute(const std::string& _name, const DType::Type_t returnTypeId, const DType::Type_t argumentTypeId, DMethodObject* object, const DUnicodeString& description) : __name(_name), __type(DType::DMethodType, returnTypeId, argumentTypeId), __description(description)
//{
        //__type(DMethodObjectType, returnTypeId, argumentTypeId);
        //}

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
