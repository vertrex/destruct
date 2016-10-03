#ifndef _DESTRUCT_RECURSIVETHREADSAFEOBJECT_HPP_
#define _DESTRUCT_RECURSIVETHREADSAFEOBJECT_HPP_

#include "threading.hpp"
#include "dobject.hpp"

namespace Destruct
{

class BaseValue;

class RecursiveThreadSafeObject : public DObject 
{
public:
  RecursiveThreadSafeObject(DStruct* dstruct, DValue const& args);
  RecursiveThreadSafeObject(RecursiveThreadSafeObject const&);
  RecursiveThreadSafeObject(DObject* dobject, mutex mtx); 

  EXPORT static DObject*   newObject(DStruct *, DValue const& args);

  DObject*          clone() const;
  virtual DValue    getValue(size_t index) const;
  void              setValue(size_t idx, DValue const &);
  virtual DValue    call(size_t index, DValue const &);

  virtual DValue    getValue(DUnicodeString const& name) const;
  void              setValue(DUnicodeString  const& name, DValue const &);

  virtual DValue    call(DUnicodeString const& name, DValue const &);
  virtual DValue    call(DUnicodeString const& name);
protected:
  ~RecursiveThreadSafeObject();

  BaseValue*        getBaseValue(size_t index);
  BaseValue const*  getBaseValue(size_t index) const;
  DObject*          object(void) const;

private:
  DObject*          __dobject; //RealValue (refcount)
  mutex             __mutex;
};

}

#endif
