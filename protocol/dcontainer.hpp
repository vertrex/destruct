#ifndef DESTRUCT_DCONTAINER_HPP_
#define DESTRUCT_DCONTAINER_HPP_

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

  virtual DUInt64    size(void) = 0;
  virtual DValue     get(DValue const& args) = 0;
  virtual DObject*   setItem(DValue const& args) = 0;
  virtual DObject*   iterator(void) = 0;
public:
  RealValue<DFunctionObject* >  getObject;
  RealValue<DFunctionObject* >  sizeObject;
  RealValue<DFunctionObject* >  setItemObject; 
  RealValue<DFunctionObject* >  iteratorObject; 
};
}
#endif 
