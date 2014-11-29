#ifndef DESTRUCT_DMAPITERATOR_HPP
#define DESTRUCT_DMAPITERATOR_HPP

/*
 *  This is a protocol object iterator
 *  the protocol need to implem some function 
 *  next(), first(), isDone()  
 */ 

#include "protocol/dmutableobject.hpp"
#include "protocol/dmap.hpp"

namespace Destruct
{
class DObject;

template <typename KeyType, DType::Type_t KeyTypeId,
          typename ValueType, DType::Type_t ValueTypeId>
class   DMapItem : public DCppObject<DMapItem<KeyType, KeyTypeId, ValueType, ValueTypeId> >
{
  typedef DMapItem<KeyType, KeyTypeId, ValueType, ValueTypeId> DMapItemType;
public:
  DMapItem(DStruct* dstruct, DValue const& args) : DCppObject<DMapItemType>(dstruct, args)
  {
  }
  DMapItem(const DMapItem& copy) : DCppObject<DMapItemType >(copy), key(copy.key), value(copy.value)
  {
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

template< typename KeyType, DType::Type_t KeyTypeId,
          typename ValueType, DType::Type_t ValueTypeId>
class DMapIterator : public DCppObject<DMapIterator<KeyType, KeyTypeId, ValueType, ValueTypeId> >
{
  typedef std::map<KeyType, ValueType>          MapType;
  typedef typename MapType::iterator            IteratorType;
  typedef DMapIterator<KeyType, KeyTypeId, ValueType, ValueTypeId > DMapIteratorType;
public:
  IteratorType                begin, end, it;

  DMapIterator(DStruct* dstruct, DValue const& args) : DCppObject<DMapIteratorType >(dstruct, args), container(args.get<DObject*>())
  {
    this->init();
        
    DMap<KeyType, KeyTypeId, ValueType, ValueTypeId>* map = dynamic_cast<DMap<KeyType, KeyTypeId, ValueType, ValueTypeId>* >(args.get<DObject*>());
    if (map)
    { 
      this->begin = map->begin();
      this->end = map->end();
      this->first();
    }
    else
      throw DException("Can't initialize DMapIterator wrong argument type.");
  }

  DMapIterator(const DMapIterator& copy) : DCppObject<DMapIteratorType >(copy), begin(copy.begin), end(copy.end), container(copy.container)
  {
  }

  ~DMapIterator()
  {
  }

  void                        next(void)
  {
    this->it++;
  }

  void                        first(void)
  {
    this->it = this->begin;
  }

  DInt8                       isDone(void)
  {  
    if (this->it == this->end)
      return (1);

    return (0);
  }

  DValue                        currentItem(void)
  {
    DObject* item = static_cast<DObject*>(this->container)->call("newItem").get<DObject*>();
    item->setValue("index", RealValue<KeyType>(this->it->first));
    item->setValue("value", RealValue<ValueType>(this->it->second));

    return (RealValue<DObject*>(item));
  }
private:
  RealValue<DObject*>         container;
  RealValue<DFunctionObject*> nextObject, firstObject, isDoneObject, currentItemObject;

/*
 * DStruct declaration
 */
public:
  static size_t         ownAttributeCount()
  {
    return (5);
  };

  static DAttribute*    ownAttributeBegin()
  {
     static DAttribute  attributes[] = 
     {
       DAttribute(DType::DObjectType, "container"),
       DAttribute(DType::DNoneType, "nextItem", DType::DNoneType), 
       DAttribute(DType::DNoneType, "first", DType::DNoneType),
       DAttribute(DType::DInt8Type, "isDone", DType::DNoneType),
       DAttribute(DType::DObjectType, "currentItem",  DType::DNoneType),
     };
     return (attributes);
  }

  static DPointer<DMapIterator>* memberBegin()
  {
    static DPointer<DMapIterator> memberPointer[] = 
    {
      DPointer<DMapIterator>(&DMapIterator::container),
      DPointer<DMapIterator>(&DMapIterator::nextObject, &DMapIterator::next),
      DPointer<DMapIterator>(&DMapIterator::firstObject, &DMapIterator::first),
      DPointer<DMapIterator>(&DMapIterator::isDoneObject, &DMapIterator::isDone),
      DPointer<DMapIterator>(&DMapIterator::currentItemObject, &DMapIterator::currentItem),
    };
    return memberPointer;
  }

  static DAttribute*   ownAttributeEnd()
  {
    return (ownAttributeBegin() + ownAttributeCount());
  }

  static DPointer<DMapIterator>* memberEnd()
  { 
    return (memberBegin() + ownAttributeCount()); 
  }
};


}

#endif
