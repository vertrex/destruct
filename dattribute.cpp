#include "dattribute.hpp"
#include "dstruct.hpp"
#include "destruct.hpp"

namespace Destruct
{

DAttribute::DAttribute(const std::string& _name, const DType::Type_t typeId) : __name(_name), __type(typeId)
{
}

const DUnicodeString&     DAttribute::name(void) const
{
  return (this->__name);
}

DType                     DAttribute::type(void) const
{
  return (this->__type);
}

}
