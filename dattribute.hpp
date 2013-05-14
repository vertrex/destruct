#ifndef DESTRUCT_DATTRIBUTE_HPP_
#define DESTRUCT_DATTRIBUTE_HPP_

#include "dtype.hpp"

namespace Destruct
{

class DAttribute
{
public:
                        DAttribute(const DUnicodeString& _name, const DType::Type_t typeId);
  const DUnicodeString& name(void) const;
  DType                 type(void) const;
private:
  DUnicodeString        __name;
  DType                 __type;
};

}

#endif
