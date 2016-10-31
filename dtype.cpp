#include <string>
#include <vector>

#include "dtype.hpp"
#include "drealvalue.hpp"
#include "dnullobject.hpp"
#include "dbuffer.hpp"

namespace Destruct
{

class DString;
class DObject;
class DFunctionObject;
class DNone;

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
 "DMethod",
 "DNone",

 "DStruct",
 "DBuffer",

 "DOpaque",

 "DUnknown"
};

DType::DType(Type_t typeId) : __typeId(typeId), __returnTypeId(DUnknownType), __argumentTypeId(DUnknownType)
{
}

DType::DType(Type_t typeId, Type_t returnTypeId, Type_t argumentTypeId) : __typeId(typeId), __returnTypeId(returnTypeId), __argumentTypeId(argumentTypeId)
{
}

FinalValue*     DType::newValue(void) const
{
//(  if this->__typeId == DUnknownType)
  //throw DException() ; ...
  return (this->__prototypes[this->__typeId]->clone()); ///XXX no meaning anymore as there is 3 type on a type with getType returnType etc ... a bit stupid or bad designed
}

FinalValue*     DType::newReturnValue(void) const
{
  return (this->__prototypes[this->__returnTypeId]->clone()); //? return and aergument in type ? or in attribute or other inherited 
}

FinalValue*     DType::newArgumentValue(void) const
{
  return (this->__prototypes[this->__argumentTypeId]->clone());
}

DType::Type_t    DType::getType(void) const
{
  return (this->__typeId);
}

DType::Type_t    DType::getReturnType(void) const
{
  return (this->__returnTypeId);
}

DType::Type_t   DType::getArgumentType(void) const
{
  return (this->__argumentTypeId);
}

const DUnicodeString DType::name(void) const
{
  return (this->__typeName[this->__typeId]);
}

const DUnicodeString DType::returnName(void) const
{
  return (this->__typeName[this->__returnTypeId]);
}

const DUnicodeString DType::argumentName(void) const
{
  return (this->__typeName[this->__argumentTypeId]);
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
  __prototypes[DObjectType] = new RealValue<DObject* >(DNone); //DNone ? XXX
  __prototypes[DMethodType] = new RealValue<DFunctionObject* >(NULL); //DMethodObject ?  None funcobject?
  __prototypes[DNoneType] = new RealValue<DObject* >(DNone);

  __prototypes[DStructType] = new RealValue<DStruct* >(NULL);
  __prototypes[DBufferType] = new RealValue<DBuffer>();
  
  __prototypes[DOpaqueType] = new RealValue<DOpaque>(NULL);
//dunknown type ? 
}

void            DType::clean(void)
{
  for (int32_t i = 0; i < DUnknownType; ++i)
    delete __prototypes[i];
}

std::vector<FinalValue* >       DType::__prototypes(DType::DUnknownType);
}
