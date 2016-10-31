#ifndef __OBJECTSTUB_HPP__
#define __OBJECTSTUB_HPP__

#include <iostream>
#include <stdint.h>
#include <map>
#include <vector>

class Object
{
public:
  Object();
  void        addValue(std::string key, std::string value);
  std::string getValue(std::string);
private:
  std::map<std::string, std::string> __values;
};

class ObjectManager
{
public:
  static ObjectManager&  instance(void);
  uint64_t object(Object* object);
  Object*  objectId(uint64_t object);
private:
  ObjectManager(); 
  ~ObjectManager();
  std::vector<Object*>         __objects;
  std::map<Object*, uint64_t>  __ids;
};

class ObjectStub
{
public:
  ObjectStub(uint64_t id, void* socket);
  std::string getValue(std::string value);
private:
  uint64_t      __id;
  void*         __socket;
}; 
