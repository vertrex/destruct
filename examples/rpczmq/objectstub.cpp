#include "objectstub.hpp"

/**
 *  Object
 */
Object::Object()
{
}

void    Object::addValue(std::string key, std::string value)
{
  this->__values[key] = value;
}

std::string      Object::getValue(std::string key)
{
  return __values[key];
}
/**
 *  ObjectManager
 */
ObjectManager::ObjectManager()
{
}

ObjectManager::~ObjectManager()
{
}

ObjectManager& ObjectManager::instance()
{
  static ObjectManager objectManager;
  return objectManager;
}

uint64_t        ObjectManager::object(Object* object)
{
  std::map<Object*, uint64_t>::const_iterator f = this->__ids.find(object);
  if (f != this->__ids.end())
    return (f->second);

  this->__objects.push_back(object);
  this->__ids[object] = this->__objects.size() -1;

  return (this->__objects.size() - 1);
}

Object*      ObjectManager::objectId(uint64_t id) 
{
  return (this->__objects[id]);
}

/**
 *   ObjectStub
 */
ObjectStub::ObjectStub(uint64_t id, void* socket) : __id(id), __socket(socket)
{
}

std::string ObjectStub::getValue(std::string value)
{
  ObjectManager& objectManager = ObjectManager::instance();
  Object* object = objectManager.objectId(__id);
  //send value
  //get reply
  return (object->getValue(value));
}

