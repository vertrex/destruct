#ifndef _DESTRUCT_DOBJECT_HPP_
#define _DESTRUCT_DOBJECT_HPP_

#include <memory>
#include "dpolicy.hpp"
#include "dstruct.hpp"

#include <iostream>
#include <string>

namespace Destruct
{
class DValue;
class BaseValue;

class DObject : public RefcountPolicy<DObject>
{
public:
  friend class RefcountPolicy<DObject>;
 
  DObject(DStruct * dstructDef, DValue const& args); //static dstructDef ? 

  DStruct * instanceOf() const;
  virtual DObject* clone() const = 0;
  virtual DValue getValue(size_t index) const = 0;
  virtual void setValue(size_t idx, DValue const &) = 0;
  virtual DValue call(size_t index, DValue const &)  = 0;

  virtual DValue getValue(std::string const& name) const;
  virtual void setValue(std::string const& name, DValue const &);

  virtual DValue call(std::string const& name, DValue const &);
  virtual DValue call(std::string const& name);

protected:
  DObject(const DObject& rhs) : __dstructDef(rhs.__dstructDef) 
  {
  }
  virtual BaseValue* getBaseValue(size_t index) = 0;
  virtual BaseValue const* getBaseValue(size_t index) const = 0;
  static  BaseValue* getBaseValue(DObject*, size_t index);
  static  BaseValue const* getBaseValue(DObject const*, size_t idx);
  virtual ~DObject()
  {
  }
private:
  DStruct * const __dstructDef;
};

}

#endif
