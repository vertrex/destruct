#include "destruct.hpp"
#include "dexception.hpp"
#include "protocol/protocol.hpp"
#include "drealvalue.hpp"
#include "dnullobject.hpp"

#include <iostream>

namespace Destruct
{

Destruct::Destruct()
{
   Protocol(this);
}

Destruct::~Destruct()
{
//XXX FIX BECAUSE SOME struct are registred twice 
 //for (size_t idx = 0; idx != this->__structures.size(); ++idx)
   //delete this->__structures[idx];
}

Destruct&     Destruct::instance()
{
  static Destruct       destruct;
  return (destruct);
}

void Destruct::registerDStruct(DStruct* dstructDef)
{
  this->__structures.push_back(dstructDef);
}

bool Destruct::unregister(DStruct* dstructDef)
{
  for (Iterator c = this->__structures.begin(); c != this->__structures.end(); ++c)
  {
    if (*c == dstructDef)
    {
      this->__structures.erase(c);
      *c = NULL;
      return true;
    }
  }

  return false;
}

size_t  Destruct::count(void)
{
  return (this->__structures.size());
}

DStruct*       Destruct::find(DUnicodeString const & name) 
{
  for (Iterator c = this->__structures.begin(); c != this->__structures.end(); ++c)
  {
    if ((*c)->name() == name)
    {
      return (*c);
    }
  }

  return (NULL);
}

DStruct*        Destruct::find(const size_t index)
{
  if (index < this->__structures.size())
    return (this->__structures[index]);
  return (NULL);
}

DObject *       Destruct::generate(DUnicodeString const& name)
{
  return (this->generate(name, RealValue<DObject*>(DNone)));
} 

DObject *       Destruct::generate(DUnicodeString const& name, DValue const& args)
{
  DStruct* dstruct = this->find(name);
  if (dstruct == NULL) //return NULL ? 
  {
    std::string error = "Can't find DStruct : " + name;
    throw DException(error);
  }

  return (dstruct->newObject(args));
} 
}
