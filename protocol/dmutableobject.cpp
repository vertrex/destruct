#include "dmutablestruct.hpp"
#include "dmutableobject.hpp"

#include "../dstruct.hpp"
#include "../dnullobject.hpp"
#include "../dexception.hpp"
#include "../drealvalue.hpp"

namespace Destruct
{

DMutableObject::DMutableObject(const DUnicodeString& name) : DDynamicObject(dynamic_cast<DStruct* >(new DMutableStruct(NULL, name, DMutableObject::newObject)))
{
  this->init(this);
}

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
  if (idx >= this->__values.size())
  {
    DStruct* dstruct = this->instanceOf();
    for (size_t i = this->__values.size() ; i < dstruct->attributeCount(); i++)
        this->__values.push_back(dstruct->attribute(i).type().newValue());
  }
  this->__values[idx]->set(v);
}

DValue DMutableObject::call(size_t idx, DValue const& v)
{
   if (idx > this->__values.size())
   {
     std::cout << "throw value doenst exist python must catch it ! and ret error " << std::endl;   
     throw Destruct::DException("Value doesn't exist.");
   }
 
  return (this->__values[idx]->getFinal().get<DFunctionObject *>()->call(v));
}

void DMutableObject::setValueAttribute(std::string const& name, DValue const& v, DType::Type_t type)
{
  std::cout << "SET VALUE ATTRIBUTE" << std::endl;
  DAttribute attribute(name, type);
  this->instanceOf()->addAttribute(attribute);
  this->__values.push_back(attribute.type().newValue()); //set new value ou directement v possible ?
  this->__values.back()->set(v);

}

}
