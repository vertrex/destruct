#ifndef DESTRUCT_DCONTAINER_HPP_
#define DESTRUCT_DCONTAINER_HPP_

#include "../dvalue.hpp"
#include "dcppobject.hpp"

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

  virtual RealValue<DUInt64>    push(DValue const& args) = 0;
  virtual DValue                get(DValue const& args) = 0;
  virtual RealValue<DUInt64>    size(void) = 0;
  virtual RealValue<DObject*>   setItem(DValue const& args) = 0;

  RealValue<DFunctionObject* >  pushObject;
  RealValue<DFunctionObject* >  getObject;
  RealValue<DFunctionObject* >  sizeObject;
  RealValue<DFunctionObject* >  setItemObject; 
};

//XXX move in DVector.hpp

template <typename RealType, DType::Type_t  RealTypeId>
class DVector : public DContainer
{
  typedef DVector<RealType, RealTypeId> DVectorType;
public:
  DVector()
  {
  };

  DVector(const DVector<RealType, RealTypeId>& copy) :  __vector(copy.__vector) 
  {
  }

  RealValue<DUInt64>  push(DValue const& args) 
  {
    this->__vector.push_back(args.get<RealType>());
    return (this->__vector.size() - 1);
  }
   
  DValue  get(DValue const& args)
  {
    DUInt64 index = args.get<DUInt64>();
    if (index >= this->__vector.size())
      throw DException("DContainer::get bad index\n");
      
    return (RealValue<RealType>(this->__vector[index]));
  }

  RealValue<DUInt64>   size(void)
  {
    return (this->__vector.size());
  }

  RealValue<DObject*>    setItem(DValue const& args)
  {
    DObject*     argumentsObject = args.get<DObject*>();
    DInt64       index = argumentsObject->getValue("index").get<DInt64>();
    RealType     item = argumentsObject->getValue("item").get<RealType>();
     
    if (index >= (DInt64)this->__vector.size()) 
     throw DException("setItem : Index error");    

    this->__vector[index] = item; 
 
    return (DNone);
  }

/*
 *  DStruct declaration
 */ 
  static size_t ownAttributeCount()
  {
    return (4);
  }
 
  static DAttribute* ownAttributeBegin()
  {
    static DAttribute  attributes[] = 
    {
      DAttribute("push", DType::DMethodType, DType::DUInt64Type, RealTypeId), 
      DAttribute("get",  DType::DMethodType, RealTypeId, DType::DUInt64Type),
      DAttribute("size", DType::DMethodType, DType::DUInt64Type, DType::DNoneType),
      DAttribute("setItem", DType::DMethodType, DType::DNoneType, DType::DObjectType),
    };
    return (attributes);
  }

  static DMemoryPointer<DVectorType>* memberBegin()
  {
    static DMemoryPointer<DVectorType> memberPointer[] = 
    {
      DMemoryPointer<DVectorType>(&DVectorType::pushObject, &DVectorType::push),
      DMemoryPointer<DVectorType>(&DVectorType::getObject, &DVectorType::get),
      DMemoryPointer<DVectorType>(&DVectorType::sizeObject, &DVectorType::size),
      DMemoryPointer<DVectorType>(&DVectorType::setItemObject, &DVectorType::setItem),
    };
    return (memberPointer);
  }

  static DAttribute* ownAttributeEnd()
  {
    return (ownAttributeBegin() + ownAttributeCount());
  }

  static DMemoryPointer<DVectorType >*  memberEnd()
  {
    return (memberBegin() + ownAttributeCount());
  } 
private:
  std::vector<RealType>   __vector;
};

}
#endif 
