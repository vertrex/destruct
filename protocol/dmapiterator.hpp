#ifndef DESTRUCT_DMAPITERATOR_HPP
#define DESTRUCT_DMAPITERATOR_HPP

/*
 *  This is a protocol object iterator
 *  the protocol need im plem of some function 
 *  next(), first(), isDone()  
 */ 
#include "dvalue.hpp"
#include "dmethodobject.hpp"

#include "protocol/dmemberpointer.hpp"
#include "protocol/dcppmutable.hpp"
#include "protocol/dcontainer.hpp"

class DObject;

namespace Destruct
{

template< typename KeyType, typename ValueType, DType::Type_t ValueTypeId>
class DMapIterator : public DCppMutable<DMapIterator<KeyType, ValueType, ValueTypeId> > //pas besoin de DMutable car pas a muter ... car on peut avoir le type par template bon ok on va generer tous les types ...
{
  typedef std::map<KeyType, ValueType>          MapType;
  typedef typename MapType::iterator            IteratorType;
  typedef DMapIterator<KeyType, ValueType, ValueTypeId > DMapType;
public:
  DMapIterator() : DCppMutable<DMapType >(new DMutableStruct(NULL, "DMapIterator", DMapType::newObject, DMapType::ownAttributeBegin(), DMapType::ownAttributeEnd())), index(0), container(NULL) //DObject None ? 
  {
    this->init(); //must be constructed to init 
    //this->first(); 
  }

  DMapIterator(const DMapIterator& copy) : DCppMutable<DMapType >(copy), index(0), container(NULL)
  {
    this->init();
  }

  IteratorType                begin;
  IteratorType                 end;
  RealValue<DUInt64>          index; //signed en python
  RealValue<DObject*>         container; //setContainer pour update le type ?
  RealValue<DFunctionObject*> nextObject;
  RealValue<DFunctionObject*> firstObject;
  RealValue<DFunctionObject*> isDoneObject;
  RealValue<DFunctionObject*> currentItemObject;

  void                        next(void)
  {
    this->begin++;
  }

  void                        first(void)
  {
     std::cout << "DMap first " << std::endl;
     //XXX 
  }

  DInt8                       isDone(void)
  {  
    if (this->begin == this->end)
      return (1);

    return (0);
  }

  DValue                      currentItem(void) 
  {
    return RealValue<ValueType>(this->begin->first);
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

  static DMemoryPointer<DMapIterator>* memberBegin()
  {
    static DMemoryPointer<DMapIterator> memberPointer[] = 
    {
      DMemoryPointer<DMapIterator>(&DMapIterator::container),
      DMemoryPointer<DMapIterator>(&DMapIterator::index),
      DMemoryPointer<DMapIterator>(&DMapIterator::nextObject, &DMapIterator::next),
      DMemoryPointer<DMapIterator>(&DMapIterator::firstObject, &DMapIterator::first),
      DMemoryPointer<DMapIterator>(&DMapIterator::isDoneObject, &DMapIterator::isDone),
      DMemoryPointer<DMapIterator>(&DMapIterator::currentItemObject, &DMapIterator::currentItem),
    };
    return memberPointer;
  }

  static DAttribute*   ownAttributeEnd()
  {
    return (ownAttributeBegin() + ownAttributeCount());
  }

  static DMemoryPointer<DMapIterator>* memberEnd()
  { 
    return (memberBegin() + ownAttributeCount()); 
  }
};


}

#endif
