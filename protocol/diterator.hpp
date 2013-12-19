#ifndef DESTRUCT_DITERATOR_HPP_
#define DESTRUCT_DITERATOR_HTPP

/*
 *  This is a protocol object iterator
 *  the protocol need im plem of some function 
 *  next(), first(), isDone()  
 */ 

#include "../dmemberpointer.hpp"
#include "../dmethodobject.hpp"
class DObject;

#define declareObject(name)\
  RealValue<DFunctionObject *> name##Object;

#define declareAttribute(returnType, name, argumentType)\
  DAttribute(name, DType::DMethodType, DType::returnType##Type, DType::argumentType##Type)

#define declareAttributeValue(dtype, name)\
  DAttribute(name, DType::dtype##Type)


#define attributeList(a1, a2, a3, a4, a5, a6, a7, a8)\
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
       a8,\
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


#define objectFunctionList(f1, f2, f3, f4, f5, f6, f7, f8)\
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
                  DMemoryPointer<DIterator>(&DIterator::f7##Object),\
                  DMemoryPointer<DIterator>(&DIterator::f8),\
          };\
          return memberPointer;\
  }\
  static DMemoryPointer<DIterator>* memberEnd()\
  { return (memberBegin() + ownAttributeCount()); }\

namespace Destruct
{
//?XXX cas special des protocol herite directement de DObject pour moins se faire chier ? 
// et pas avoir a cree un object avec make new etc.. ?
class DIterator  //template ? 
{
public:
// XXX XXX const DObject* mais faut coder const call a ce moment la !
  DIterator();
  DIterator(DObject* dobject); 
  DIterator(const DIterator& copy,DObject* object);//DIterableObject ? DIterable force avoir certains method (dcpp object) comme ca rapide en cpp ? pas besoin d utiliser call si c en python la method python sera binder sur l objet a la place de la method dynamique cpp
  void      next();
  void      first();
  RealValue<DInt8>      isDone();
  DValue                currentItem();
  void                  setContainer(DValue const& value);
  RealValue<DObject*>   container(void);
  RealValue<DObject*>   iterator(void);
  //void  last();
  
  RealValue<DUInt64>    index; //signed peut etre utilise ou pas ? peut etre neg ?
  DObject*              object;

//DESTRUCT MOP DECLARATIONA SIMPLIFIER !!!!!!!!

  declareObject(next)
  declareObject(first)
  declareObject(isDone)
  declareObject(currentItem)
  declareObject(setContainer)
  declareObject(container)
  declareObject(iterator)
  //declareValue(index)

  attributeList(declareAttribute(DNone,"next", DNone),
  declareAttribute(DNone,"first", DNone),
  declareAttribute(DInt8,"isDone", DNone),
  declareAttribute(DUnicodeString, "currentItem", DNone),
  declareAttribute(DNone,"setContainer", DObject),
  declareAttribute(DObject,"container", DNone), //valueDirect () ?
  declareAttribute(DObject,"iterator", DNone),
  declareAttributeValue(DUInt64, "index"))


  declareAttributeCount(8)

  objectFunctionList(next, first, isDone, currentItem, setContainer, container, iterator, index)
private:
};
 //Destruct* toto = Destruct::instance();//->registerDStruct(dstructvectors);


}

#endif
