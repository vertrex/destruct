#ifndef _DESTRUCT_DEBUGOBJECT_HPP_
#define _DESTRUCT_DEBUGOBJECT_HPP_

#include "dobject.hpp"
#include "dsimpleobject.hpp"
#include "dfunction.hpp"

namespace Destruct
{

class BaseValue;

class TraceFunctionObject : public DFunctionObject
{
public:
  TraceFunctionObject(DFunctionObject* functionObject); 

  virtual DValue call(void) const;
  virtual DValue call(DValue const& args) const;
protected:
  ~TraceFunctionObject(); 
private:
  DFunctionObject*      __functionObject;
};

class TraceObject : public DObject 
{
public:
  TraceObject(DStruct* dstruct, DValue const& args);
  TraceObject(TraceObject const&);
  TraceObject(DObject*);

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
  ~TraceObject();

  BaseValue*        getBaseValue(size_t index);
  BaseValue const*  getBaseValue(size_t index) const;
  DObject*          object(void) const;
private:
  DObject*          __dobject;
};

}

#endif
