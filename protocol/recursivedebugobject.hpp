#ifndef _DESTRUCT_RECURSIVEDEBUGOBJECT_HPP_
#define _DESTRUCT_RECURSIVEDEBUGOBJECT_HPP_

#include "protocol/debugobject.hpp"

namespace Destruct
{

class RecursiveDebugFunctionObject : public DebugFunctionObject
{
public:
  RecursiveDebugFunctionObject(DFunctionObject* functionObject); 
  ~RecursiveDebugFunctionObject();

  DValue call(void)const;
  DValue call(DValue const& args) const;
private:
  DFunctionObject*      __functionObject;
};


class RecursiveDebugObject : public DebugObject
{
public:
  RecursiveDebugObject(DStruct* dstruct, DValue const& args);
  RecursiveDebugObject(DebugObject const&);
  RecursiveDebugObject(DObject*);

  virtual DValue    getValue(size_t index) const;
  virtual DValue    call(size_t index, DValue const &);

  virtual DValue    getValue(std::string const& name) const;
  virtual DValue    call(std::string const& name, DValue const &);
  virtual DValue    call(std::string const& name);

  static DObject*   newObject(DStruct *, DValue const& args);
};

}

#endif
