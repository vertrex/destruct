#ifndef DSTRUCT_SIMPLE_OBJECT_HPP_
#define DSTRUCT_SIMPLE_OBJECT_HPP_

#include "ddynamicobject.hpp"

#include "dstruct.hpp"

namespace Destruct
{

class DSimpleObject : public DDynamicObject
{
public:
  explicit DSimpleObject(DStruct *);
  DSimpleObject(DSimpleObject const &);

  static DObject*   newObject(DStruct *);
  virtual DObject*  clone() const;
};


//class DSequence
//{
// 
//
//}

/*
 *  SI DSequence herite de dobject -> on peu le remettre partout a la place de dobjet  
    donc faut tjrs test et on peu pas forcer une list ca peu devenir relou
    si on a plusieurs type d object de base c peut etre mieux mais du coup faut implem     
    la serialization pour tous c types (de toute faudra) et le rajouter ds les DType de base
 *
 /
////this is a sequence interface

//class DSequenceIType// public DObject ??? obliger pour pouvoir etre contenue ds DStruct ou alors c un type special ?
//{
  //DSequence();
  //virtual DSequence* clone() const = 0;
  //virtual size_t     size() const = 0;  
  //virtual void       append(size_t idx, DValue const& ) = 0; 
  //virtual void       push(size_t idx, DValue const& ) = 0;
  //virtual void       clean(void) = 0;
////type Name 
////  virtual void       next
//// virtual DIterator next() = 0;
//} 

//DValue -> object ou autre type -> ca c sur 


//dobject.mylist = -> donc ca doit etre aussi une dvalue ou une class
//si c un base type elle a pas toute les proprietes d un object
//donc pas interchangeable avec un dobject et plus fortement type

//si non c juste une description de class specigfique
//pas oublie que pour l instant les fonctions et le typage fort (des objects) 
//sont pas implemente

//donc possibilite d implem que en cpp object
//ou alors il faut implem les fonctions ! meme si pas de reel besoierver = manager.get_server()n pour la serialization 

//avatange d etre un basetype:


//inconveninent :




//avantage d etre un dobject

//incoveninent :
 

//ou 

//class DSequence : DCppObject
//{
  //size() member
  //append member() 
  //name() etc...

  //-> mais bon du coup y aura getValue/ Setvalue etc .. dont on pas besoin pour une list
//c pas la meme que la relation Class / Object -> bien que ca soit un object ?
//avec les methode append / push etc.. 

//private: 
//list 

//}

//if object->isType() == "dsequence" :
  //write size
  //for object in sequence:
     //write object




//class DList

//class DArray type / number

//class DBuffer : public DSimpleObject // Test pour NTFS
//{
//virtual []
//size();
// 
//
//}

*
class DContainerObject : public DSimpleObject //interface
{
  explit DContainerObject(DStruct const *);
  DContainerObject(DContainerObject const &);

  static DObject* newObject(DStruct const *);
//  virtual DObject* clone() const;
  virtual DObject* next() const; //iterator ?
  virtual DInt32   count() const;//  size
  virtual bool     push_back(DObject*) const;

//serialize ?   -> du coup ca deviens plus logique d avoir ca ici non ? si non ca doit etre implem ds dserialize....
// deserialzie ? -> du coup ca deviens plus logique d avoir ca ici
// par contre a pas besoin d interface d'attribut donc tres basic ....
};

*/
//implem
/*
class DList : public DContainerObject // le problem c la serialization un peu comme les strings donc faut savoir si c un base type et les avatange de faire un container object ? plusieurs implem ? regarder ds c/python  ou justement plusieurs dserialization ? les fontions serialize/dserialize sont overwritable ?? elle renvoie quoi ? ? possible en pyrhon ? DContainerObject(object).addtoot
{
   []
   next()  //iterator
   count()  /// sequence ! []
   push_back()
//iterator() return an iterator  Destruct::iterator<Dobject* >*  // iterator.next() 
  coyp() etc..
private:
  std_list() //DSTdList // DMultiThreadList // DLinkedList nect() prev() parent9) etc....

};


//ce qu il maque c le cast  comme en python ou le miens de savoir si l objet et bien un container
// u ndynamic_cast devrai suffir en theorie ! 
// PyIter_Check(object) en python ...

class DMap:: public DContainerObject
{
  next()
  []
  attribute [] -> toto->

key/value/items
}

DList.push_back(DRealValue<32>);
DList.push_back
*/

}
#endif
