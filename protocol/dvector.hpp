#ifndef DESTRUCT_DVECTOR_HPP_
#define DESTRUCT_DVECTOR_HPP_

#include "protocol/dcppobject.hpp"

namespace Destruct
{

template <typename VectorType, DType::Type_t  VectorTypeId>
class DVector : public DContainer, public DCppObject<DVector<VectorType, VectorTypeId> > //herite en DStruct plutot ?
{
  typedef DVector<VectorType, VectorTypeId> DVectorType;
public:
  RealValue<DFunctionObject* >  pushObject;

//XXX prendre container en argument ? DStruct* dstruct, DValue args) comme ca ca fait un constructeur 
// par contre faut que new object prenne une value donc modifier destruct / dstruct et tous les dobjects
  DVector(DStruct* dstruct, DValue const& args) : DCppObject<DVector<VectorType, VectorTypeId> >(dstruct, args)
  {
    this->init(); //XXX if not init par lui meme mais par dcppobject push sera pas init on dirait qu il init une copie des object qui sont DCOntainer comme si il herite aussi de DContainer ??
  };

  DVector(const DVectorType& copy) : DCppObject<DVector<VectorType, VectorTypeId> >(copy), __vector(copy.__vector) 
  {
    this->init();
  }

  ~DVector()
  {
  }

  DUInt64  push(DValue const& args) 
  {
    this->__vector.push_back(args.get<VectorType>());
    return (this->__vector.size() - 1);
  }
   
  DValue  get(DValue const& args)
  {
    DUInt64 index = args.get<DUInt64>();
    if (index >= this->__vector.size())
      throw DException("DContainer::get bad index\n");
      
    return (RealValue<VectorType>(this->__vector[index]));
  }

  DUInt64   size(void)
  {
    return (this->__vector.size());
  }

  DObject*    setItem(DValue const& args)
  {
    DObject*     argumentsObject = args.get<DObject*>();
    DInt64       index = argumentsObject->getValue("index").get<DUInt64>();
    VectorType     item = argumentsObject->getValue("value").get<VectorType>();
     
    if (index >= (DInt64)this->__vector.size()) 
     throw DException("setItem : Index error");    

    this->__vector[index] = item; 
 
    return (DNone);
  }

  DObject*  iterator(void)
  {
    DObject* iterator = Destruct::instance().generate("DIterator", RealValue<DObject*>(this));
    return (iterator);
  }

/*
 *  DStruct declaration
 */ 
  static size_t ownAttributeCount()
  {
    return (5);
  }

  static DAttribute* ownAttributeBegin()
  {
    static DAttribute  attributes[] = 
    {
      DAttribute(DType::DUInt64Type,"push", VectorTypeId), 
      DAttribute(VectorTypeId, "get",  DType::DUInt64Type),
      DAttribute(DType::DUInt64Type,"size", DType::DNoneType),
      DAttribute(DType::DNoneType, "setItem", DType::DObjectType),
      DAttribute(DType::DObjectType, "iterator", DType::DNoneType),
    };
    return (attributes);
  }

  static DPointer<DVectorType>* memberBegin()
  {
    static DPointer<DVectorType> memberPointer[] = 
    {
      DPointer<DVectorType>(&DVectorType::pushObject, &DVectorType::push),
      DPointer<DVectorType>(&DVectorType::getObject, &DVectorType::get),
      DPointer<DVectorType>(&DVectorType::sizeObject, &DVectorType::size),
      DPointer<DVectorType>(&DVectorType::setItemObject, &DVectorType::setItem),
      DPointer<DVectorType>(&DVectorType::iteratorObject, &DVectorType::iterator),
    };
    return (memberPointer);
  }

  static DAttribute* ownAttributeEnd()
  {
    return (ownAttributeBegin() + ownAttributeCount());
  }

  static DPointer<DVectorType >*  memberEnd()
  {
    return (memberBegin() + ownAttributeCount());
  } 
private:
  std::vector<VectorType>   __vector;
};

}
#endif
