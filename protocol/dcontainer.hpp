#ifndef DESTRUCT_DCONTAINER_HPP_
#define DESTRUCT_DCONTAINER_HPP_

#include "../dvalue.hpp"

namespace Destruct 
{

//class DContainer() ... method [] / size tous les trucs de base des containers ...
class IContainer
{
          //virtual ~IContainer() {};
  public:
  virtual RealValue<DObject*> iterator() = 0;
};


template <typename RealType>
class DContainer : public IContainer
{
public:
  DContainer()//DContainer::DContainer()
  {
          //this->iteratorObject = new DMethodObject(this, &DContainer< RealType >::iterator);
    this->iteratorObject = new DMethodObject(this, &DContainer< RealType >::iterator);
  }

  DContainer(const RealType& copy)
  {
    this->iteratorObject = new DMethodObject(this, &DContainer< RealType >::iterator);
  }


  RealValue<DFunctionObject* >  iteratorObject;

  RealValue<DObject*>    iterator(void)
  {
          std::cout << "DVector<STRING> ITERATOR FACTORY " << std::endl;
        //SEGFAULT SI PURE PYTOHON ??? car n herite pas de DVector<String> donc peut pas le convertir !

     DStruct* iteratorStruct = Destruct::instance().find("DIterator");  //mettre en temp pour pas chercher a chaque fois // optim
     DIterator* iterator = new DIterator();
     DStruct* selfStruct = Destruct::instance().find("DVector<String>");  //mettre en temp pour pas chercher a chaque fois // optim
     DClassObject<DIterator>* diterator = makeNewDObject<DIterator>(iteratorStruct, *iterator);
     DClassObject<RealType>* self = makeNewDObject< RealType >(selfStruct, *(RealType *)this);

     if (self)  //XXX si pure python on le convertie mais ca marchera pas !!! XXX 
     {
       iterator->first();
       iterator->setIterable(RealValue<DObject*>(self));

       return (diterator);
     }

     std::cout << "iterator factory can't cast do DVector<String> " << std::endl;
     return RealValue<DObject*>(DNone);
  }
};

template <typename RealType, DType::Type_t  RealTypeId>
class DVector : public DContainer<DVector<RealType, RealTypeId> > // Inherti container XXX et dcpp object comme ca peut return this, c un protocol donc un objet forcement
// mais pour d autre objet c pas focrcement le cas ct juste le cas avant  on cree un DCPPclass< dvector>
{
public:
  RealValue<DFunctionObject* >  pushObject;
  RealValue<DFunctionObject* >  getObject;
  RealValue<DFunctionObject* >  sizeObject;
 
  DVector()
  {
    this->pushObject = new DMethodObject(this, &DVector<RealType, RealTypeId>::push);
    this->getObject = new DMethodObject(this, &DVector<RealType, RealTypeId>::get);
    this->sizeObject = new DMethodObject(this, &DVector<RealType, RealTypeId>::size);
  };

  DVector(const DVector<RealType, RealTypeId>& copy) :  __vector(copy.__vector) 
  {
    this->pushObject = new DMethodObject(this, &DVector<RealType, RealTypeId>::push);
    this->getObject = new DMethodObject(this, &DVector<RealType, RealTypeId>::get);
    this->sizeObject = new DMethodObject(this, &DVector<RealType, RealTypeId>::size);
    //this->iteratorObject = new DMethodObject(this, &DContainer<DVector<RealType, RealTypeId> >::iterator);
  }

  RealValue<DInt32>  push(DValue const& args) // replace DValue par  RealValue<RealType  > possible ? quand c interchangeable et pourquoi?uniquement par copy a cause du operator conv() 
  {
    this->__vector.push_back(args.get<RealType>()); 
    return (this->__vector.size()- 1);
  }
   
  RealValue<RealType> get(DValue const& args)
  {
     //this->size > index ? 
     //RAISE EXCEPTION XXX -> python ca sera un DITerator !!!!!!!!!! si non ca va segfault logique
    DInt32 index = args.get<DInt32>();
    return (this->__vector[index]);
  }

  RealValue<DInt32>   size(void)
  {
    return this->__vector.size();
  }


  static size_t ownAttributeCount()
  {
    return (4);
  }
 
  static DAttribute* ownAttributeBegin()
  {

    static DAttribute  attributes[] = {
                       DAttribute("push", DType::DMethodType, DType::DInt32Type, RealTypeId), 
                       DAttribute("get",  DType::DMethodType, RealTypeId, DType::DInt32Type),
                       DAttribute("size", DType::DMethodType, DType::DInt32Type, DType::DNoneType),
                       ///XX HERITAGE HERITER ICI VOIR ANCIEN CODE AVEC HERITAGE DS LES EXEMPLES 
                       DAttribute("iterator", DType::DMethodType, DType::DObjectType, DType::DNoneType)
                                      };
    return (attributes);
  }

  static DAttribute* ownAttributeEnd()
  {
    return (ownAttributeBegin() + ownAttributeCount());
  }

  static DMemoryPointer<DVector<RealType, RealTypeId> >* memberBegin()
  {
    static DMemoryPointer<DVector<RealType, RealTypeId> > memberPointer[] = 
    {
      DMemoryPointer<DVector<RealType, RealTypeId> >(&DVector<RealType, RealTypeId>::pushObject),
      DMemoryPointer<DVector<RealType, RealTypeId> >(&DVector<RealType, RealTypeId>::getObject),
      DMemoryPointer<DVector<RealType, RealTypeId> >(&DVector<RealType, RealTypeId>::sizeObject),
      DMemoryPointer<DVector<RealType, RealTypeId> >(&DVector<RealType, RealTypeId>::iteratorObject),
    };
    return (memberPointer);
  }

  static DMemoryPointer<DVector<RealType, RealTypeId> >*  memberEnd()
  {
    return (memberBegin() + ownAttributeCount());
  } 
private:
  std::vector<RealType>   __vector;
};

}
#endif 
