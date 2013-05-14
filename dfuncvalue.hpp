#ifndef DSTRUCT_DFUNC_VALUE_HPP_
#define DSTRUCT_DFUNC_VALUE_HPP_

#include <string>
#include "dvalue.hpp"

class DObject;

namespace Destruct
{

template <typename ReturnPlainType>
class DFunctionValue : public ComputingValue
{
  typedef ReturnPlainType (*FunctionType)(DObject *);

public:
  DFunctionValue(DObject * dobject, FunctionType getFunction) : ComputingValue(dobject), __getFunction(getFunction)
  {
  }

  BaseValue * clone(DObject * dobject) const
  {
    return (new DFunctionValue(dobject, this->__getFunction));
  }

  DValue getFinal() const
  {
    return RealValue<ReturnPlainType>(this->__getFunction(this->__dobject));
  }

  void set(DValue const & v)
  {
    throw "not supported";
  }

private:
  FunctionType __getFunction;

};

}

#endif
