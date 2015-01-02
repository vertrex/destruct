#ifndef __COMPOSE__
#define __COMPOSE__

#include <stdint.h>

#include "destruct.hpp"
#include "dobject.hpp"
#include "dunicodestring.hpp"
#include "drealvalue.hpp"
#include "protocol/dcppobject.hpp"

using namespace Destruct;

template<typename CppClass>
class ComposableObject : public CppClass
{
public:
  RealValue<DObject*>  inherited;

  ComposableObject<CppClass>(DStruct* dstruct) : CppClass(dstruct)
  {
    this->init();
  }

  ComposableObject<CppClass>(DStruct* dstruct, DValue const& args) : CppClass(dstruct, args)
  {
    this->inherited = args.get<DObject*>();
    this->init();
  }

  static DObject* newObject(DStruct * dstruct, DValue const& args)
  {
    return (new ComposableObject<CppClass>(dstruct, args));
  }

  virtual DValue getValue(size_t idx) const
  {
   return CppClass::getValue(idx);
  }

  virtual DValue getValue(std::string const& name) const
  {
    try
    {
      return CppClass::getValue(name);
    }
    catch (...) //XXX DException // DExceptionAttributeError
    {
     return ((DObject*)inherited)->getValue(name);
    }
  }

  virtual void  setValue(size_t idx, DValue const& v)
  {
    return CppClass::setValue(idx, v);
  }

  virtual void  setValue(std::string const& name, DValue const& v)
  {
    try
    {
      return CppClass::setValue(name, v);
    }
    catch (...) //XXX DException // DExceptionAttributeError
    {
     return ((DObject*)inherited)->setValue(name, v);
    }
  }

  virtual DValue call(std::string const& name, DValue const& args)
  {
    try
    {
      return CppClass::call(name, args);
    }
    catch (...)
    {
      return ((DObject*)inherited)->call(name, args);
    }
  }

  virtual DValue call(std::string const& name)
  {
    try
    {
      return CppClass::call(name);
    }
    catch (...)
    {
      return ((DObject*)inherited)->call(name);
    }
  }
};


class Compose : public DObject
{
public:
  Compose(DStruct* dstruct, DValue const& args);
  //Compose(Compose const&);
  //Compose(DObject*);

  static DObject*   newObject(DStruct *, DValue const& args);

  DObject*          clone() const;
  virtual DValue    getValue(size_t index) const;
  void              setValue(size_t idx, DValue const &);
  virtual DValue    call(size_t index, DValue const &);

  virtual DValue    getValue(std::string const& name) const;
  void              setValue(std::string const& name, DValue const &);

  virtual DValue    call(std::string const& name, DValue const &);
  virtual DValue    call(std::string const& name);
protected:
  ~Compose();

  BaseValue*        getBaseValue(size_t index);
  BaseValue const*  getBaseValue(size_t index) const;
private:
  std::vector<DObject*> __objects;
};

#endif
