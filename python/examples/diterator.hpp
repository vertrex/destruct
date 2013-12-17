#ifndef DESTRUCT_EXAMPLES_DITERATOR
#define DESTRUCT_EXAMPLES_DITERATOR

#include <iostream>

#include "destruct.hpp"
#include "dmemberpointer.hpp"
#include "drealvalue.hpp"
#include "dcppobject.hpp"
#include "dmethodobject.hpp"

namespace Destruct
{

template <typename RealType, DType::Type_t  RealTypeId>
class DVector
{
public:
  RealValue<DFunctionObject* >  pushObject;
  RealValue<DFunctionObject* >  getObject;
  RealValue<DInt32>             count;
  RealValue<DFunctionObject* >  returnVoidObject;
  RealValue<DFunctionObject* >  allVoidObject;
  RealValue<DFunctionObject* >  callVoidObject;
  RealValue<DFunctionObject* >  returnObjectObject;
  RealValue<DFunctionObject* >  callObjectObject;
 
  DInt32 toto;

  DVector()
  {
    this->count = 0;
    this->pushObject = new DMethodObject(this, &DVector<RealType, RealTypeId>::push);
    this->getObject = new DMethodObject(this, &DVector<RealType, RealTypeId>::get);

//XXX void
    this->callVoidObject = new DMethodObject(this, &DVector<RealType, RealTypeId>::callVoid);
    this->returnVoidObject = new DMethodObject(this, &DVector<RealType, RealTypeId>::returnVoid);
    this->allVoidObject = new DMethodObject(this, &DVector<RealType, RealTypeId>::allVoid);

    this->returnObjectObject = new DMethodObject(this, &DVector<RealType, RealTypeId>::returnObject);
    this->callObjectObject = new DMethodObject(this, &DVector<RealType, RealTypeId>::callDObject);
  };

  /*
   *  Copy constructor is VERY important if you want to create a DClassObject because it's passed by const ref (copy)
   */ 
                                                                                        ///XXX ca va pas !!! peut pas copyer si non ca pointe tjrs sur le this de l  autre object ce ki es un peu space donc peut etre passer this en argument c quand meme mieux pour pas que ca soit le gros bordel :) XXX
  DVector(const DVector<RealType, RealTypeId>& copy) : count(copy.count), __vector(copy.__vector) 
  {
        ///XXX on peu pas le passer par copy si non ca garde l ancien this voir avec python par ex on change count
        //et il continue avec l ancioenne value car le this->count et bioen celui la mais 
        //  this->function(this) et call avec l ancien this ! donc passe this en argument plutot ???? XXX
    this->pushObject = new DMethodObject(this, &DVector<RealType, RealTypeId>::push);
    this->getObject = new DMethodObject(this, &DVector<RealType, RealTypeId>::get);
//XXX void

    this->callVoidObject = new DMethodObject(this, &DVector<RealType, RealTypeId>::callVoid);
    this->returnVoidObject = new DMethodObject(this, &DVector<RealType, RealTypeId>::returnVoid);
    this->allVoidObject = new DMethodObject(this, &DVector<RealType, RealTypeId>::allVoid);

    this->returnObjectObject = new DMethodObject(this, &DVector<RealType, RealTypeId>::returnObject);
    this->callObjectObject = new DMethodObject(this, &DVector<RealType, RealTypeId>::callDObject);
  }

  RealValue<DInt32>  push(DValue const& args) // replace DValue par  RealValue<RealType  > possible ? quand c interchangeable et pourquoi?uniquement par copy a cause du operator conv() 
  {
    this->__vector.push_back(args.get<RealType>()); 
    this->count = this->count + 1; //FIXED les constructeur par copy tous ca // ca increnmente pas le bon ????????????????? en python probleme avec les this tous ca ?
    return (this->count - 1); //current elem
  }
   
  RealValue<RealType> get(DValue const& args) //get( std::String) ca marche en core cette fin a 2 balle ? 
  {
          //DUInt32 i =  args.get<DUInt32>();
    //if (i < i.count)
    return (this->__vector[args.get<DInt32>()]);
    //throw ? comment on faiiiiiiiiiit faut un protocoooool d erreur voyez
    // return NONE; 
    //return DVOID;
  }

  void  returnVoid(DValue const& args)
  {
          //std::cout << "void returnVoid(args) called" << std::endl;
  }

  void allVoid(void)
  {
          //std::cout << "void allVoid(void) called" << std::endl;
  }

    //DValue possible here ?
  RealValue<DInt32> callVoid(void)
  {
          //std::cout << "int callVoid(void) called" << std::endl;
    return (1);
  }

  RealValue<DObject* > returnObject(void)
  {
     return DNone;
  }

  RealValue<DObject *> callDObject(DValue const& args)
  {
     DObject* obj =  args.get<DObject *>();
     if (obj == DNone)
     std::cout << "virer objet sert a rien pdty omcpil" << std::endl;
     return DNone;
  }


  static size_t ownAttributeCount() //endure c relou de changer ici et ailleurs ..
  {
    return (8);
  }
 
  static DAttribute* ownAttributeBegin()
  {

    static DAttribute  attributes[] = {
                                //XXX une method peut pas rtourner une method dommage -> factory tous ca ...
                                        DAttribute("push", DType::DMethodType, DType::DInt32Type, RealTypeId), //template :( pas possible
                                        DAttribute("get",  DType::DMethodType, RealTypeId, DType::DInt32Type),
                                        DAttribute("count",DType::DInt32Type), // XXX void existe pas :) /dfunction / dmethod :) ...
                                                                //DType::DVoidMethodType ?      DVoid           DVoid
                                                                //XXX void void !
                                        DAttribute("callVoid", DType::DMethodType, DType::DInt32Type, DType::DNoneType),
                                        DAttribute("returnVoid", DType::DMethodType, DType::DNoneType, DType::DInt32Type),
                                        DAttribute("allVoid", DType::DMethodType, DType::DNoneType, DType::DNoneType),
                                                  
                                        
                                        DAttribute("returnObject", DType::DMethodType, DType::DObjectType, DType::DNoneType),
                                        DAttribute("callObject", DType::DMethodType, DType::DObjectType, DType::DObjectType),
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
      DMemoryPointer<DVector<RealType, RealTypeId> >(&DVector<RealType, RealTypeId>::count),
//SEGFAULT SI PAS DEFINIE mais definie en attribute -> donc le coup des virtuelle :) 
// check if == 0 == virtuel car  non definie ... etc...
      DMemoryPointer<DVector<RealType, RealTypeId> >(&DVector<RealType, RealTypeId>::callVoidObject),
      DMemoryPointer<DVector<RealType, RealTypeId> >(&DVector<RealType, RealTypeId>::returnVoidObject),
      DMemoryPointer<DVector<RealType, RealTypeId> >(&DVector<RealType, RealTypeId>::allVoidObject),



      DMemoryPointer<DVector<RealType, RealTypeId> >(&DVector<RealType, RealTypeId>::returnObjectObject),
      DMemoryPointer<DVector<RealType, RealTypeId> >(&DVector<RealType, RealTypeId>::callObjectObject),
        //Dmemory pointer qui pointe vers un call directement avec un constructeur ds DMemoryPointeur qui fait un DMethodBase a la place ?
      //this->allVoidObject = new DMethodObject(this, &DVector<RealType, RealTypeId>::allVoid);
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
