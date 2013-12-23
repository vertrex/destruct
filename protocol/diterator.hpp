#ifndef DESTRUCT_DITERATOR_HPP_
#define DESTRUCT_DITERATOR_HTPP

/*
 *  This is a protocol object iterator
 *  the protocol need im plem of some function 
 *  next(), first(), isDone()  
 */ 
#include "../dvalue.hpp"
#include "../dmemberpointer.hpp"
#include "../dmethodobject.hpp"

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

#define declareAttributeCount(count)\
  static size_t         ownAttributeCount()\
  {\
    return (count);\
  };\


#define objectFunctionList(f1, f2, f3, f4, f5, f6, f7)\
  static DMemoryPointer<DIterator>* memberBegin()\
  {\
    static DMemoryPointer<DIterator> memberPointer[] = \
          {\
                  DMemoryPointer<DIterator>(&DIterator::f1##Object),\
                  DMemoryPointer<DIterator>(&DIterator::f2##Object),\
                  DMemoryPointer<DIterator>(&DIterator::f3##Object),\
                  DMemoryPointer<DIterator>(&DIterator::f4##Object),\
                  DMemoryPointer<DIterator>(&DIterator::f5##Object),\
                  DMemoryPointer<DIterator>(&DIterator::f6##Object),\
                  DMemoryPointer<DIterator>(&DIterator::f7),\
          };\
          return memberPointer;\
  }\
  static DMemoryPointer<DIterator>* memberEnd()\
  { return (memberBegin() + ownAttributeCount()); }\

namespace Destruct
{

template <typename RealType, DType::Type_t  RealTypeId>
class DIterator  //template ? 
{
public:
// XXX XXX const DObject* mais faut coder const call a ce moment la !
  DIterator()
  {
   //XXX peut pas etre appeller par les autres constructure ?  ou faire une fonction init() pour pas copier 2 fois la meme chose
    this->index = 0;
    this->object = NULL;
    
    this->nextObject = new DMethodObject(this, &DIterator::next);
    this->firstObject = new DMethodObject(this, &DIterator::first); 
    this->isDoneObject = new DMethodObject(this, &DIterator::isDone); 
    this->currentItemObject = new DMethodObject(this, &DIterator::currentItem); 
    this->containerObject = new DMethodObject(this, &DIterator::container); 
    this->iteratorObject = new DMethodObject(this, &DIterator::iterator); 
  }

  DIterator(DObject* dobject)
  {
    this->index = 0;
    this->object = dobject;
    
    this->nextObject = new DMethodObject(this, &DIterator::next);
    this->firstObject = new DMethodObject(this, &DIterator::first); 
    this->isDoneObject = new DMethodObject(this, &DIterator::isDone); 
    this->currentItemObject = new DMethodObject(this, &DIterator::currentItem); 
    this->containerObject = new DMethodObject(this, &DIterator::container); 
    this->iteratorObject = new DMethodObject(this, &DIterator::iterator); 
  }


  DIterator(const DIterator& copy,DObject* object)
  {
    this->nextObject = new DMethodObject(this, &DIterator::next);
    this->firstObject = new DMethodObject(this, &DIterator::first); 
    this->isDoneObject = new DMethodObject(this, &DIterator::isDone); 
    this->currentItemObject = new DMethodObject(this, &DIterator::currentItem);
    this->containerObject = new DMethodObject(this, &DIterator::container); 
    this->iteratorObject = new DMethodObject(this, &DIterator::iterator); 
  }

  void      next()
  {
    this->index = this->index + 1;
  }

  void      first()
  {
    this->index = 0;
  }

  RealValue<DInt8>      isDone()
  {
    if (this->object)
    {
      DValue count = this->object->call("size",RealValue<DObject*>(DNone));
      if (this->index < count.get<DUInt64>())
        return 0;
    }
    return 1;
  }

  DValue                currentItem()
  {//XXX DVALUE DVALUE ? PROBLEM ICI DE PERFF ? 
    if (this->object)
    {
      return (this->object->call("get", RealValue<DUInt64>(this->index)));
    }
    return RealValue<DObject*>(DNone);
  } 

  RealValue<DObject*>   container(DValue const& value)
  {
    DObject*  container = value.get<DObject* >();

    if (container != DNone)
    {
      this->object = container;
    }
    return this->object;
  }


  RealValue<DObject*>   iterator(void)
  {
    return this->object;
  }

  //void  last();
  
  RealValue<DUInt64>    index; //signed en python
  DObject*              object;

  declareObject(next)
  declareObject(first)
  declareObject(isDone)
  declareObject(currentItem)
  declareObject(container)
  declareObject(iterator)

  attributeList(declareAttribute(DNone,"next", DNone),
  declareAttribute(DNone,"first", DNone),
  declareAttribute(DInt8,"isDone", DNone),
  DAttribute("currentItem",  DType::DMethodType, RealTypeId, DType::DNoneType),
  declareAttribute(DObject,"container", DObject),
  declareAttribute(DObject,"iterator", DNone),
  declareAttributeValue(DUInt64, "index"))

  declareAttributeCount(7)

  objectFunctionList(next, first, isDone, currentItem, container, iterator, index)
private:
};


}

#endif
