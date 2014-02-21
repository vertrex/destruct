#include "diterator.hpp"

namespace Destruct
{

void  DIterator::next(void)
{
  this->index = this->index + 1;
}

void  DIterator::first(void)
{
  this->index = 0;
}

void DIterator::setValue(size_t idx, DValue const& v)
{
  std::cout << "SetValue DIterator CALLLLLLLLLEEEED" << std::endl;
  if (idx == 0)
  {
    std::cout << "changing type for iterator " << std::endl;
    DObject* container = v.get<DObject*>();
    DAttribute attr = container->instanceOf()->attribute("get"); //
    this->__struct->replaceAttribute(5, DAttribute(attr.type().getReturnType(), "currentItem", DType::DNoneType));
  }
  DCppObject<DIterator>::setValue(idx, v);
}

DInt8        DIterator::isDone(void)
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

DValue DIterator::currentItem(void)
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

} 
