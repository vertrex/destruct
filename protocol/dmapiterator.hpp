#ifndef DESTRUCT_DMAPITERATOR_HPP
#define DESTRUCT_DMAPITERATOR_HPP

/*
 *  This is a protocol object iterator
 *  the protocol need to implem some function 
 *  next(), first(), isDone()  
 */ 

#include "protocol/dmutableobject.hpp"

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
   //this->init();
  }
  DMapItem(const DMapItem& copy) : DCppObject<DMapItemType >(copy), key(copy.key), value(copy.value)
  {
    //thius->init();
  }

  //~DMapItem() //create struct elswhere
  //{
  //}

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
      DAttribute(KeyTypeId, "key"), 
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
class DMapIterator : public DCppObject<DMapIterator<KeyType, KeyTypeId, ValueType, ValueTypeId> > //pas besoin de DMutable car pas a muter ... car on peut avoir le type par template bon ok on va generer tous les types ...
{
  typedef std::map<KeyType, ValueType>          MapType;
  typedef typename MapType::iterator            IteratorType;
  typedef DMapIterator<KeyType, KeyTypeId, ValueType, ValueTypeId > DMapIteratorType;
public:
  IteratorType                begin, end, it;

  DMapIterator(DStruct* dstruct, DValue const& args) : DCppObject<DMapIteratorType >(dstruct, args), index(0), container(NULL) //DObject None ? 
  {
    this->init(); //must be constructed to init 
    this->__itemStruct = makeNewDCpp<DMapItem<KeyType, KeyTypeId, ValueType, ValueTypeId> >("DMapItem");
  }

  DMapIterator(const DMapIterator& copy) : DCppObject<DMapIteratorType >(copy), index(0), container(NULL)
  {
    this->init();
    this->__itemStruct = makeNewDCpp<DMapItem<KeyType, KeyTypeId, ValueType, ValueTypeId> >("DMapItem");
  }

  ~DMapIterator()
  {
    delete this->__itemStruct;
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
    DObject* item = this->__itemStruct->newObject();
    item->setValue("key", RealValue<KeyType>(this->it->first));
    item->setValue("value", RealValue<ValueType>(this->it->second));

    return (RealValue<DObject*>(item));
    //return item;
    //return RealValue<KeyType>(this->it->first);
  }
private:
  RealValue<DUInt64>          index; //signed en python XXX sert a rien c pour les iterator de vector ca ./.non ? 
  RealValue<DObject*>         container; //setContainer pour update le type ?
  RealValue<DFunctionObject*> nextObject, firstObject, isDoneObject, currentItemObject;
  DStruct*                    __itemStruct;

/*
 * DStruct declaration
 */
public:
  static size_t         ownAttributeCount()
  {
    return (6);
  };

  static DAttribute*    ownAttributeBegin()
  {
     static DAttribute  attributes[] = 
     {
       DAttribute(DType::DObjectType, "container"),
       DAttribute(DType::DUInt64Type, "index"), // ??? ca sert a quoi, current key??
       DAttribute(DType::DNoneType, "nextItem", DType::DNoneType), 
       DAttribute(DType::DNoneType, "first", DType::DNoneType),
       DAttribute(DType::DInt8Type, "isDone", DType::DNoneType),
       //DAttribute(KeyTypeId, "currentItem",  DType::DNoneType),
       DAttribute(DType::DObjectType, "currentItem",  DType::DNoneType),
     };
     return (attributes);
  }

  static DPointer<DMapIterator>* memberBegin()
  {
    static DPointer<DMapIterator> memberPointer[] = 
    {
      DPointer<DMapIterator>(&DMapIterator::container),
      DPointer<DMapIterator>(&DMapIterator::index),
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
