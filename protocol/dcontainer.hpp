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
  }

  DContainer(const RealType& copy)
  {
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
      diterator->first();
      diterator->container(RealValue<DObject*>(self));

      return (diterator);
    }

    return RealValue<DObject*>(DNone);
  }
};

template <typename RealType, DType::Type_t  RealTypeId>
class DVector : public DContainer<DVector<RealType, RealTypeId>, RealTypeId, RealType > 
{
  typedef DVector<RealType, RealTypeId> DVectorType;
public:
  DVector()
  {
  };

  DVector(const DVector<RealType, RealTypeId>& copy) :  __vector(copy.__vector) 
  {
  }

  //using DContainer<DVector<RealType, RealTypeId>, RealTypeId, RealType >::iterator;
  RealValue<DObject*>    iterator(void)
  {
    return DContainer<DVector<RealType, RealTypeId>, RealTypeId, RealType >::iterator();
  };

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
    return (5);
  }
 
  static DAttribute* ownAttributeBegin()
  {
    static DAttribute  attributes[] = 
    {
      DAttribute("push", DType::DMethodType, DType::DUInt64Type, RealTypeId), 
      DAttribute("get",  DType::DMethodType, RealTypeId, DType::DUInt64Type),
      DAttribute("size", DType::DMethodType, DType::DUInt64Type, DType::DNoneType),
      DAttribute("setItem", DType::DMethodType, DType::DNoneType, DType::DObjectType), 
      DAttribute("iterator", DType::DMethodType, DType::DObjectType, DType::DNoneType),
    };
    return (attributes);
  }

  static DAttribute* ownAttributeEnd()
  {
    return (ownAttributeBegin() + ownAttributeCount());
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

      //XXX ?
      //DMemoryPointer<DVectorType>(&DVectorType::iteratorObject); //cree le DRealValue et le stock encore mieux ... car au final on a pas besoin d y accedder directemnet a par si on veut bypasse le mop pour aller plus vite et garder la compatibilite puisqu on call un truc qui pointer ves ququchose donc peut etre modifier ;  iteratorObject->call(); directement meux que iterator::iterator() car lui ne gere pas l overloading et que object->call("iterator") car lui et plus lent et si on connais le type on a pas besoin faut aussi la possibiltie de pouvor appeller la methode de l objet si overwrittent et vu qu on la stcok c possible maintnant :) appeller la method de la class parent Object::iteratorObject 
    };
    return (memberPointer);
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
