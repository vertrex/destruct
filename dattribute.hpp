#ifndef DESTRUCT_DATTRIBUTE_HPP_
#define DESTRUCT_DATTRIBUTE_HPP_

#include "dtype.hpp"


namespace Destruct
{
class DStruct;

class DAttribute
{
public:
                        DAttribute(const DUnicodeString& _name, const DType::Type_t typeId, const DUnicodeString &description = "");
                        DAttribute(const DUnicodeString& _name, const DType::Type_t typeId, const DType::Type_t returnTypeId,  const DType::Type_t argumentTypeId, const DUnicodeString &description = "");
  const DUnicodeString& name(void) const;
  const DUnicodeString& description(void) const;
  DType                 type(void) const;
private:
  DUnicodeString        __name;
  DType                 __type;
  DUnicodeString        __description;
};

}

#endif
