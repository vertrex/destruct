#ifndef DESTRUCT_DTYPE_HPP_
#define DESTRUCT_DTYPE_HPP_

#include <stdint.h>
#include <vector>

#include "dunicodestring.hpp"

namespace Destruct
{

class FinalValue;

class DObject;
class DString;

//define is not affected by namespace

#define DInt8   int8_t   
#define DInt16  int16_t
#define DInt32  int32_t  
#define DInt64  int64_t  

#define DUInt8  uint8_t  
#define DUInt16 uint16_t 
#define DUInt32 uint32_t
#define DUInt64 uint64_t 

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

    DUnknownType,
  };

  explicit DType(Type_t typeId); //explicit to be sure to have a defined type when constructing DType 
  explicit DType(Type_t typeId, Type_t ReturnTypeId, Type_t ArgumentTypeId);

  FinalValue*                           newValue(void) const;
  FinalValue*                           newReturnValue(void) const; //hum hum
  FinalValue*                           newArgumentValue(void) const; //hum hum only usefull for func object ...
  Type_t                                getType(void) const;
  Type_t                                getReturnType(void) const;
  Type_t                                getArgumentType(void) const;
  const DUnicodeString                  name(void) const;
  const DUnicodeString                  returnName(void) const;
  const DUnicodeString                  argumentName(void) const;
  
  static void                           init(void);
  static void                           clean(void);
private:
  Type_t                                __typeId;
  Type_t                                __returnTypeId; //xor ? flag pour prendre moins de place ?
  Type_t                                __argumentTypeId;
  static const std::string              __typeName[];
  static std::vector<FinalValue* >      __prototypes;
};

}

#endif
