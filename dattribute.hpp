#ifndef DESTRUCT_DATTRIBUTE_HPP_
#define DESTRUCT_DATTRIBUTE_HPP_

#include "destruct.hpp"
#include "dtype.hpp"
#include "dunicodestring.hpp"

namespace Destruct
{
class DStruct;

class DAttribute
{
public:
  EXPORT DAttribute(const DType::Type_t typeId,  const DUnicodeString& _name, const DUnicodeString& description = ""); 
  EXPORT DAttribute(const DType::Type_t returnTypeId, const DUnicodeString& _name, const DType::Type_t argumentTypeId, const DUnicodeString& description = "");
  EXPORT DAttribute(const DType::Type_t returnTypeId, const DUnicodeString& _name, const DType::Type_t argumentTypeId, const DType::Type_t typeId, const DUnicodeString& description = "");

  EXPORT const DUnicodeString& name(void) const;
  EXPORT const DUnicodeString& description(void) const;
  EXPORT DType                 type(void) const;
private:
  DUnicodeString        __name;
  DType                 __type;
  DUnicodeString        __description;
};

}

#endif
