#ifndef DESTRUCT_DMAPITERATOR_HPP
#define DESTRUCT_DMAPITERATOR_HPP

/*
 *  This is a protocol object iterator
 *  the protocol need im plem of some function 
 *  next(), first(), isDone()  
 */ 

class DObject;

namespace Destruct
{

template< typename KeyType, DType::Type_t KeyTypeId,
          typename ValueType, DType::Type_t ValueTypeId>
class DMapIterator : public DCppObject<DMapIterator<KeyType, KeyTypeId, ValueType, ValueTypeId> > //pas besoin de DMutable car pas a muter ... car on peut avoir le type par template bon ok on va generer tous les types ...
{
  typedef std::map<KeyType, ValueType>          MapType;
  typedef typename MapType::iterator            IteratorType;
  typedef DMapIterator<KeyType, KeyTypeId, ValueType, ValueTypeId > DMapIteratorType;
public:
  DMapIterator(DStruct* dstruct, DValue const& args) : DCppObject<DMapIteratorType >(dstruct, args), index(0), container(NULL) //DObject None ? 
  {
    this->init(); //must be constructed to init 
  }

  DMapIterator(const DMapIterator& copy) : DCppObject<DMapIteratorType >(copy), index(0), container(NULL)
  {
    this->init();
  }

  IteratorType                begin;
  IteratorType                end;
  IteratorType                it;
  RealValue<DUInt64>          index; //signed en python
  RealValue<DObject*>         container; //setContainer pour update le type ?
  RealValue<DFunctionObject*> nextObject;
  RealValue<DFunctionObject*> firstObject;
  RealValue<DFunctionObject*> isDoneObject;
  RealValue<DFunctionObject*> currentItemObject;

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

  DValue                      currentItem(void) 
  {
    return RealValue<ValueType>(this->it->first);
  }

/*
 * DStruct declaration
 */
  static size_t         ownAttributeCount()
  {
    return (6);
  };

  static DAttribute*    ownAttributeBegin()
  {
     static DAttribute  attributes[] = 
     {
       DAttribute(DType::DObjectType, "container"),
       DAttribute(DType::DUInt64Type, "index"),
       DAttribute(DType::DNoneType, "nextItem", DType::DNoneType), 
       DAttribute(DType::DNoneType, "first", DType::DNoneType),
       DAttribute(DType::DInt8Type, "isDone", DType::DNoneType),
       DAttribute(ValueTypeId, "currentItem",  DType::DNoneType),
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
