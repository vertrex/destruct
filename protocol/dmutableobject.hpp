#include "../ddynamicobject.hpp"
#include "../dsimpleobject.hpp"

//#include "../dstruct.hpp"
class DMutableStruct;

namespace Destruct
{

class DMutableObject : public DDynamicObject //dobject DSimple ou D??object 
{
public:
  DMutableObject(DMutableStruct* dstructDef); 
  DMutableObject(DMutableObject const &);
//XXX constructor par copy  virtual DValue           getValue(size_t idx) const;

  using DObject::getValue;
  using DObject::setValue;


  virtual DValue           getValue(size_t idx) const;
  virtual void             setValue(size_t idx, DValue const &);
  virtual DValue           call(size_t idx, DValue const&); //this is juste an helper function

  static DObject* newObject(DMutableStruct* myClass);
  virtual DObject*  clone() const;
};

}
