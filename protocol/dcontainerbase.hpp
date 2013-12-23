#ifndef DESTRUCT_DContainerBase_HPP
#define DESTRUCT_DContainerBase_HPP

#include "../dfunction.hpp"
#include "../drealvalue.hpp"

namespace Destruct 
{

class DContainerBase
{
public:
  DContainerBase() {};
  virtual RealValue<DObject*>   iterator() = 0;
  RealValue<DFunctionObject* >  pushObject;
  RealValue<DFunctionObject* >  getObject;
  RealValue<DFunctionObject* >  sizeObject;
  RealValue<DFunctionObject* >  iteratorObject;
};

}

#endif
