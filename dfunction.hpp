#ifndef _DESTRUCT_DFUNCTION_HPP_
#define _DESTRUCT_DFUNCTION_HPP_

namespace Destruct
{

/*
 *   This is the base class to inherit from to create a functin/callable object
 */
class DFunctionObject : public RefcountPolicy<DFunctionObject>
{
public:
  virtual DValue call(void)               const = 0;
  virtual DValue call(const DValue& args) const = 0;
  virtual ~DFunctionObject()
  {
  }
};

}
#endif
