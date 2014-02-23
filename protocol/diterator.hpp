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
class DValue; 

namespace Destruct
{

class DIterator : public DCppMutable<DIterator>
{
public:
  DIterator(DValue const& args) : DCppMutable(new DMutableStruct(NULL, "DIterator", DIterator::newObject, DIterator::ownAttributeBegin(), DIterator::ownAttributeEnd()), args), index(0), container(NULL) //DObject None ? 
  {
    this->init(); //must be constructed to init 

    DObject* ocontainer = args.get<DObject*>();

    this->container = ocontainer;
    DAttribute attr = ocontainer->instanceOf()->attribute("get"); //
    this->__struct->replaceAttribute(5, DAttribute(attr.type().getReturnType(), "currentItem", DType::DNoneType));
    this->index = 0;
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
      DPointer<DIterator>(&DIterator::nextObject, &DIterator::next),
      DPointer<DIterator>(&DIterator::firstObject, &DIterator::first),
      DPointer<DIterator>(&DIterator::isDoneObject, &DIterator::isDone),
      DPointer<DIterator>(&DIterator::currentItemObject, &DIterator::currentItem),
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
