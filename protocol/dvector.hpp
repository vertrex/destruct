#ifndef DESTRUCT_DVECTOR_HPP_
#define DESTRUCT_DVECTOR_HPP_

#include "protocol/dcppobject.hpp"

namespace Destruct
{

template <typename RealType, DType::Type_t  RealTypeId>
class DVector : public DContainer, public DCppObject<DVector<RealType, RealTypeId> > //herite en DStruct plutot ?
{
  typedef DVector<RealType, RealTypeId> DVectorType;
public:

  DVector(DStruct* dstruct) : DCppObject<DVector<RealType, RealTypeId> >(dstruct)
  {
  };

  DVector(const DVectorType& copy) : DCppObject<DVector<RealType, RealTypeId> >(copy), __vector(copy.__vector) 
  {
  }

  DUInt64  push(DValue const& args) 
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

  DUInt64   size(void)
  {
    return (this->__vector.size());
  }

  DObject*    setItem(DValue const& args)
  {
    DObject*     argumentsObject = args.get<DObject*>();
    DInt64       index = argumentsObject->getValue("index").get<DUInt64>();
    RealType     item = argumentsObject->getValue("value").get<RealType>();
     
    if (index >= (DInt64)this->__vector.size()) 
     throw DException("setItem : Index error");    

    this->__vector[index] = item; 
 
    return (DNone);
  }
 

  DObject*  iterator(void)
  {
    DObject* iterator = Destruct::instance().generate("DIterator");
    iterator->setValue("container", RealValue<DObject*>(this));

    return (iterator);
  }

/*
 *  DStruct declaration
 */ 
  static size_t ownAttributeCount()
  {
    return (5);
  }

//XXX mettre ds DCOntainer pour pouvoir heriter en python de DCOntainerString 
// et overwrite lse method par des methodes python :) 
// ca devrait marcher et permet de faire un container complement different en python
// sanns overwrite vector car par ex un pycontainerlinkedlist aurait plus a herite de dcontainer
// que dvector   
  static DAttribute* ownAttributeBegin()
  {
    static DAttribute  attributes[] = 
    {
      DAttribute(DType::DUInt64Type,"push", RealTypeId), 
      DAttribute(RealTypeId, "get",  DType::DUInt64Type),
      DAttribute(DType::DUInt64Type,"size", DType::DNoneType),
      DAttribute(DType::DNoneType, "setItem", DType::DObjectType),
      DAttribute(DType::DObjectType, "iterator", DType::DNoneType),
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
      DMemoryPointer<DVectorType>(&DVectorType::iteratorObject, &DVectorType::iterator),
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
