#include "diterator.hpp"

namespace Destruct
{

void  DIterator::next(void)
{
  this->index = this->index + 1;

//this->iterator++; 
//deja ca va pas :) il devrait faire un iterator++ 
// en template iteator c++ mais du coup faut retemplater iterator :( c pu vriament un protocol 

}

void  DIterator::first(void)
{
//this->iterator = iterator.begin();
  this->index = 0;
}

void DIterator::setValue(size_t idx, DValue const& v)
{
  if (idx == 0)
  {
    DObject* container = v.get<DObject*>();
    DAttribute attr = container->instanceOf()->attribute("get"); //
    this->__struct->replaceAttribute(5, DAttribute(attr.type().getReturnType(), "currentItem", DType::DNoneType));
    DCppMutable<DIterator>::setValue(idx, v);
    this->index = 0;
  }
  else
    DCppMutable<DIterator>::setValue(idx, v);
}

DInt8        DIterator::isDone(void)
{
//if it != it.end();
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

DValue DIterator::currentItem(void)
{
//return *it
  if (this->container) // !DNone ?
  {
    DContainer* dcontainer = dynamic_cast<DContainer*>((DObject*)this->container);
    if (dcontainer)
    {
      DFunctionObject* get = dcontainer->getObject;
      return (get->call(RealValue<DUInt64>(this->index))); //XXX MAP doit envoyer ue clef pas un index il faut un iterator std::map :( 
    }
    else
      return (((DObject*)this->container)->call("get", RealValue<DUInt64>(this->index)));
  }
  throw DException("DIterator::currentItem container is not set.\n");
}

} 
