#include "diterator.hpp"

#include "../destruct.hpp"

namespace Destruct
{
//xxx makeNew  ds protocol.cpp ? qui init les protocol ? 
// ca nous fait une base fix et pas besoin d impleme en python a part peut re ce as specia ld iterator ? 
// et encore .. car au final on renvoie un dobject et on peu le creer et l heriter en python

//THIS USE A PROTOCOL !


//DStruct* diteratorStruct = makeNewDClass < DIterator >(NULL, "DIterator");

DIterator::DIterator()
{
     this->index = 0;
     this->object = NULL;
    
     this->nextObject = new DMethodObject(this, &DIterator::next);
     this->firstObject = new DMethodObject(this, &DIterator::first); 
     this->isDoneObject = new DMethodObject(this, &DIterator::isDone); 
     this->currentItemObject = new DMethodObject(this, &DIterator::currentItem); 
     this->setIterableObject = new DMethodObject(this, &DIterator::setIterable); 
}

DIterator::DIterator(DObject* dobject) 
{
     this->index = 0;
     this->object = dobject;
    
     this->nextObject = new DMethodObject(this, &DIterator::next);
     this->firstObject = new DMethodObject(this, &DIterator::first); 
     this->isDoneObject = new DMethodObject(this, &DIterator::isDone); 
     this->currentItemObject = new DMethodObject(this, &DIterator::currentItem); 
     this->setIterableObject = new DMethodObject(this, &DIterator::setIterable); 
}

DIterator::DIterator(const DIterator& copy, DObject* object) : index(copy.index), object(copy.object)
{
  //copy.object->addRef();
  this->nextObject = new DMethodObject(this, &DIterator::next);
  this->firstObject = new DMethodObject(this, &DIterator::first); 
  this->isDoneObject = new DMethodObject(this, &DIterator::isDone); 
  this->currentItemObject = new DMethodObject(this, &DIterator::currentItem);
  this->setIterableObject = new DMethodObject(this, &DIterator::setIterable); 
}

//void    DIterator::setIterable(RealValue<DObject*> value)
void    DIterator::setIterable(DValue const& value)
{
  //std::cout << "DIterator::setIterable(" << object->instanceOf()->name() << ")" << std::endl;
  this->object = value.get<DObject*>();
}

void    DIterator::next(void)
{
  //std::cout << "DIterator::next " << std::endl;
  this->index++;
}

void    DIterator::first(void)
{
  //std::cout << "DIterator::first" << std::endl;
  this->index = 0;
}

RealValue<DInt8> DIterator::isDone(void)
{
  //std::cout << "DIterator::isDone" << std::endl;
  if (this->object)
  {
    DValue count = this->object->call("size",RealValue<DObject*>(DNone));
    //plus lent que par getValue donc a choisir ! 
    if (this->index < count.get<DInt32>())
      return 0;
  }
  return 1;
}

//DValue ? plus lent ... mais plus pratique ? car peut forcer une conversion en c++ ? 

DValue DIterator::currentItem(void)
{
  //std::cout << "DIterator::currentItem" << std::endl;

  //XXX posbilite d\optime c++ ;
  // DIterable = dynamic_cast<Protocol::DIterable* >();
  // if DIterable
  //  return DItreable.get(this->index)

  if (this->object)
  {
   //std::cout << "DIterator::currentItem call this->object get" << std::endl;
    return RealValue<DUnicodeString>(this->object->call("get", RealValue<DInt32>(this->index)).get<DUnicodeString>()); 
  }//return this->object->call("get", RealValue<DInt32>(this->index)).get<DUnicodeString>(); 
  //std::cout << "DIterator::currenItem Object is not set can't call get method !" << std::endl; 

  return RealValue<DObject*>(DNone);
}

}
/*  CPP 
    DIterator new iterator(object);
    for (iterator->first(); !iterator->isDone(); iterator->next)
      DObject* iterator->currentItem()
       std::cout << object " << std::endl;
 
    autre implem 

    DIterator iterator new iterator(object);
    while (object = iterator->next())
    {
       std::cout << object << std::endl;
    }

    autre implem

    method_show(DObject*)
       std::cout << show())
    
    object::iterator()
    {
                        //prend une method d'iteration en entree
        return DIterator(this, this->methodShow)
    }    

    DIterator = object->iterator()
    while (object = iterator->next()); 



 */
