#ifndef DESTRUCT_DITERATOR_HPP
#define DESTRUCT_DITERATOR_HPP

/*
 *  This is a protocol object iterator
 *  the protocol need im plem of some function 
 *  next(), first(), isDone()  
 */ 
#include "../dvalue.hpp"
#include "../dcppmutable.hpp"
#include "../dmemberpointer.hpp"
#include "../dmethodobject.hpp"
#include "dcontainer.hpp"

class DObject;

namespace Destruct
{

class DIterator : public DCppMutable<DIterator>
{
public:
  RealValue<DUInt64>          index; //signed en python
  RealValue<DObject*>         container; //setContainer pour update le type ?
  RealValue<DFunctionObject*> nextObject;
  RealValue<DFunctionObject*> firstObject;
  RealValue<DFunctionObject*> isDoneObject;
  RealValue<DFunctionObject*> currentItemObject;

  DIterator() : DCppMutable(new DMutableStruct(NULL, "DIterator", DIterator::newObject, DIterator::ownAttributeBegin(), DIterator::ownAttributeEnd())), index(0), container(NULL) //DObject None ? 
  {
    this->init(); //must be constructed to init  
  }

  DIterator(const DIterator& copy) : DCppMutable(copy), index(0), container(NULL)
  {
    this->init();
  }

  void  next(void);
  void  first(void);
  void setValue(size_t idx, DValue const& v);
  RealValue<DInt8>      isDone(void);
  DValue currentItem(void);

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
       DAttribute("container", DType::DObjectType),
       DAttribute("index", DType::DUInt64Type),
       DAttribute("next",   DType::DMethodType, DType::DNoneType, DType::DNoneType), 
       DAttribute("first",  DType::DMethodType, DType::DNoneType, DType::DNoneType),
       DAttribute("isDone", DType::DMethodType, DType::DInt8Type, DType::DNoneType),
       DAttribute("currentItem",  DType::DMethodType, DType::DUnknownType, DType::DNoneType),
     };
     return (attributes);
  }

  static DMemoryPointer<DIterator>* memberBegin()
  {
    static DMemoryPointer<DIterator> memberPointer[] = 
    {
      DMemoryPointer<DIterator>(&DIterator::container),
      DMemoryPointer<DIterator>(&DIterator::index),
      DMemoryPointer<DIterator>(&DIterator::nextObject, &DIterator::next),
      DMemoryPointer<DIterator>(&DIterator::firstObject, &DIterator::first),
      DMemoryPointer<DIterator>(&DIterator::isDoneObject, &DIterator::isDone),
      DMemoryPointer<DIterator>(&DIterator::currentItemObject, &DIterator::currentItem),
    };
    return memberPointer;
  }

  static DAttribute*   ownAttributeEnd()
  {
    return (ownAttributeBegin() + ownAttributeCount());
  }

  static DMemoryPointer<DIterator>* memberEnd()
  { 
    return (memberBegin() + ownAttributeCount()); 
  }
};


}

#endif
