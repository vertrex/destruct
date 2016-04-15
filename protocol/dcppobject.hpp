#ifndef DSTRUCT_CPP_OBJECT_HPP_
#define DSTRUCT_CPP_OBJECT_HPP_

#include "dstructs.hpp"
#include "protocol/dcpppointer.hpp"

namespace Destruct
{

/*
 *  This object protocol is used to bind CPP object to MOP
 *  This is a multi-inheritance adaptor pattern wich get object trough const reference (copy the passe object)
 *  (Take care of memory leak :) 
 *  This let you acces object method through the MOP 
 *  
 */
template<typename CppClass>
class DCppObject : public DObject
{
public:
  DCppObject(DStruct* dstruct) : DObject(dstruct, RealValue<DObject*>(DNone)), __members(CppClass::memberBegin())
  {
  }

  DCppObject(DStruct* dstruct, DValue const& args) : DObject(dstruct, args), __members(CppClass::memberBegin()) 
  {
  }

  DCppObject(DCppObject const& copy) : DObject(copy), __members(CppClass::memberBegin())
  {
    //this->copy(this, copy);  //will work for normal object must does it will work for method as init must be init late ? XXX must test with method to be sure if used here will be easier or maybe use an object between to avoid having to call copy everywhere, must take care to copy in all object only init right now in most of constructor
  }

  void  init(void) //Must be cal by inherited object constructor // or must use a between class to wrap ?
  {
    //use default object value and set it like in ddynamicobject 
    std::vector<BaseValue*> baseValue;
    if (this->baseObject())
    {
      DObject* baseObject = this->baseObject();
      const DStruct* base = this->base();
   
      int32_t attributeCount = (int32_t)base->attributeCount();
      for (int32_t index = 0; index < attributeCount; ++index)
        this->__baseValue.push_back(baseObject->getBaseValue(index));
    }

    for (size_t idx = 0; idx < CppClass::ownAttributeCount(); ++idx)
    {
      this->__members[idx].init(static_cast<CppClass*>(this));
      this->__baseValue.push_back(this->getBaseValue(idx));
    }
  }

  static DObject* newObject(DStruct * dstruct, DValue const& args)
  {
    return (new CppClass(dstruct, args));
  }

  virtual DObject* clone() const
  {
    return (new CppClass((const CppClass&)* dynamic_cast<const CppClass *>(this)));
  }

  using DObject::getValue;
  using DObject::setValue;
  using DObject::call;

  virtual void   setValue(size_t idx, DValue const& v)
  {
    BaseValue* p = this->__baseValue[idx];
    p->set(v);
  }

  virtual DValue getValue(size_t idx) const
  {
    return (this->__baseValue[idx]->getFinal());
  }

                                 //auto template possible for args ?
  virtual DValue call(size_t idx, DValue const & args)
  {
    DValue v = this->__baseValue[idx]->getFinal();
    DFunctionObject* fo = v.get<DFunctionObject*>();

    try 
    {
      DValue res = fo->call(args);
      //fo->destroy();
      return (res);
    }
    catch (DException const& exception)
    {
      throw DException(this->instanceOf()->name() + "::" + this->instanceOf()->attribute(idx).name() + "("+ args.asUnicodeString() + ") Error : \n" + exception.error());
    }
  }

protected:
  void copy(DCppObject* self, DCppObject const& rhs)
  {
    this->init(); //still need to init all as we make copy ? 
    for (size_t idx = 0; idx != rhs.__baseValue.size(); ++idx)
       this->__baseValue[idx] = rhs.__baseValue[idx]->clone(self);
  }

  virtual BaseValue* getBaseValue(size_t idx)
  {
    return (&this->__members[idx].value(static_cast<CppClass *>(this)));
  }

  virtual BaseValue const* getBaseValue(size_t idx) const
  {
    return (&this->__members[idx].value(static_cast<const CppClass *>(this)));
  }

  virtual ~DCppObject()
  {
  };

  DPointer<CppClass>*       __members;
  std::vector<BaseValue*>   __baseValue;
};


template <typename CppClass>
inline 
DStruct*  makeNewDCpp(DUnicodeString const& name)
{
  return (new DStruct(NULL, name, DCppObject<CppClass>::newObject, CppClass::ownAttributeBegin(), CppClass::ownAttributeEnd()));
}

template <typename CppClass>
inline 
void  registerDCpp(DUnicodeString const& name)
{
  DStructs::instance().registerDStruct(new DStruct(NULL, name, DCppObject<CppClass>::newObject, CppClass::ownAttributeBegin(), CppClass::ownAttributeEnd()));
}


template<typename CppClass>
class DCppObjectSingleton : public DCppObject<CppClass>
{
public:
  DCppObjectSingleton(DStruct* dstruct) : DCppObject<CppClass>(dstruct)
  {
  }

  DCppObjectSingleton(DStruct* dstruct, DValue const& args) : DCppObject<CppClass>(dstruct, args)
  {
  }

  DCppObjectSingleton(DCppObjectSingleton const& copy) : DCppObject<CppClass>(copy)
  {
//    this->copy(this, copy); //must copy all attributes // or let's do it in CppClass constructor ?
  }

  static DObject* newObject(DStruct* dstruct, DValue const& args)
  { 
    static DObject* object = new CppClass(dstruct, args);
    object->addRef();

    return (object);
  }

  virtual DObject* clone() const
  {
    DObject* object = const_cast<DObject*>(static_cast<const DObject* >(const_cast<const DCppObjectSingleton<CppClass> *>(this)));
    object->addRef();

    return (object);
  }

  //destructor ?
};

template <typename CppClass>
inline 
DStruct*  makeNewDCppSingleton(DUnicodeString const& name)
{
  return (new DStruct(NULL, name, DCppObjectSingleton<CppClass>::newObject, CppClass::ownAttributeBegin(), CppClass::ownAttributeEnd()));
}

template <typename CppClass>
inline 
void  registerDCppSingleton(DUnicodeString const& name)
{
  DStructs::instance().registerDStruct(new DStruct(NULL, name, DCppObjectSingleton<CppClass>::newObject, CppClass::ownAttributeBegin(), CppClass::ownAttributeEnd()));
}

}
#endif
