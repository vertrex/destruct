#include "dserialize.hpp"

namespace Destruct
{

class DSerializeBinary : public DSerialize
{
/*
 *  This dserialize in a binary compatible format. That can be used for an archive format 
 */
public:
  DSerializeBinary();
  const DUnicodeString name(void);
  DSerializeBinary*   create(void);

  bool     serialize(DStream& output, DObject* dobject); 
  bool     serialize(DStream& output, DFunctionObject* value, DType::Type_t argumentType, DType::Type_t returnType);
  bool     serialize(DStream& output, DStruct& dstruct);
  bool     serialize(DStream& output, const DUnicodeString& str);
  bool     serialize(DStream& output, DValue value, DType::Type_t type);
 
  DStruct* deserialize(DStream& input);
  bool     deserialize(DStream& input, DObject* dobject);
  bool     deserialize(DStream& input, DUnicodeString& str);
  DValue   deserialize(DStream& input, DType::Type_t type);
  DValue   deserialize(DStream& input, DType::Type_t returnType, DType::Type_t argumentType); //inplky type is in descriptor/databuff
};

}
