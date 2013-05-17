#include <string>
#include <vector>

#include "dtype.hpp"
#include "drealvalue.hpp"

namespace Destruct
{

class DString;
class DObject;


const std::string DType::__typeName[] = 
{
 "DInt8",
 "DInt16",
 "DInt32",
 "DInt64",

 "DUInt8",
 "DUInt16",
 "DUInt32",
 "DUInt64",

 "DUnicodeString",

 "DObject",
 "DUnknown"
};

DType::DType(Type_t typeId) : __typeId(typeId)
{
}

FinalValue*     DType::newValue(void) const
{
  return (this->__prototypes[this->__typeId]->clone());
}

DType::Type_t    DType::getType(void) const
{
  return (this->__typeId);
}

const std::string DType::name(void) const
{
  return (__typeName[this->__typeId]);
}

void            DType::init(void)
{
  __prototypes[DInt8Type] = new RealValue<DInt8>(0);
  __prototypes[DInt16Type] = new RealValue<DInt16>(0);
  __prototypes[DInt32Type] = new RealValue<DInt32>(0);
  __prototypes[DInt64Type] = new RealValue<DInt64>(0);

  __prototypes[DUInt8Type] = new RealValue<DUInt8>(0);
  __prototypes[DUInt16Type] = new RealValue<DUInt16>(0);
  __prototypes[DUInt32Type] = new RealValue<DUInt32>(0);
  __prototypes[DUInt64Type] = new RealValue<DUInt64>(0);
 
  __prototypes[DUnicodeStringType] = new RealValue<DUnicodeString>("");
  __prototypes[DObjectType] = new RealValue<DObject* >(0);
}

void            DType::clean(void)
{
  for (int32_t i = 0; i < DUnknownType; ++i)
    delete __prototypes[i];
}

std::vector<FinalValue* >       DType::__prototypes(DType::DUnknownType);
}
