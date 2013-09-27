#include "destruct.hpp"

namespace Destruct
{

Destruct::Destruct()
{
}

Destruct::~Destruct()
{
//XXX FIX BECAUSE SOME struct are registred twice like the CPP one so it segfault (test avec siter.py) 
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
  DStruct* dstruct = this->find(name);

  if (dstruct == NULL)
    return (NULL);

  return (dstruct->newObject());
} 

}
