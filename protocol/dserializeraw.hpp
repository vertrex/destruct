#include "dserialize.hpp"

namespace Destruct
{

class DSerializeRaw : public DSerialize
{
/*
 *  This desrialize and deserialize as binary 'raw' 'flat' type without markers. 
 *  That can be used to read file produce by other tools.
 */
public:
  DSerializeRaw();
  const DUnicodeString name(void);
  DSerializeRaw*   create(void);

  bool     serialize(DStream& output, DObject* dobject); 
  bool     serialize(DStream& output, DFunctionObject* value, DType::Type_t argumentType, DType::Type_t returnType);
  bool     serialize(DStream& output, DStruct& dstruct); 
  bool     serialize(DStream& output, DValue value, DType::Type_t type);

  bool     deserialize(DStream& input, DObject* dobject);
  DStruct* deserialize(DStream& output); 
  DValue   deserialize(DStream& input, DType::Type_t type);
  DValue   deserialize(DStream& input, DType::Type_t returnType, DType::Type_t argumentType);
};

}
