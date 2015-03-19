#ifndef _DESTRUCT_RECURSIVEDEBUGOBJECT_HPP_
#define _DESTRUCT_RECURSIVEDEBUGOBJECT_HPP_

#include "protocol/traceobject.hpp"

namespace Destruct
{

class RecursiveTraceFunctionObject : public TraceFunctionObject
{
public:
  RecursiveTraceFunctionObject(DFunctionObject* functionObject); 
  ~RecursiveTraceFunctionObject();

  DValue call(void)const;
  DValue call(DValue const& args) const;
private:
  DFunctionObject*      __functionObject; //used ?
};


class RecursiveTraceObject : public TraceObject
{
public:
  RecursiveTraceObject(DStruct* dstruct, DValue const& args);
  RecursiveTraceObject(TraceObject const&);
  RecursiveTraceObject(DObject*);

  virtual DValue    getValue(size_t index) const;
  virtual DValue    call(size_t index, DValue const &);

  virtual DValue    getValue(DUnicodeString const& name) const;
  virtual DValue    call(DUnicodeString const& name, DValue const &);
  virtual DValue    call(DUnicodeString const& name);

  static DObject*   newObject(DStruct *, DValue const& args);
};

}

#endif
