#ifndef _DESTRUCT_DFUNCTION_HPP_
#define _DESTRUCT_DFUNCTION_HPP_

#include "dnullobject.hpp"
#include "dvalue.hpp"

namespace Destruct
{

class DValue;
/*
 *   This is the base class to inherit from to create a functin/callable object
 */

class DFunctionObject : public RefcountPolicy<DFunctionObject>
{
public:
  virtual DValue call(void)               const = 0;
  virtual DValue call(DValue const& args) const = 0;
  virtual ~DFunctionObject()
  {
  }
};

}
#endif
