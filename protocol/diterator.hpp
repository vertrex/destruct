#ifndef DESTRUCT_DITERATOR_HPP
#define DESTRUCT_DITERATOR_HPP

/*
 *  This is a protocol object iterator
 *  the protocol need im plem of some function 
 *  next(), first(), isDone()  
 */ 
#include "../dvalue.hpp"
#include "../dcppobject.hpp"
#include "../dmemberpointer.hpp"
#include "../dmethodobject.hpp"
#include "dcontainer.hpp"

class DObject;

namespace Destruct
{

/*
    DObject
       ^
       |
       |        
 DCppObject     Dcontainer ? 
       ^         ^
       |         |
       |         |
    DConainerObject
  
    need access to this 
    need access to member func


 */



        //template< class DObjectType >
        //class DIterator : public DObjectType<DIterator> 
class DIterator : public DCppObject< DIterator > //C PU UNE TEMPLATE METTRE DS un .CPP  
{
public:
  RealValue<DFunctionObject*> nextObject;
  RealValue<DFunctionObject*> firstObject;
  RealValue<DFunctionObject*> isDoneObject;
  RealValue<DFunctionObject*> currentItemObject;
  RealValue<DFunctionObject*> contaienrObject;
  RealValue<DUInt64>          index; //signed en python
  RealValue<DObject*>         container; //setContainer pour update le field qu'il faut ? pour le type ?

  DIterator(DStruct* dstruct) : DCppObject(dstruct), index(0), container(NULL) //DObject None ? 
  {
    this->init(); //because object DIterator is not yet finished to create so can't point to func pointer ?
  }

  DIterator(const DIterator& copy) : DCppObject(copy), index(0), container(NULL)
  {
    this->init();
  }

  void  next(void)
  {
    this->index = this->index + 1;
  }

  void  first(void)
  {
    this->index = 0;
  }

  void setValue(size_t idx, DValue const& v)
  {
    if (idx == 0) 
    {
      DAttribute attr = this->instanceOf()->attribute(idx);
      //must change type accordingly to Container  
      std::cout << "setting container " << std::endl;
      //this->instanceOf()->attribute(idx)->type->modifyType("type")   

    }
    DCppObject<DIterator>::setValue(idx, v);
  }

  RealValue<DInt8>      isDone(void)
  {
    if (this->container) // !DNone ? 
    {
      DValue count;
      DContainer* dcontainer = dynamic_cast<DContainer*>((DObject*)this->container);
      if (dcontainer)
      {
        DFunctionObject* size = dcontainer->sizeObject;  
        count = size->call(RealValue<DObject*>(DNone));
      }
      else
        count = ((DObject*)this->container)->call("size", RealValue<DObject*>(DNone));

      if (this->index < count.get<DUInt64>())
        return (0);
    }

    return (1);
  }

  DValue currentItem(void)
  {
    if (this->container) // !DNone ?
    {
      DContainer* dcontainer = dynamic_cast<DContainer*>((DObject*)this->container);
      if (dcontainer)
      {
        DFunctionObject* get = dcontainer->getObject;
        return (get->call(RealValue<DUInt64>(this->index)));
      }
      else
        return (((DObject*)this->container)->call("get", RealValue<DUInt64>(this->index)));
    }
    throw DException("DIterator::currentItem container is not set.\n");
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
       DAttribute("container", DType::DObjectType),
       DAttribute("index", DType::DUInt64Type),
       DAttribute("next",   DType::DMethodType, DType::DNoneType, DType::DNoneType), 
       DAttribute("first",  DType::DMethodType, DType::DNoneType, DType::DNoneType),
       DAttribute("isDone", DType::DMethodType, DType::DInt8Type, DType::DNoneType),
       //DAttribute("currentItem",  DType::DMethodType, RealTypeId, DType::DNoneType), #XXX
       DAttribute("currentItem",  DType::DMethodType, DType::DUnicodeStringType, DType::DNoneType),
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
