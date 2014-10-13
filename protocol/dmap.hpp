#ifndef DESTRUCT_DMAP_HPP_
#define DESTRUCT_DMAP_HPP_

#include <map>

#include "dmapiterator.hpp"

namespace Destruct
{

template <typename KeyType, DType::Type_t KeyTypeId,
          typename ValueType, DType::Type_t ValueTypeId>
class   DMapItem : public DCppObject<DMapItem<KeyType, KeyTypeId, ValueType, ValueTypeId> >
{
  typedef DMapItem<KeyType, KeyTypeId, ValueType, ValueTypeId> DMapItemType;
public:
  DMapItem(DStruct* dstruct, DValue const& args) : DCppObject<DMapItemType>(dstruct, args)
  {
   //this->init();
  }
  DMapItem(const DMapItem& copy) : DCppObject<DMapItemType >(copy), key(copy.key), value(copy.value)
  {
    //thius->init();
  }

  ~DMapItem()
  {
    delete this->instanceOf();
  }

  RealValue<KeyType>       key;
  RealValue<ValueType>     value;
  static size_t ownAttributeCount()
  {
    return (2);
  }

  static DAttribute* ownAttributeBegin()
  {
    static DAttribute  attributes[] = 
    {
      DAttribute(KeyTypeId, "index"), 
      DAttribute(ValueTypeId,"value"),
    };
    return (attributes);
  }

  static DPointer<DMapItemType>* memberBegin()
  {
    static DPointer<DMapItemType> memberPointer[] = 
    {
      DPointer<DMapItemType>(&DMapItemType::key),
      DPointer<DMapItemType>(&DMapItemType::value),
    };
    return (memberPointer);
  }

  static DAttribute* ownAttributeEnd()
  {
    return (ownAttributeBegin() + ownAttributeCount());
  }

  static DPointer<DMapItemType >*  memberEnd()
  {
    return (memberBegin() + ownAttributeCount());
  } 
};

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
    this->init();
    //std::string name = "DMapItem" + DType(KeyTypeId).name() + DType(ValueTypeId).name(); // for deserializaion ?
    this->__itemStruct = makeNewDCpp<DMapItem<KeyType, KeyTypeId, ValueType, ValueTypeId> >("DMapItem");
  };

  DMap(const DMapType& copy) : DCppObject<DMap<KeyType, KeyTypeId, ValueType, ValueTypeId> >(copy),  __map(copy.__map) 
  {
    this->init();
    this->__itemStruct = makeNewDCpp<DMapItem<KeyType, KeyTypeId, ValueType, ValueTypeId> >("DMapItem");
  }

  ~DMap()
  {
    delete this->__itemStruct;
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
    KeyType   key = argumentsObject->getValue("key").get<KeyType>(); //XXX KEY KEY 
    ValueType value = argumentsObject->getValue("value").get<ValueType>();

    this->__map[key] = value;
 
    return (DNone);
  }

  DObject*    iterator(void)
  {
    DStruct* dstruct = makeNewDCpp<DMapIterator<KeyType, KeyTypeId, ValueType,  ValueTypeId > >("DMapIterator");
    DObject* iterator = dstruct->newObject(RealValue<DObject*>(this));
   
    DMapIterator<KeyType, KeyTypeId, ValueType, ValueTypeId>* diterator = static_cast<DMapIterator<KeyType, KeyTypeId, ValueType, ValueTypeId> *>(iterator);

    diterator->begin = this->__map.begin();
    diterator->end = this->__map.end();
    diterator->first();

    return (iterator);
  }

  DObject* newItem(void)
  {
//    return (new DMapItem<a,b,c,d>(const dstruct, DNone);
    return this->__itemStruct->newObject(); //newItem(key, value) ???
  }

  /*
   *  DStruct declaration
   */ 
>>>>>>> e97570450d96013b902561d583b8c5fe5d070d11
  static size_t ownAttributeCount()
  {
    return (5);
  }

  static DAttribute* ownAttributeBegin()
  {
    static DAttribute  attributes[] = 
    {
      DAttribute(ValueTypeId, "get",  KeyTypeId), 
      DAttribute(DType::DUInt64Type,"size", DType::DNoneType),
      DAttribute(DType::DNoneType, "setItem", DType::DObjectType),
      DAttribute(DType::DObjectType, "iterator", DType::DNoneType),
      DAttribute(DType::DObjectType, "newItem", DType::DNoneType),
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
      DPointer<DMapType>(&DMapType::__newItem, &DMapType::newItem),
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
  DStruct*                       __itemStruct;
  RealValue<DFunctionObject*>    __newItem;
  std::map<KeyType, ValueType>   __map; //XXX stocker des ref value si on veut que ca soit ref count ... les objet tous ca ... 
};

}
#endif
