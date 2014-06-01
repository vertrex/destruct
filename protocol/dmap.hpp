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
  typedef std::map<KeyType, ValueType>                     MapType;
  typedef typename MapType::iterator                       iteratorType;
  typedef DMap<KeyType, KeyTypeId, ValueType, ValueTypeId> DMapType;
public:
  DMap(DStruct* dstruct, DValue const& args) : DCppObject<DMap<KeyType, KeyTypeId, ValueType, ValueTypeId> >(dstruct, args)
  {
  };


  DMap(const DMapType& copy) : DCppObject<DMap<KeyType, KeyTypeId, ValueType, ValueTypeId> >(copy),  __map(copy.__map) 
  {
  }

  DValue  get(DValue const& args)
  {
    KeyType key = args.get<KeyType>();  

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
    DStruct* dstruct = makeNewDCpp<DMapIterator<KeyType, KeyTypeId, ValueType,  ValueTypeId > >("DMapIterator");
    DObject* iterator = dstruct->newObject(RealValue<DObject*>(this));
   
    DMapIterator<KeyType, KeyTypeId, ValueType, KeyTypeId>* diterator = static_cast<DMapIterator<KeyType, KeyTypeId, ValueType, ValueTypeId> *>(iterator);

    diterator->begin = this->__map.begin();
    diterator->end = this->__map.end();
    diterator->first();

    return (iterator);
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
      DAttribute(ValueTypeId, "get",  KeyTypeId), 
      DAttribute(DType::DUInt64Type,"size", DType::DNoneType),
      DAttribute(DType::DNoneType, "setItem", DType::DObjectType),
      DAttribute(DType::DObjectType, "iterator", DType::DNoneType),
    };
    return (attributes);
  }

  static DPointer<DMapType>* memberBegin()
  {
    static DPointer<DMapType> memberPointer[] = 
    {
      DPointer<DMapType>(&DMapType::_get, &DMapType::get),
      DPointer<DMapType>(&DMapType::_size, &DMapType::size),
      DPointer<DMapType>(&DMapType::_setItem, &DMapType::setItem),
      DPointer<DMapType>(&DMapType::_iterator, &DMapType::iterator),
    };
    return (memberPointer);
  }

  static DAttribute* ownAttributeEnd()
  {
    return (ownAttributeBegin() + ownAttributeCount());
  }

  static DPointer<DMapType >*  memberEnd()
  {
    return (memberBegin() + ownAttributeCount());
  } 
private:
  std::map<KeyType, ValueType>   __map; //XXX stocker des ref value si on veut que ca soit ref count ... les objet tous ca ... 
};

}
#endif
