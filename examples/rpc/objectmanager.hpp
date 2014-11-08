#ifndef __OBJECT_MANAGER_HPP__
#define __OBJECT_MANAGER_HPP__ 

#include <map>

template<typename TypeName>
class ObjectManager
{
public:
  typedef typename std::map<uint64_t, TypeName > mapType;
  typedef typename std::map<uint64_t, TypeName >::const_iterator mapIterator;
  ObjectManager() : __currentID(0) 
  {
  }

  uint64_t                     registerObject(TypeName object)
  {
    mapIterator i = this->__objectsID.begin();
    for (; i != this->__objectsID.end(); ++i)
      if (i->second == object)
        return i->first;

    uint64_t id = this->__currentID;
    this->__objectsID[id] = object;
    this->__currentID++;
    return (id);
  }

  TypeName                      object(uint64_t id) const
  {
    mapIterator object = this->__objectsID.find(id);
    if (object != this->__objectsID.end())
      return (object->second);
    return (NULL);
  }
private:
  uint64_t              __currentID; 
  mapType               __objectsID;
};

#endif
