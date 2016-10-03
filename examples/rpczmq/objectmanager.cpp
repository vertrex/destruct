#include "objectmanager.hpp"
#include "serverfunctionobject.hpp"

using namespace Destruct;

ObjectManager::ObjectManager(DStruct* dstruct, DValue const& args) : DCppObjectSingleton<ObjectManager>(dstruct, args)
{
  this->init();
}

ObjectManager::ObjectManager(ObjectManager const& rhs) : DCppObjectSingleton<ObjectManager>(rhs), __objects(rhs.__objects), __ids(rhs.__ids), __fids(rhs.__fids)
{
  this->copy(this, rhs);
}

ObjectManager::~ObjectManager()
{
  this->clear();
}

void          ObjectManager::clear(void)
{
  for (uint64_t i = 0; i < this->__objects.size(); ++i)
    this->__objects[i]->destroy();

  this->__ids.clear(); //already call deleted via map ?
  this->__fids.clear(); 
}

DUInt64       ObjectManager::registerObject(DValue const& arg)
{
  DObject* object = arg;
  std::map<DObject*, DInt64>::const_iterator f = this->__ids.find(object);
  if (f != this->__ids.end())
    return (f->second);

  object->addRef(); //for all ?
  this->__objects.push_back(object);
  this->__ids[object] = this->__objects.size() -1;

  return (this->__objects.size() - 1);
}

DObject*      ObjectManager::object(DValue const& arg) 
{
  DUInt64 id = arg;

  if (id > this->__objects.size() - 1)
    return (DNone);

  return (this->__objects[id]);
}

DUInt64       ObjectManager::registerFunctionObject(DType type, DFunctionObject* func)
{
  std::map<DFunctionObject*, DInt64>::const_iterator f = this->__fids.find(func);
  if (f != this->__fids.end())
    return (f->second);

  ServerFunctionObject* serverFunctionObject = static_cast<ServerFunctionObject*>(DStructs::instance().find("ServerFunctionObject")->newObject());
  serverFunctionObject->argumentType = type.getArgumentType();
  serverFunctionObject->returnType = type.getReturnType();
  serverFunctionObject->functionObject = func;

  this->__objects.push_back(serverFunctionObject);
  this->__fids[func] = this->__objects.size() - 1;

  return (this->__objects.size() - 1);
}
