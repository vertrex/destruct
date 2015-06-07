#ifndef DESTRUCT_DITERATOR_HPP
#define DESTRUCT_DITERATOR_HPP

/*
 *  This is a protocol object iterator
 *  the protocol need im plem of some function 
 *  next(), first(), isDone()  
 */ 

//#include "protocol/dcppmutable.hpp"
#include "protocol/dcppobject.hpp"
#include "protocol/dcontainer.hpp"

class DObject;
class DValue; 

namespace Destruct
{

template <typename ValueType, DType::Type_t ValueTypeId >
class DIterator : public  DCppObject<DIterator<ValueType, ValueTypeId> >
{
public:
  DIterator(DStruct* dstruct, DValue const& args) : DCppObject<DIterator<ValueType, ValueTypeId> >(dstruct, args), index(0), container(args.get<DObject*>())
  {
    this->init();
  }

  DIterator(const DIterator& copy) : DCppObject<DIterator<ValueType, ValueTypeId > >(copy), index(copy.index), container(copy.container)
  {
    this->init();
  }

  ~DIterator()
  {
    ((DObject*)container)->destroy();
  }

  void  next(void)
  {
    this->index = this->index + 1;
  }

  void  first(void)
  {
    this->index = 0;
  }

  DValue        isDone(void)
  {
    if (this->container)
    {
      DValue count;
      DContainer* dcontainer = dynamic_cast<DContainer*>((DObject*)this->container);
      if (dcontainer)
      {
        DFunctionObject* size = dcontainer->_size;  
        count = size->call(RealValue<DObject*>(DNone));
      }
      else
        count = ((DObject*)this->container)->call("size");

      if (this->index < count.get<DUInt64>())
        return (RealValue<DInt8>(0));
    }
    else
      throw DException("DIterator::isDone have no container to iterate on.");
    return (RealValue<DInt8>(1));
  }

  DValue currentItem(void)
  {
    if (this->container)
    {
      DContainer* dcontainer = dynamic_cast<DContainer*>((DObject*)this->container);
      if (dcontainer)
      {
        DFunctionObject* get = dcontainer->_get;
        return (get->call(RealValue<DUInt64>(this->index)));
      }
      else
        return (((DObject*)this->container)->call("get", RealValue<DUInt64>(this->index)));
    }
    throw DException("DIterator::currentItem container is not set.\n");
  }

  RealValue<DUInt64>          index;
  RealValue<DObject*>         container;
  RealValue<DFunctionObject*> _next;
  RealValue<DFunctionObject*> _first;
  RealValue<DFunctionObject*> _isDone;
  RealValue<DFunctionObject*> _currentItem;


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
