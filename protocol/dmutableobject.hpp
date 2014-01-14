#include "../ddynamicobject.hpp"
#include "../dsimpleobject.hpp"

//#include "../dstruct.hpp"
class DMutableStruct;

namespace Destruct
{

class DMutableObject : public DDynamicObject //dobject DSimple ou D??object 
{
public:
  DMutableObject(DStruct* dstructDef); 
  DMutableObject(DMutableObject const &);
//XXX constructor par copy 

  static DObject* newObject(DStruct* myClass);
  virtual DObject*  clone() const;
};

}
