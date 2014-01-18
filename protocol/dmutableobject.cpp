#include "dmutablestruct.hpp"
#include "dmutableobject.hpp"

#include "../dstruct.hpp"
#include "../dnullobject.hpp"
#include "../dexception.hpp"
#include "../drealvalue.hpp"

namespace Destruct
{

DMutableObject::DMutableObject(DMutableStruct* dstructDef) : DDynamicObject(dynamic_cast<DStruct* >(dstructDef))
{
  this->init(this);
}

DMutableObject::DMutableObject(DMutableObject const & rhs) : DDynamicObject(rhs)
{
  this->copy(this, rhs);
}

DObject* DMutableObject::newObject(DMutableStruct* myClass)
{
  return (new DMutableObject(myClass));
}

DObject* DMutableObject::clone() const
{
 return (new DMutableObject(*this));
}

DValue DMutableObject::getValue(size_t idx) const
{
  std::cout <<"get value " << std::endl;
  if (idx > this->__values.size())
  {
    std::cout << "throw value doenst exist python must catch it ! and ret error " << std::endl;   
      throw Destruct::DException("Value doesn't exist.");
         
  }
  return (this->__values[idx]->getFinal());

  //XXX DValue devrait etre set sur DNone par default car si non renvoie un DVlaue non init et segfault 
  return Destruct::RealValue<Destruct::DObject*>(Destruct::DNone);
}

void DMutableObject::setValue(size_t idx, DValue const & v)
{
 std::cout << "Set value " << std::endl;
//creer une fonction newValue ? qui cree la value pour etr reutiliser apres ?
  if (idx >= this->__values.size())
  {
    if (idx < this->instanceOf()->attributeCount())
    {
       this->__values[idx] = this->instanceOf()->attribute(idx).type().newValue();
        //XXX c le bon truc a faire a reutiliser aileurs ou je fait n imp genre un new DValue
       //*i = a->type().newValue();
       this->__values[idx]->set(v);
       std::cout << "new value setted" << std::endl;
       return ;
    }
  }
  
  this->__values[idx]->set(v);
}

DValue DMutableObject::call(size_t idx, DValue const& v)
{
 std::cout << "call " << std::endl;
        //if (idx > this->__values.size())
        //this->__values[idx] = new 

        //return (this->__values[idx]->getFinal().get<DFunctionObject *>()->call(v));
  return Destruct::RealValue<Destruct::DObject*>(Destruct::DNone);
}


}
