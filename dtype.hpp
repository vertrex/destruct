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

class DType
{
public:
//XXX this is used as dispatch table in python so it better to not change order :)
  enum Type_t
  {
    DInt8Type,
    DInt16Type,
    DInt32Type,
    DInt64Type,

    DUInt8Type,
    DUInt16Type,
    DUInt32Type,
    DUInt64Type,

    DUnicodeStringType,

    DObjectType,

    DUnknownType,
  };

  explicit DType(Type_t typeId); //explicit to be sure to have a defined type when constructing DType 

  FinalValue*                           newValue(void) const;
  Type_t                                getType(void) const;
  const std::string                     name(void) const; //type name
  static void                           init(void);
  static void                           clean(void);
  //XXX add this for serialization 
private:
  Type_t                                __typeId;
  static const std::string              __typeName[];
  static std::vector<FinalValue* >      __prototypes;
};

}

#endif
