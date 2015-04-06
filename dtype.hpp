#ifndef DESTRUCT_DTYPE_HPP_
#define DESTRUCT_DTYPE_HPP_

#include <stdint.h>
#include <vector>

#include "destruct.hpp"
#include "dunicodestring.hpp"

typedef int8_t  DInt8;
typedef int16_t DInt16;
typedef int32_t DInt32;
typedef int64_t DInt64;

typedef uint8_t  DUInt8;
typedef uint16_t DUInt16;
typedef uint32_t DUInt32;
typedef uint64_t DUInt64;

namespace Destruct
{

class FinalValue;

class DObject;
class DString;

/*
 *  Enum is used as a dispatch table in python bindings so order must not be changed
 */

class DType
{
public:
  enum Type_t //renomer type
  {
    DInt8Type,  //virer le Type a la fi ncar de doute on fait DType:DInt8
    DInt16Type,
    DInt32Type,
    DInt64Type,

    DUInt8Type,
    DUInt16Type,
    DUInt32Type,
    DUInt64Type,

    DUnicodeStringType,

    DObjectType,
    DMethodType,
    DNoneType,

    DStructType,
    DBufferType,

    DUnknownType,
  };

  EXPORT explicit DType(Type_t typeId); //explicit to be sure to have a defined type when constructing DType 
  EXPORT explicit DType(Type_t typeId, Type_t ReturnTypeId, Type_t ArgumentTypeId);

  EXPORT FinalValue*                    newValue(void) const;
  EXPORT FinalValue*                    newReturnValue(void) const; //hum hum
  EXPORT FinalValue*                    newArgumentValue(void) const; //hum hum only usefull for func object ...
  EXPORT Type_t                         getType(void) const;
  EXPORT Type_t                         getReturnType(void) const;
  EXPORT Type_t                         getArgumentType(void) const;
  EXPORT const DUnicodeString           name(void) const;
  EXPORT const DUnicodeString           returnName(void) const;
  EXPORT const DUnicodeString           argumentName(void) const;
  
  EXPORT static void                    init(void);
  EXPORT static void                    clean(void);
private:
  Type_t                                __typeId;
  Type_t                                __returnTypeId; //xor ? flag pour prendre moins de place ?
  Type_t                                __argumentTypeId;
  static const std::string              __typeName[];
  static std::vector<FinalValue* >      __prototypes;
};

}

#endif
