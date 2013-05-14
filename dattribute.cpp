#include "dattribute.hpp"

namespace Destruct
{

DAttribute::DAttribute(const DUnicodeString& _name, const DType::Type_t typeId) : __name(_name), __type(typeId)
{
}

const DUnicodeString&     DAttribute::name(void) const
{
  return (this->__name);
}

DType                  DAttribute::type(void) const
{
  return (this->__type);
}

}
