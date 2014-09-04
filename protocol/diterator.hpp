#ifndef DESTRUCT_DITERATOR_HPP
#define DESTRUCT_DITERATOR_HPP

/*
 *  This is a protocol object iterator
 *  the protocol need im plem of some function 
 *  next(), first(), isDone()  
 */ 

#include "protocol/dcppmutable.hpp"

class DObject;
class DValue; 

namespace Destruct
{

class DIterator : public DCppMutable<DIterator>
{
public:
  DIterator(DValue const& args) : DCppMutable(new DMutableStruct(NULL, "DIterator", DIterator::newObject, DIterator::ownAttributeBegin(), DIterator::ownAttributeEnd()), args), index(0), container(NULL) //DObject None ? 
  {
    this->init(); //must be constructed to init 

    DObject* ocontainer = args.get<DObject*>(); //ref added dion;t destriy ut
    this->container = ocontainer; //ref already added
    DAttribute attr = ocontainer->instanceOf()->attribute("get"); //
    this->__struct->replaceAttribute(5, DAttribute(attr.type().getReturnType(), "currentItem", DType::DNoneType));
    this->index = 0;
  }

  DIterator(const DIterator& copy) : DCppMutable(copy), index(0), container(NULL)
  {
    this->init();
  }

  ~DIterator()
  {
  }

  RealValue<DUInt64>          index;
  RealValue<DObject*>         container;
  RealValue<DFunctionObject*> _next;
  RealValue<DFunctionObject*> _first;
  RealValue<DFunctionObject*> _isDone;
  RealValue<DFunctionObject*> _currentItem;

  void                        next(void);
  void                        first(void);
  DInt8                       isDone(void);
  DValue                      currentItem(void);

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

  static DPointer<DIterator>* memberBegin()
  {
    static DPointer<DIterator> memberPointer[] = 
    {
      DPointer<DIterator>(&DIterator::container),
      DPointer<DIterator>(&DIterator::index),
      DPointer<DIterator>(&DIterator::_next, &DIterator::next),
      DPointer<DIterator>(&DIterator::_first, &DIterator::first),
      DPointer<DIterator>(&DIterator::_isDone, &DIterator::isDone),
      DPointer<DIterator>(&DIterator::_currentItem, &DIterator::currentItem),
    };
    return memberPointer;
  }

  static DAttribute*   ownAttributeEnd()
  {
    return (ownAttributeBegin() + ownAttributeCount());
  }

  static DPointer<DIterator>* memberEnd()
  { 
    return (memberBegin() + ownAttributeCount()); 
  }
};


}

#endif
