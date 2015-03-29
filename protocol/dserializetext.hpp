#include "dserialize.hpp"

namespace Destruct
{

class DSerializeText : public DSerialize
{
public:
  DSerializeText();
  const DUnicodeString name(void);
  DSerializeText*      create(void);

  bool     serialize(DStream& output, DObject* dobject); 
  bool     serialize(DStream& output, DFunctionObject* value, DType::Type_t argumentType, DType::Type_t returnType);
  bool     serialize(DStream& output, DObject* dobject, int depth);
  bool     serialize(DStream& output, DValue value, DType::Type_t type);
  bool     serialize(DStream& output, DStruct& dstruct); 

  bool     deserialize(DStream& input, DObject* dobject);
  DValue   deserialize(DStream& input, DType::Type_t type);
  DStruct* deserialize(DStream& output); //can overload return type ...
  DValue   deserialize(DStream& input, DType::Type_t returnType, DType::Type_t argumentType);
};

}
