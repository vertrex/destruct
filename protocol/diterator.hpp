#ifndef DESTRUCT_DITERATOR_HPP
#define DESTRUCT_DITERATOR_HPP

/*
 *  This is a protocol object iterator
 *  the protocol need im plem of some function 
 *  next(), first(), isDone()  
 */ 
#include "../dvalue.hpp"
#include "../dmemberpointer.hpp"
#include "../dmethodobject.hpp"
#include "dcontainerbase.hpp"

class DObject;

#define declareObject(name)\
  RealValue<DFunctionObject *> name##Object;

#define declareAttribute(returnType, name, argumentType)\
  DAttribute(name, DType::DMethodType, DType::returnType##Type, DType::argumentType##Type)

#define declareAttributeType(returnType, name, argumentType)\
  DAttribute(name, DType::DMethodType, returnType, DType::argumentType##Type)


#define declareAttributeValue(dtype, name)\
  DAttribute(name, DType::dtype##Type)

#define attributeList(a1, a2, a3, a4, a5, a6, a7)\
  static DAttribute*    ownAttributeBegin()\
  {\
     static DAttribute  attributes[] = \
     {\
       a1,\
       a2,\
       a3,\
       a4,\
       a5,\
       a6,\
       a7,\
     };\
     return (attributes);\
  }\
\
  static DAttribute*   ownAttributeEnd()\
  {\
    return (ownAttributeBegin() + ownAttributeCount());\
  }\

#define declareAttributeCount(count)\
  static size_t         ownAttributeCount()\
  {\
    return (count);\
  };\


namespace Destruct
{

class DIteratorBase
{
public:
  declareObject(next)
  declareObject(first)
  declareObject(isDone)
  declareObject(currentItem)
  declareObject(container)
  declareObject(iterator)
};

template <typename RealType, DType::Type_t  RealTypeId>
class DIterator : public DIteratorBase 
{
public:
  DIterator()
  {
   //XXX peut pas etre appeller par les autres constructure ?  ou faire une fonction init() pour pas copier 2 fois la meme chose
    this->index = 0;
    this->__container = NULL;
  }

  DIterator(DObject* dobject)
  {
    this->index = 0;
    this->__container = NULL;
  }


  DIterator(const DIterator& copy,DObject* object)
  {
  }

  void  next(void)
  {
    this->index = this->index + 1;
  }

  void  first(void)
  {
    this->index = 0;
  }

  RealValue<DInt8>      isDone(void)
  {
    if (this->__container)
    {
      DValue count;
      DContainerBase* dcontainer = dynamic_cast<DContainerBase*>(this->__container);
      if (dcontainer)
      {
        DFunctionObject* size = dcontainer->sizeObject;  
        count = size->call(RealValue<DObject*>(DNone));
      }
      else
        count = this->__container->call("size", RealValue<DObject*>(DNone));

      if (this->index < count.get<DUInt64>())
        return (0);
    }

    return (1);
  }

  DValue currentItem(void)
  {
    if (this->__container)
    {
      DContainerBase* dcontainer = dynamic_cast<DContainerBase*>(this->__container);
      if (dcontainer)
      {
        DFunctionObject* get = dcontainer->getObject;
        return (get->call(RealValue<DUInt64>(this->index)));
      }
      else
        return (this->__container->call("get", RealValue<DUInt64>(this->index)));
    }
    throw DException("DIterator::currentItem container is not set.\n");
  } 

  RealValue<DObject*>   container(DValue const& value)
  {
    DObject*  container = value.get<DObject* >();

    if (container != DNone)
      this->__container = container;

    return (this->__container);
  }

  RealValue<DObject*>   iterator(void)
  {
    return (this->__container);
  }

/*
 * DStruct declaration
 */

  RealValue<DUInt64>    index; //signed en python

  attributeList(declareAttribute(DNone,"next", DNone),
  declareAttribute(DNone,"first", DNone),
  declareAttribute(DInt8,"isDone", DNone),
  DAttribute("currentItem",  DType::DMethodType, RealTypeId, DType::DNoneType),
  declareAttribute(DObject,"container", DObject),
  declareAttribute(DObject,"iterator", DNone),
  declareAttributeValue(DUInt64, "index"))

  declareAttributeCount(7)

  static DMemoryPointer<DIterator>* memberBegin()
  {
    static DMemoryPointer<DIterator> memberPointer[] = 
    {
      DMemoryPointer<DIterator>(&DIterator::nextObject, &DIterator::next),
      DMemoryPointer<DIterator>(&DIterator::firstObject, &DIterator::first),
      DMemoryPointer<DIterator>(&DIterator::isDoneObject, &DIterator::isDone),
      DMemoryPointer<DIterator>(&DIterator::currentItemObject, &DIterator::currentItem),
      DMemoryPointer<DIterator>(&DIterator::containerObject, &DIterator::container),
      DMemoryPointer<DIterator>(&DIterator::iteratorObject, &DIterator::iterator),
      DMemoryPointer<DIterator>(&DIterator::index),
    };
    return memberPointer;
  }

  static DMemoryPointer<DIterator>* memberEnd()
  { 
    return (memberBegin() + ownAttributeCount()); 
  }

private:
  DObject*              __container;
};


}

#endif
