#include "ddynamicobject.hpp"
#include "dsimpleobject.hpp"
#include "drealvalue.hpp"

class DMutableStruct;

namespace Destruct
{

class DMutableObject : public DDynamicObject
{
public:
  DMutableObject(const DUnicodeString& name); 
  DMutableObject(DMutableStruct* dstructDef); 
  DMutableObject(DMutableObject const &);
  //~DMutableObject();

  using DObject::getValue;
  using DObject::setValue;

  virtual DValue           getValue(size_t idx) const;
  virtual void             setValue(size_t idx, DValue const &);
  virtual DValue           call(size_t idx, DValue const&); //this is juste an helper function

  void setValueAttribute(std::string const& name, DValue const& v, DType::Type_t type);

  static DObject* newObject(DMutableStruct* myClass);
  virtual DObject*  clone() const;
};

}
