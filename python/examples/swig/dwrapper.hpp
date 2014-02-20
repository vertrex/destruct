#ifndef DWRAPPER_HPP
#define DWRAPPER_HPP

#include "dvalue.hpp"

namespace Destruct
{
  class DValue;
}

class DWrapper
{
public:
                       DWrapper();
                       ~DWrapper();
//possibilite de renvoyer un RealValue<DObject*> pour pas avoir a convertir ? voir dcppobject protocol ...
  Destruct::DObject*   getObject();
  void                 setObject(Destruct::DObject* object);
//
  Destruct::DValue     getValue();
  void                 setValue(Destruct::DValue value);

  void                 run(Destruct::DObject* module, Destruct::DObject* arguments);
                                        //swig template et python template ? 
                                        //RealValue<type>() au lieu de DValue pour eviter les conversions et reconversin ?>
  void                 runValueObject(Destruct::DValue module, Destruct::DValue arguments);
  void                 runValueMethod(Destruct::DValue module, Destruct::DValue arguments);
  //void                 runValueMethod(Destruct::DValue* module, Destruct::DValue* arguments);
  void                 serialize(Destruct::DObject* object);
private:
  Destruct::DObject*   __object;
  Destruct::DValue    __value;
};

//faire une class de conversion avec l operator() 

//class FakeNode:
//{
//
//}

#endif
