#ifndef DESTRUCT_DITERATOR_HPP
#define DESTRUCT_DITERATOR_HPP

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

class DIterator : public DCppMutable<DIterator>
{
public:
  DIterator() : DCppMutable(new DMutableStruct(NULL, "DIterator", DIterator::newObject, DIterator::ownAttributeBegin(), DIterator::ownAttributeEnd())), index(0), container(NULL) //DObject None ? 
  {
    this->init(); //must be constructed to init  
  }

  DIterator(const DIterator& copy) : DCppMutable(copy), index(0), container(NULL)
  {
    this->init();
  }

  RealValue<DUInt64>          index; //signed en python
  RealValue<DObject*>         container; //setContainer pour update le type ?
  RealValue<DFunctionObject*> nextObject;
  RealValue<DFunctionObject*> firstObject;
  RealValue<DFunctionObject*> isDoneObject;
  RealValue<DFunctionObject*> currentItemObject;

  void                        next(void);
  void                        first(void);
  DInt8                       isDone(void);
  DValue                      currentItem(void);
  void                        setValue(size_t idx, DValue const& v);

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
       DAttribute(DType::DUnknownType, "currentItem",  DType::DNoneType),
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
