#include "../dstruct.hpp"

namespace Destruct
{

class DMutableStruct : public DStruct
{
public:
  DMutableStruct(DStruct const* base, const DUnicodeString& name, CreateObjectFunction objectFunction);
  DObject* newObject() const;
};

}
