#include "destruct.hpp"
#include "dexception.hpp"
#include "protocol/protocol.hpp"
#include "drealvalue.hpp"
#include "dnullobject.hpp"

#include <iostream>

namespace Destruct
{

Destruct::Destruct() : __nameSpace(new NameSpace(""))
{
   Protocol(this);
}

Destruct::~Destruct()
{
//XXX FIX BECAUSE SOME struct are registred twice 
//std::vector<DStruct* >::const_iterator dstruct = this->__structures.begin();
 //for (; dstruct != this->__structures.end(); ++dstruct)
 //delete (*dstruct);
 //this->__structures.clear();

 //this->__nameSpace.clear();
}

Destruct&     Destruct::instance()
{
  static Destruct       destruct;
  return (destruct);
}

void Destruct::registerDStruct(DStruct* dstruct)
{
  //XXX check if already exist
  this->__nameSpace->addStructure(dstruct);
}

void Destruct::registerDStruct(const std::string& nameSpaceName, DStruct* dstruct)
{
  NameSpace* nameSpace = this->__nameSpace->create(nameSpaceName);
  nameSpace->addStructure(dstruct);
}

bool Destruct::unregister(DStruct* dstructDef)
{
  //for (Iterator c = this->__structures.begin(); c != this->__structures.end(); ++c)
  //{
    //if (*c == dstructDef)
    //{
        //this->__structures.erase(c);
        //*c = NULL;
        //return true;
    //}
  //}
  //unregister module /namespace
  //XXX

  return (false);
}

/**
 * Total DStruct count
 */
size_t  Destruct::count(void)
{
 return (this->__nameSpace->count());
}

DStruct*        Destruct::find(const size_t index)
{
  return (this->__nameSpace->findDStruct(index));
}

DStruct*       Destruct::find(DUnicodeString const & dstructPath) 
{
  return (this->__nameSpace->findDStruct(dstructPath));
}

                        //rename newObject(name)
DObject *       Destruct::generate(DUnicodeString const& name)
{
  return (this->generate(name, RealValue<DObject*>(DNone)));
} 

                        //rename newObject(name)
DObject *       Destruct::generate(DUnicodeString const& name, DValue const& args)
{
  DStruct* dstruct = this->find(name);
  if (dstruct == NULL)
  {
    std::string error = "Can't find DStruct : " + name;
    throw DException(error);
  }

  return (dstruct->newObject(args));
} 

/**
 * NameSpace
 */
NameSpace::NameSpace(std::string const& name) : __name(name)
{
}

NameSpace::~NameSpace(void)
{
  std::cout << "Unloding NameSpace " << this->__name << std::endl;
}

const std::string  NameSpace::name(void) const
{
  return (this->__name);
}

void  NameSpace::addStructure(DStruct* dstruct)
{
  this->__structures.push_back(dstruct);
}

/**
 *  Return nameSpace named name from this nameSpaces or return NULL 
 */
NameSpace* NameSpace::nameSpace(std::string const& name) const
{
  std::vector<NameSpace*>::const_iterator nameSpace = this->__nameSpaces.begin();
  for (; nameSpace != this->__nameSpaces.end(); ++nameSpace)
  {
    if ((*nameSpace)->name() == name)
      return (*nameSpace);
  }

  return (NULL);
}

/**
 *  Create and return nameSpace from nameSpaces recursively
 */
NameSpace* NameSpace::create(std::string const& nameSpaces)
{
  std::string nameSpaceName = "";
  std::string subNameSpace = "";

  std::size_t idx = nameSpaces.find(".");
  if (idx == std::string::npos)
    nameSpaceName = nameSpaces;
  else
  {
    nameSpaceName = nameSpaces.substr(0, idx);
    subNameSpace = nameSpaces.substr(idx + 1);
  }

  NameSpace* nameSpace = this->nameSpace(nameSpaceName);
  if (!nameSpace)
  {
    nameSpace = new NameSpace(nameSpaceName);
    this->__nameSpaces.push_back(nameSpace); 
  }

  if (subNameSpace != "")
    return (nameSpace->create(subNameSpace));

  return (nameSpace);
}

/**
 *  Return count of DStruct in recursive nameSpace 
 */
size_t NameSpace::count(void) const
{
  size_t size = 0;

  std::vector<NameSpace*>::const_iterator nameSpace = this->__nameSpaces.begin();
  for (; nameSpace != this->__nameSpaces.end(); ++nameSpace)
  {
    size += (*nameSpace)->count();
  }
  return (this->__structures.size() + size);
}


/**
 * Return Dstruct at index : search in nameSpace tree recursively;
 */
DStruct*        NameSpace::findDStruct(size_t index)
{
  size_t dstructCount = this->__structures.size();
  
  if (index < dstructCount)
    return (this->dstruct(index));

  index -= dstructCount;

  std::vector<NameSpace*>::const_iterator nameSpace = this->__nameSpaces.begin();
  for (; nameSpace != this->__nameSpaces.end(); ++nameSpace)
  {
    size_t subNameSpaceSize = (*nameSpace)->count();

    if (index < subNameSpaceSize)
      return ((*nameSpace)->findDStruct(index));
    
    index -= subNameSpaceSize;
  }

  throw DException("NameSpace::findStruct index not found");
}

/**
 * Return DStruct name : search in nameSpace tree recursively
 */
DStruct*       NameSpace::findDStruct(DUnicodeString const& dstructPath)
{
  std::string nameSpaceName = "";
  std::string subNameSpace = "";

  std::size_t idx = dstructPath.find(".");
  if (idx == std::string::npos)
    return (this->dstruct(dstructPath));
  else
  {
    nameSpaceName = dstructPath.substr(0, idx);
    subNameSpace = dstructPath.substr(idx + 1);
  }

  NameSpace* nameSpace = this->nameSpace(nameSpaceName);
  if (!nameSpace)
    return (NULL);

  if (subNameSpace != "")
    return (nameSpace->findDStruct(subNameSpace));

  return (NULL);
}
/**
 *  return DStruct at index in structures of this NameSpace
 */
DStruct*        NameSpace::dstruct(const size_t index)
{
  if (index < this->__structures.size())
    return (this->__structures[index]);
  throw DException("NameSpace : Can't find DStruct at index in NameSpace " + this->name());
}

DStruct*        NameSpace::dstruct(std::string const& name)
{
  std::vector<DStruct*>::iterator dstruct = this->__structures.begin();
  for (; dstruct != this->__structures.end(); ++dstruct)
  {
    if ((*dstruct)->name() == name)
      return ((*dstruct)); 
  }

  return (NULL);
}

}
