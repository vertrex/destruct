#ifndef DESTRUCT_DCONTAINER_HPP_
#define DESTRUCT_DCONTAINER_HPP_

#include "../dvalue.hpp"
#include "../dcppobject.hpp"
#include "dcontainerbase.hpp"

namespace Destruct 
{


//template <typename RealType, DType::Type_t RealTypeId>
//class DItem
//{
//public:
  //DItem()
  //{
  //}

  //DItem(const DItem<RealType, RealTypeId>& copy) : index(copy.index), item(copy.item) 
  //{
  //}

  //RealValue<DUInt64>  index;
  //RealValue<RealType> item;

  //static size_t ownAttributeCount()
  //{
    //return (2);
  //}
 
  //static DAttribute* ownAttributeBegin()
  //{

    //static DAttribute  attributes[] = {
                       //DAttribute("item", RealTypeId), 
                       //DAttribute("index",  DType::DUInt64Type)
                                      //};
    //return (attributes);
  //}

  //static DAttribute* ownAttributeEnd()
  //{
    //return (ownAttributeBegin() + ownAttributeCount());
  //}

  //static DMemoryPointer<DItem<RealType, RealTypeId> >* memberBegin()
  //{
    //static DMemoryPointer<DItem<RealType, RealTypeId> > memberPointer[] = 
    //{
      //DMemoryPointer<DItem<RealType, RealTypeId> >(&DItem<RealType, RealTypeId>::index),
      //DMemoryPointer<DItem<RealType, RealTypeId> >(&DItem<RealType, RealTypeId>::item),
    //};
    //return (memberPointer);
  //}

  //static DMemoryPointer<DItem<RealType, RealTypeId> >*  memberEnd()
  //{
    //return (memberBegin() + ownAttributeCount());
  //} 
//};

template <typename RealType, DType::Type_t RealTypeId, typename RealTypee >
class DContainer : public DContainerBase 
{
public:
  DContainer()
  {
    this->iteratorObject = new DMethodObject(this, &DContainer< RealType, RealTypeId, RealTypee >::iterator);
  }

  DContainer(const RealType& copy)
  {
    this->iteratorObject = new DMethodObject(this, &DContainer< RealType, RealTypeId, RealTypee >::iterator);
  }

  RealValue<DObject*>    iterator(void)
  {
    DStruct* iteratorStruct = makeNewDClass< DIterator<RealTypee, RealTypeId> >(NULL, "DIterator");
    DIterator<RealTypee, RealTypeId>* iterator = new DIterator<RealTypee, RealTypeId>();
    DClassObject<DIterator<RealTypee, RealTypeId> >* diterator = makeNewDObject<DIterator<RealTypee, RealTypeId> >(iteratorStruct, *iterator);
                                    
    DStruct* selfStruct = makeNewDClass< RealType >(NULL, "DContainer");
    DClassObject<RealType>* self = makeNewDObject< RealType >(selfStruct, *(RealType *)this); 

    if (self)
    {
      iterator->first();
      iterator->container(RealValue<DObject*>(self));

      return (diterator);
    }

    return RealValue<DObject*>(DNone);
  }
};

template <typename RealType, DType::Type_t  RealTypeId>
class DVector : public DContainer<DVector<RealType, RealTypeId>, RealTypeId, RealType > 
{
public:
  DVector()
  {
    this->pushObject = new DMethodObject(this, &DVector<RealType, RealTypeId>::push);
    this->getObject = new DMethodObject(this, &DVector<RealType, RealTypeId>::get);
    this->sizeObject = new DMethodObject(this, &DVector<RealType, RealTypeId>::size);
    this->setItemObject = new DMethodObject(this, &DVector<RealType, RealTypeId>::setItem);
  };

  DVector(const DVector<RealType, RealTypeId>& copy) :  __vector(copy.__vector) 
  {
    this->pushObject = new DMethodObject(this, &DVector<RealType, RealTypeId>::push);
    this->getObject = new DMethodObject(this, &DVector<RealType, RealTypeId>::get);
    this->sizeObject = new DMethodObject(this, &DVector<RealType, RealTypeId>::size);
    this->setItemObject = new DMethodObject(this, &DVector<RealType, RealTypeId>::setItem);
  }

  RealValue<DUInt64>  push(DValue const& args) 
  {
    this->__vector.push_back(args.get<RealType>());
 
    return (this->__vector.size() - 1);
  }
   
  RealValue<RealType> get(DValue const& args)
  {
    DUInt64 index = args.get<DUInt64>();
    if (index >= this->__vector.size())
    {
      std::cout << "DContainer error in get : bad index" << std::endl;
      throw DException("DContainer::get bad index\n");
    }
      
    return (this->__vector[index]);
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
     
    this->__vector[index] = item; 
     
    return (DNone);
  }

  static size_t ownAttributeCount()
  {
    return (5);
  }
 
  static DAttribute* ownAttributeBegin()
  {

    static DAttribute  attributes[] = {
                       DAttribute("push", DType::DMethodType, DType::DUInt64Type, RealTypeId), 
                       DAttribute("get",  DType::DMethodType, RealTypeId, DType::DUInt64Type),
                       DAttribute("size", DType::DMethodType, DType::DUInt64Type, DType::DNoneType),
                       DAttribute("iterator", DType::DMethodType, DType::DObjectType, DType::DNoneType),
                       DAttribute("setItem", DType::DMethodType, DType::DNoneType, DType::DObjectType), 
                                      };
    return (attributes);
  }

  static DAttribute* ownAttributeEnd()
  {
    return (ownAttributeBegin() + ownAttributeCount());
  }

  static DMemoryPointer<DVector<RealType, RealTypeId> >* memberBegin()
  {
    static DMemoryPointer<DVector<RealType, RealTypeId> > memberPointer[] = 
    {
      DMemoryPointer<DVector<RealType, RealTypeId> >(&DVector<RealType, RealTypeId>::pushObject),
      DMemoryPointer<DVector<RealType, RealTypeId> >(&DVector<RealType, RealTypeId>::getObject),
      DMemoryPointer<DVector<RealType, RealTypeId> >(&DVector<RealType, RealTypeId>::sizeObject),
      DMemoryPointer<DVector<RealType, RealTypeId> >(&DVector<RealType, RealTypeId>::iteratorObject),
      DMemoryPointer<DVector<RealType, RealTypeId> >(&DVector<RealType, RealTypeId>::setItemObject)
    };
    return (memberPointer);
  }

  static DMemoryPointer<DVector<RealType, RealTypeId> >*  memberEnd()
  {
    return (memberBegin() + ownAttributeCount());
  } 
private:
  std::vector<RealType>   __vector;
};

}
#endif 
