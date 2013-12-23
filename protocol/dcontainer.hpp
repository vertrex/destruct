#ifndef DESTRUCT_DCONTAINER_HPP_
#define DESTRUCT_DCONTAINER_HPP_

#include "../dvalue.hpp"
#include "../dcppobject.hpp"
#include "dcontainerbase.hpp"

namespace Destruct 
{

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
  };

  DVector(const DVector<RealType, RealTypeId>& copy) :  __vector(copy.__vector) 
  {
    this->pushObject = new DMethodObject(this, &DVector<RealType, RealTypeId>::push);
    this->getObject = new DMethodObject(this, &DVector<RealType, RealTypeId>::get);
    this->sizeObject = new DMethodObject(this, &DVector<RealType, RealTypeId>::size);
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
    return this->__vector.size();
  }

  static size_t ownAttributeCount()
  {
    return (4);
  }
 
  static DAttribute* ownAttributeBegin()
  {

    static DAttribute  attributes[] = {
                       DAttribute("push", DType::DMethodType, DType::DUInt64Type, RealTypeId), 
                       DAttribute("get",  DType::DMethodType, RealTypeId, DType::DUInt64Type),
                       DAttribute("size", DType::DMethodType, DType::DUInt64Type, DType::DNoneType),
                       DAttribute("iterator", DType::DMethodType, DType::DObjectType, DType::DNoneType)
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
