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
  DAttribute attribute(name, type);
//add attribute        item  DTypeString
  this->instanceOf()->addAttribute(attribute);
  //DValue(RealValue<std::string>()) 
  //this->__prototype[typeid]->clone();// !!a utiliser plus souvet non ? on devrait tjrs reutiliser clone et newObject /newValue relerie comment marche ce pattern et l utiliser ! si non ca sert a rien ! -> check partout et ecrire pourquoi ds un guide dev a faire de toute !

                        // push back prend un BasevValue * // meme truc que PythonBaseValue ? 
                        
                //je cree un DValue( realvalue<std::string> ) ? 

//  this->__values.push_back(DValue(v));
  this->__values.push_back(attribute.type().newValue()); //XXX set new value ou directement v possible ?
//set la value apres (DValue ds un autre DValue de pas le meme type / j ai deja eu ce pb je crois et je fainte je c pu ou comme pour replace 
  this->__values.back()->set(v);
}

}
