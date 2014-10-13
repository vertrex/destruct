#include "diterator.hpp"
#include "protocol/dcontainer.hpp"

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

DInt8        DIterator::isDone(void)
{
  if (this->container) // !DNone ? 
  {
    DValue count;
    DContainer* dcontainer = dynamic_cast<DContainer*>((DObject*)this->container); //XXX optim faire une fosi au debut ? 
    if (dcontainer)
    {
      DFunctionObject* size = dcontainer->_size;  
      count = size->call(RealValue<DObject*>(DNone));
    }
    else
      count = ((DObject*)this->container)->call("size", RealValue<DObject*>(DNone));

    if (this->index < count.get<DUInt64>())
      return (0);
  }
  else
    throw DException("DIterator::isDone have no container to iterate on.");

  return (1);
}

DValue DIterator::currentItem(void)
{
  if (this->container) // !DNone ?
  {
    DContainer* dcontainer = dynamic_cast<DContainer*>((DObject*)this->container);
    if (dcontainer)
    {
      DFunctionObject* get = dcontainer->_get;
      return (get->call(RealValue<DUInt64>(this->index)));
    }
    else
      return (((DObject*)this->container)->call("get", RealValue<DUInt64>(this->index)));
  }
  throw DException("DIterator::currentItem container is not set.\n");
}

} 
