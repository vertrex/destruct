#ifndef __DESTRUCT_MUTABLEOBJECT_HPP
#define __DESTRUCT_MUTABLEOBJECT_HPP

#include "destruct.hpp"
#include "ddynamicobject.hpp"

class DMutableStruct;

namespace Destruct
{

class DMutableObject : public DDynamicObject
{
public:
  EXPORT DMutableObject(const DUnicodeString& name, DValue const& args); 
  EXPORT DMutableObject(DMutableStruct* dstructDef, DValue const& args); 
  EXPORT DMutableObject(DMutableObject const &);
  EXPORT ~DMutableObject();

  using DObject::getValue;
  using DObject::setValue;

  EXPORT virtual DValue           getValue(size_t idx) const;
  EXPORT virtual void             setValue(size_t idx, DValue const &);
  EXPORT virtual DValue           call(size_t idx, DValue const&); //this is juste an helper function

  EXPORT void setValueAttribute(DType::Type_t type, std::string const& name, DValue const& v);

  EXPORT static DObject* newObject(DMutableStruct* myClass, DValue const& args);
  EXPORT virtual DObject*  clone() const;
};

}

#endif
