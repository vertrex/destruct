#include "../dstruct.hpp"

namespace Destruct
{

class DMutableStruct : public DStruct
{
public:
                //inherit dstruct car dmutlbe struc c pareille ? gerer le dynamique si parent est modifier ?  fait 2 constructeu different ? 


  typedef DObject* (*CreateMutableObjectFunction)(DMutableStruct *);
  DMutableStruct(DStruct const* base, const DUnicodeString& name, CreateMutableObjectFunction objectFunction);
  DObject* newObject(); // const;for mutable on petu remettre donc ?
  void addAttribute(const DAttribute& attribute);

private:
  const CreateMutableObjectFunction    __createObject;
};

}
