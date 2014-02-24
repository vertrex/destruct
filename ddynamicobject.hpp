#ifndef DESTRUCT_DYNAOBJECT_HPP
#define DESTRUCT_DYNAOBJECT_HPP

#include <vector>
#include "dobject.hpp"

namespace Destruct
{

class DDynamicObject : public DObject
{
public :
  DDynamicObject(DStruct *, DValue const& args);
  DDynamicObject(DDynamicObject const &);
  ~DDynamicObject();

  using DObject::getValue;
  using DObject::setValue;

  virtual DValue           getValue(size_t idx) const;
  virtual void             setValue(size_t idx, DValue const &);
  virtual DValue           call(size_t idx, DValue const&); //this is juste an helper function
  //virtual void           replaceValue(size_t idx, BaseValue const& );
  //void                   replaceValue(DUnicodeString const& name, BaseValue const&);
  DObject*                 privateObject() const;
  void                     setPrivate(DObject* );

protected:
  virtual BaseValue*       getBaseValue(size_t index);
  virtual BaseValue const* getBaseValue(size_t index) const;
  void                     init(DDynamicObject* self);
  void                     copy(DDynamicObject* self, DDynamicObject const& rhs);

  typedef std::vector<BaseValue*  > ValueContainer;
  ValueContainer  __values;
private:
  DDynamicObject& operator=(DDynamicObject const &);
  DObject*        __object;
};

}

#endif
