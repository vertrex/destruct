#ifndef __OBJECT_MANAGER_HPP__
#define __OBJECT_MANAGER_HPP__ 

#include "drealvalue.hpp"
#include "dobject.hpp"
#include "dattribute.hpp"
#include "dtype.hpp"
#include "protocol/dcppobject.hpp"
#include <map>

using namespace Destruct;

//pu besoin de templater c du dobject everywhere 
template<typename TypeName, DType::Type_t TypeId>
class ObjectManager : public DCppObjectSingleton< ObjectManager<TypeName, TypeId> >
{
public:
  typedef typename std::map<uint64_t, TypeName > mapType;
  typedef typename std::map<uint64_t, TypeName >::const_iterator mapIterator;

  ObjectManager(DStruct* dstruct, DValue const& args) : DCppObjectSingleton<ObjectManager<TypeName, TypeId > >(dstruct, args),  __currentID(0) 
  {
    this->init();
  }

  ObjectManager(ObjectManager const& copy) : DCppObjectSingleton<ObjectManager<TypeName, TypeId> >(copy),  __currentID(copy.__currentID), __objectsID(copy.__objectsID)
  {
    this->init();
  }

  DUInt64       __currentID;
  RealValue     <DFunctionObject*>      _registerObject, _object;


  DUInt64       registerObject(DValue const& arg)
  {
    TypeName object = arg.get<TypeName>();
    mapIterator i = this->__objectsID.begin();
    for (; i != this->__objectsID.end(); ++i)
      if (i->second == object)
        return i->first;

    DUInt64 id = this->__currentID;
    this->__objectsID[id] = object;
    this->__currentID++;
    return (id);
  }

  TypeName      object(DValue const& arg) 
  {
    DUInt64 id = arg.get<DUInt64>();
    mapIterator object = this->__objectsID.find(id);
    if (object != this->__objectsID.end())
      return (object->second);
    return (DNone);
    //return (NULL);
  }

private:
  mapType               __objectsID;

protected:
  ~ObjectManager()
  {
    mapIterator object = this->__objectsID.begin();
    for (; object != this->__objectsID.end(); ++object)
    {
    //delete object->second;
      object->second->destroy();
    }
  }

public:
  static size_t ownAttributeCount()
  {
    return (2);
  }

  static DAttribute* ownAttributeBegin()
  {
    static DAttribute attributes[] = 
    {
      DAttribute(DType::DUInt64Type, "registerObject", TypeId), 
      DAttribute(TypeId, "object", DType::DUInt64Type), 
    };

    return (attributes);
  }

  static DPointer<ObjectManager>* memberBegin()
  {
     static DPointer<ObjectManager> memberPointer[] =
     {
       DPointer<ObjectManager>(&ObjectManager::_registerObject, &ObjectManager::registerObject),
       DPointer<ObjectManager>(&ObjectManager::_object, &ObjectManager::object),
     };

    return (memberPointer);
  } 

  static DAttribute*  ownAttributeEnd()
  {
    return (ownAttributeBegin() + ownAttributeCount());
  }

  static DPointer<ObjectManager>* memberEnd()
  {
    return (memberBegin() + ownAttributeCount());
  } 
};

/*

template<typename TypeName, TypeId>
class ObjectManager : public DCppObject<ObjectManager>  //ValueManager (dobject, dfunction, etc.. ? pourquoi pas utiliser une dmap directement ??? 
{
public:
  typedef typename std::map<uint64_t, TypeName > mapType;
  typedef typename std::map<uint64_t, TypeName >::const_iterator mapIterator;i

  ObjectManager(DStruct* dstruct, DValue const& args) : __currentID(0) 
  {
  }

  ObjectManager(ObjectManager const& copy)
  {
  }

  DUInt64       registerObject(DValue const& arg)
  {
    TypeName object = arg.get<TypeName>();
    mapIterator i = this->__objectsID.begin();
    for (; i != this->__objectsID.end(); ++i)
      if (i->second == object)
        return i->first;

    DUInt64 id = this->__currentID;
    this->__objectsID[id] = object;
    this->__currentID++;
    return (id);
  }

  TypeName      object(uint64_t id) const
  {
    mapIterator object = this->__objectsID.find(id);
    if (object != this->__objectsID.end())
      return (object->second);
    return (NULL);
  }
protected:
  ~ObjectManager()
  {
    mapIterator object = this->__objectsID.begin();
    for (; object != this->__objectsID.end(); ++object)
    {
      delete object->second;
    }
  }

private:
  DUInt64               __currentID; 
  mapType               __objectsID;
};




template<typename TypeName>
class ObjectManager
{
public:
  typedef typename std::map<uint64_t, TypeName > mapType;
  typedef typename std::map<uint64_t, TypeName >::const_iterator mapIterator;
  ObjectManager() : __currentID(0) 
  {
  }

  ~ObjectManager()
  {
    mapIterator object = this->__objectsID.begin();
    for (; object != this->__objectsID.end(); ++object)
    {
      delete object->second;
    }
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

template<>
inline ObjectManager<Destruct::DObject* >::~ObjectManager()
{
  mapIterator object = this->__objectsID.begin();
  for (; object != this->__objectsID.end(); ++object)
  {
    object->second->destroy();
  }
}

template<>
inline Destruct::DObject* ObjectManager<Destruct::DObject* >::object(uint64_t id) const
{
  std::map<uint64_t, Destruct::DObject* >::const_iterator object = this->__objectsID.find(id);
  if (object != this->__objectsID.end())
    return (object->second);
  return Destruct::RealValue<Destruct::DObject*>(Destruct::DNone); 
}
*/

#endif
