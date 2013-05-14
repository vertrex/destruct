#ifndef DESTRUCT_DYNAOBJECT_HPP
#define DESTRUCT_DYNAOBJECT_HPP

#include <vector>

#include "dobject.hpp"
#include "dvalue.hpp"

namespace Destruct
{

class DDynamicObject : public DObject
{
public :
  DDynamicObject(DStruct const *);
  DDynamicObject(DDynamicObject const &);
  ~DDynamicObject();

  using DObject::getValue;
  using DObject::setValue;

  virtual DValue        getValue(size_t idx) const;
  virtual void          setValue(size_t idx, DValue const &);
  virtual void          replaceValue(size_t idx, BaseValue const& );
  void                  replaceValue(DUnicodeString const& name, BaseValue const&);

  DObject*              privateObject() const;
  void                  setPrivate(DObject* );

protected:
  virtual BaseValue*       getBaseValue(size_t index);
  virtual BaseValue const* getBaseValue(size_t index) const;
  void                     init(DDynamicObject* self);
  void                     copy(DDynamicObject* self, DDynamicObject const& rhs);

private:
  typedef std::vector<BaseValue*  > ValueContainer;
  DDynamicObject& operator=(DDynamicObject const &);
  ValueContainer  __values;
  DObject*        __object;
};

}

#endif
