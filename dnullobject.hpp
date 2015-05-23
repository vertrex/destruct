#ifndef _DESTRUCT_DNULLOBJECT_HPP_
#define _DESTRUCT_DNULLOBJECT_HPP_

#include "destruct.hpp"
#include "dobject.hpp"

namespace Destruct
{

class DNullObject : public DObject
{
public:
  EXPORT static DNullObject*   instance(); //Use singleton::newObject() ?  DObject<Singleton > 

  EXPORT DObject*         clone() const;
  EXPORT DValue           getValue(size_t index) const;
  EXPORT void             setValue(size_t index, DValue const&);
  EXPORT DValue           call(size_t index, DValue const&);
  EXPORT DValue           getValue(DUnicodeString const& name) const;
  EXPORT void             setValue(DUnicodeString const& name, DValue const& value);
  EXPORT DValue           call(DUnicodeString const& name, DValue const& value);

protected:
  BaseValue*              getBaseValue(size_t index);
  BaseValue const*        getBaseValue(size_t index) const;
  void                    destroy(); //singleton must avoid to call ~DNullObject who use a recuring template pattern

private:
  static DStruct*  __dstructInstance();
  DNullObject();
  DNullObject(DValue const& args);
  ~DNullObject();
}; 

static DNullObject* const DNone = DNullObject::instance();

}

#endif
