#ifndef _DESTRUCT_DEBUGOBJECT_HPP_
#define _DESTRUCT_DEBUGOBJECT_HPP_

#include "dobject.hpp"
#include "dsimpleobject.hpp"
#include "dfunction.hpp"

namespace Destruct
{

class BaseValue;

class DebugFunctionObject : public DFunctionObject
{
public:
  DebugFunctionObject(DFunctionObject* functionObject); 
  ~DebugFunctionObject();

  virtual DValue call(void) const;
  virtual DValue call(DValue const& args) const;
private:
  DFunctionObject*      __functionObject;
};

class DebugObject : public DObject 
{
public:
  DebugObject(DStruct* dstruct, DValue const& args);
  DebugObject(DebugObject const&);
  DebugObject(DObject*);

  static DObject*   newObject(DStruct *, DValue const& args);

  DObject*          clone() const;
  virtual DValue    getValue(size_t index) const;
  void              setValue(size_t idx, DValue const &);
  virtual DValue    call(size_t index, DValue const &);

  virtual DValue    getValue(DUnicodeString const& name) const;
  void              setValue(DUnicodeString const& name, DValue const &);

  virtual DValue    call(DUnicodeString const& name, DValue const &);
  virtual DValue    call(DUnicodeString const& name);
protected:
  ~DebugObject();

  BaseValue*        getBaseValue(size_t index);
  BaseValue const*  getBaseValue(size_t index) const;
  DObject*          object(void) const;
private:
  DObject*          __dobject;
};

}

#endif
