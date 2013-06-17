#ifndef DESTRUCT_DATTRIBUTE_HPP_
#define DESTRUCT_DATTRIBUTE_HPP_

#include "dtype.hpp"


namespace Destruct
{
class DStruct;

class DAttribute
{
public:
                        DAttribute(const DUnicodeString& _name, const DType::Type_t typeId);
                        //DAttribute(const std::string& _name, const std::string& typeName);
  const DUnicodeString& name(void) const;
  DType                 type(void) const;
  //const std::string     typeName(void) const;
private:
  DUnicodeString        __name;
  DType                 __type;
};

}

#endif
