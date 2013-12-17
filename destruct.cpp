#include "destruct.hpp"
#include "dexception.hpp"
#include <iostream>

namespace Destruct
{

Destruct::Destruct()
{
        //std::cout << "Destruct init " << std::endl;
}

Destruct::~Destruct()
{
        //std::cout << "destruct delete" << std::endl;
//XXX FIX BECAUSE SOME struct are registred twice like the CPP one so it segfault (test avec siter.py) 
        //for (size_t idx = 0; idx != this->__structures.size(); ++idx)
        //delete this->__structures[idx];
}

Destruct&     Destruct::instance()
{
  static Destruct       destruct;
  //std::cout << "destruct.instance() " << std::endl;
  return (destruct);
}

void Destruct::registerDStruct(DStruct* dstructDef)
{
  this->__structures.push_back(dstructDef);
}

bool Destruct::unregister(DStruct* dstructDef)
{
        //std::cout << "find pointer" << std::endl;
  for (Iterator c = this->__structures.begin(); c != this->__structures.end(); ++c)
  {
          //std::cout << "for " << *c << std::endl;
    if (*c == dstructDef)
    {
            //std::cout << "returning " << *c << std::endl;
      
      this->__structures.erase(c);
      *c = NULL;
      return true;
    }
    //std::cout << "continue" << std::endl;
  }
  //std::cout << "returning none " << std::endl;
  return false;
}

size_t  Destruct::count(void)
{
  return (this->__structures.size());
}

DStruct*       Destruct::find(DUnicodeString const & name) 
{
        //std::cout << "find " << std::endl;
  for (Iterator c = this->__structures.begin(); c != this->__structures.end(); ++c)
  {
          //std::cout << "for " << *c << std::endl;
    if ((*c)->name() == name)
    {
            //std::cout << "returning " << *c << std::endl;
      return (*c);
    }
    //std::cout << "continue" << std::endl;
  }
  //std::cout << "returning none " << std::endl;
  //throw DException("Destruct::find can't find " );
  return (NULL);
}

DStruct*        Destruct::find(const size_t index)
{
        //std::cout << "find int !" << std::endl;
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
