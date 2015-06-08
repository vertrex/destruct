#include "objectmanager.hpp"

using namespace Destruct;

ObjectManager::ObjectManager(DStruct* dstruct, DValue const& args) : DCppObjectSingleton<ObjectManager>(dstruct, args),  __currentID(0) 
{
  this->init();
}

ObjectManager::ObjectManager(ObjectManager const& copy) : DCppObjectSingleton<ObjectManager>(copy),  __currentID(copy.__currentID), __objectsID(copy.__objectsID)
{
  this->init();
}

ObjectManager::~ObjectManager() //singleton !
{
  this->clear();
}

void          ObjectManager::clear(void)
{
  mapIterator object = this->__objectsID.begin();
  for (; object != this->__objectsID.end(); ++object)
    object->second->destroy();

  this->__objectsID.clear();
}

DUInt64       ObjectManager::registerObject(DValue const& arg)
{
  DObject* object = arg.get<DObject*>();
  mapIterator i = this->__objectsID.begin();
  for (; i != this->__objectsID.end(); ++i)
    if (i->second == object)
    {
      object->destroy();
      return (i->first);
    }

  DUInt64 id = this->__currentID;
  this->__objectsID[id] = object;
  this->__currentID++;

  return (id);
}

DObject*      ObjectManager::object(DValue const& arg) 
{
  DUInt64 id = arg.get<DUInt64>();
  mapIterator object = this->__objectsID.find(id);
  if (object != this->__objectsID.end())
  {   
    object->second->addRef();
    return (object->second); //addRef
  }

  return (DNone);
}
