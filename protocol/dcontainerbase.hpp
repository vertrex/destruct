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
  RealValue<DFunctionObject* >  iteratorObject;
  RealValue<DFunctionObject* >  pushObject;// ici ou ds dconatienr plutot ?
  RealValue<DFunctionObject* >  getObject;//
  RealValue<DFunctionObject* >  sizeObject;//
  RealValue<DFunctionObject* >  setItemObject; //
};

}

#endif
