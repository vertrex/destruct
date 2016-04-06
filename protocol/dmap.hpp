#ifndef DESTRUCT_DMAP_HPP_
#define DESTRUCT_DMAP_HPP_

#include <map>

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

  DMap(DStruct* dstruct, DValue const& args) : DCppObject<DMap<KeyType, KeyTypeId, ValueType, ValueTypeId> >(dstruct, args), __itemStruct(NULL)
  {
    this->init();
  };

  DMap(const DMapType& copy) : DCppObject<DMap<KeyType, KeyTypeId, ValueType, ValueTypeId> >(copy),  __map(copy.__map), __itemStruct(NULL)
  {
    this->init();
  }

  ~DMap()
  {
    delete this->__itemStruct;
  }

  DValue      get(DValue const& args)
  {
    KeyType key = args.get<KeyType>();  

    iteratorType it = this->__map.find(key);
    if (it != this->__map.end())
      return RealValue<ValueType>(it->second);
     
    DUnicodeString error("DMap key : " + args.asUnicodeString() + " not found.");
    throw DException(error);
  }

  DUInt64     size(void)
  {
    return (this->__map.size());
  }

  DObject*    setItem(DValue const& args)
  { 
    DObject*  argumentsObject = args.get<DObject*>();
    KeyType   key = argumentsObject->getValue("index").get<KeyType>();
    ValueType value = argumentsObject->getValue("value").get<ValueType>();

    //map / key add ref or use dvalue directly 
    this->__map[key] = value;

    //argumentsObject->destroy(); //will destroy contained object
 
    return (DNone);
  }

  DObject*    iterator(void)
  {
    DUnicodeString structName = "DMapIterator" + DType(KeyTypeId).name() + DType(ValueTypeId).name();
    return (DStructs::instance().generate(structName, RealValue<DObject*>(this)));
  }

  DObject*    newItem(void)
  {
    DUnicodeString structName = "DMapItem" + DType(KeyTypeId).name() + DType(ValueTypeId).name();
    return (DStructs::instance().generate(structName, RealValue<DObject*>(this)));
  }

  iteratorType begin(void)
  {
    return (this->__map.begin());
  }

  iteratorType end(void)
  {
    return (this->__map.end());
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
  RealValue<DFunctionObject*>    __newItem;
  std::map<KeyType, ValueType>   __map;
  DStruct*                       __itemStruct;
};

}
#endif
