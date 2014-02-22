#ifndef DESTRUCT_DMAP_HPP_
#define DESTRUCT_DMAP_HPP_

#include <map>
#include "dmapiterator.hpp"

namespace Destruct
{

template <typename KeyType, DType::Type_t  KeyTypeId,
          typename ValueType, DType::Type_t  ValueTypeId>
class DMap : public DContainer, public DCppObject<DMap<KeyType, KeyTypeId, ValueType, ValueTypeId> >
{
  typedef std::map<KeyType, ValueType> MapType;
  typedef typename MapType::iterator            iteratorType;
  typedef DMap<KeyType, KeyTypeId, ValueType, ValueTypeId> DMapType;
public:
  DMap(DStruct* dstruct) : DCppObject<DMap<KeyType, KeyTypeId, ValueType, ValueTypeId> >(dstruct)
  {
  };


  DMap(const DMapType& copy) : DCppObject<DMap<KeyType, KeyTypeId, ValueType, ValueTypeId> >(copy),  __map(copy.__map) 
  {
  }

  DUInt64  push(DValue const& args) 
  {
    throw DException("DMap::push not implemented.");
  }
   
  DValue  get(DValue const& args)
  {
    KeyType key = args.get<KeyType>();//bad cast car recois un index en int au lieu d une key et c pa gerer en cpp :( par index mais par iterator :( donc doit recevoir une clef  

    iteratorType it = this->__map.find(key);
    if (it != this->__map.end())
      return RealValue<ValueType>(it->second);
     
    std::string error("DMap key : " + args.asUnicodeString() + " not found.");
    throw DException(error);
  }

  DUInt64   size(void)
  {
    return (this->__map.size());
  }

  DObject*    setItem(DValue const& args)
  {
    DObject*  argumentsObject = args.get<DObject*>();
    KeyType   key = argumentsObject->getValue("index").get<KeyType>();
    ValueType value = argumentsObject->getValue("value").get<ValueType>();

    this->__map[key] = value;
 
    return (DNone);
  }

  DObject*    iterator(void)
  {
    DStruct* dstruct = makeNewDMutable<DMapIterator<KeyType, ValueType, ValueTypeId > >("DMapIterator");
    DObject* iterator = dstruct->newObject();
    iterator->setValue("container", RealValue<DObject*>(this));

    DMapIterator<KeyType,ValueType, KeyTypeId>* diterator = static_cast<DMapIterator<KeyType, ValueType, KeyTypeId> *>(iterator);

    diterator->begin = this->__map.begin();
    diterator->end = this->__map.end();
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
      DAttribute(DType::DUInt64Type,"push", KeyTypeId), //ok mais encore utile ?
      DAttribute(ValueTypeId, "get",  KeyTypeId), 
      DAttribute(DType::DUInt64Type,"size", DType::DNoneType),
      DAttribute(DType::DNoneType, "setItem", DType::DObjectType),
      DAttribute(DType::DObjectType, "iterator", DType::DNoneType),
    };
    return (attributes);
  }

  static DMemoryPointer<DMapType>* memberBegin()
  {
    static DMemoryPointer<DMapType> memberPointer[] = 
    {
      DMemoryPointer<DMapType>(&DMapType::pushObject, &DMapType::push),
      DMemoryPointer<DMapType>(&DMapType::getObject, &DMapType::get),
      DMemoryPointer<DMapType>(&DMapType::sizeObject, &DMapType::size),
      DMemoryPointer<DMapType>(&DMapType::setItemObject, &DMapType::setItem),
      DMemoryPointer<DMapType>(&DMapType::iteratorObject, &DMapType::iterator),
    };
    return (memberPointer);
  }

  static DAttribute* ownAttributeEnd()
  {
    return (ownAttributeBegin() + ownAttributeCount());
  }

  static DMemoryPointer<DMapType >*  memberEnd()
  {
    return (memberBegin() + ownAttributeCount());
  } 
private:
  std::map<KeyType, ValueType>   __map; //XXX stocker des ref value si on veut que ca soit ref count ... les objet tous ca ... 
};

}
#endif
