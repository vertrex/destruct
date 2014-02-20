#ifndef DESTRUCT_DCONTAINER_HPP_
#define DESTRUCT_DCONTAINER_HPP_

#include "dvalue.hpp"
#include "dcppobject.hpp"

namespace Destruct 
{

class DContainer
{
public:
  DContainer()
  {
  }

  DContainer(const DContainer& copy)
  {
  }

  virtual RealValue<DUInt64>    push(DValue const& args) = 0;
  virtual DValue                get(DValue const& args) = 0;
  virtual RealValue<DUInt64>    size(void) = 0;
  virtual RealValue<DObject*>   setItem(DValue const& args) = 0;

  RealValue<DFunctionObject* >  pushObject;
  RealValue<DFunctionObject* >  getObject;
  RealValue<DFunctionObject* >  sizeObject;
  RealValue<DFunctionObject* >  setItemObject; 
};
}
#endif 
