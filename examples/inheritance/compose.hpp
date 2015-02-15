#ifndef __COMPOSE__
#define __COMPOSE__

#include <stdint.h>

#include "dstructs.hpp"
#include "dobject.hpp"
#include "dunicodestring.hpp"
#include "drealvalue.hpp"
#include "protocol/dcppobject.hpp"

using namespace Destruct;


//XXX directement ds DCPpObject ? 
// pas possible ds object car virtual pure

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
    //this->object = 
    this->inherited = args.get<DObject*>(); //lists d object si plusieurs inheritance ?
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

  virtual DValue getValue(DUnicodeString const& name) const
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

  virtual void  setValue(DUnicodeString const& name, DValue const& v)
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

  virtual DValue call(DUnicodeString const& name, DValue const& args)
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

  virtual DValue call(DUnicodeString const& name)
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

class DComposedStruct;

class Compose : public DObject
{
public:
  Compose(DComposedStruct* dstruct, DValue const& args);
  //Compose(Compose const&);
  //Compose(DObject*);

  static DObject*   newObject(DStruct *, DValue const& args);

  DObject*          clone() const;
  virtual DValue    getValue(size_t index) const;
  void              setValue(size_t idx, DValue const &);
  virtual DValue    call(size_t index, DValue const &);

  virtual DValue    getValue(DUnicodeString const& name) const;
  void              setValue(DUnicodeString  const& name, DValue const &);

  virtual DValue    call(DUnicodeString const& name, DValue const &);
  virtual DValue    call(DUnicodeString const& name);
protected:
  ~Compose();

  BaseValue*        getBaseValue(size_t index);
  BaseValue const*  getBaseValue(size_t index) const;
private:
  std::vector<DObject*> __objects;
};

class DComposedStruct : public DStruct
{
public:
  DComposedStruct(DStruct const* base, DStruct const* second, const DUnicodeString& name) : DStruct(NULL, name, Compose::newObject)
  {
    this->__inherit.push_back(base);
    this->__inherit.push_back(second);
 
    DStruct::DAttributeIterator attribute = base->attributeBegin();
    for (; attribute != base->attributeEnd(); ++attribute)
     this->addAttribute(*attribute); 
  
    attribute = second->attributeBegin();
    for (; attribute != second->attributeEnd(); ++attribute)
      this->addAttribute(*attribute);
  }

  template<typename Iterator>
  DComposedStruct(DStruct const* base, const DUnicodeString& name, CreateObjectFunction objectFunction, Iterator attributeBegin, Iterator attributeEnd ) : DStruct(base, name, Compose::newObject, attributeBegin, attributeEnd)
  {
    this->__inherit.push_back(base);
                                        //name ?? overwrite DStruct(name) ?
    DStruct* second = new DStruct(NULL, name, objectFunction, attributeBegin, attributeEnd);
    this->__inherit.push_back(second);
  }

  std::vector<DStruct const*> inherit(void)
  {
    return (this->__inherit);
  }
private:
  std::vector<DStruct const*> __inherit;
};

#endif
