#include "ddynamicobject.hpp"

class DMutableStruct;

namespace Destruct
{

class DMutableObject : public DDynamicObject
{
public:
  DMutableObject(const DUnicodeString& name, DValue const& args); 
  DMutableObject(DMutableStruct* dstructDef, DValue const& args); 
  DMutableObject(DMutableObject const &);
  //~DMutableObject();

  using DObject::getValue;
  using DObject::setValue;

  virtual DValue           getValue(size_t idx) const;
  virtual void             setValue(size_t idx, DValue const &);
  virtual DValue           call(size_t idx, DValue const&); //this is juste an helper function

  void setValueAttribute(DType::Type_t type, std::string const& name, DValue const& v);

  static DObject* newObject(DMutableStruct* myClass, DValue const& args);
  virtual DObject*  clone() const;
};

}
