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
class ObjectManager : public DCppObjectSingleton<ObjectManager>
{
public:
  typedef typename std::map<uint64_t, DObject* > mapType;
  typedef typename std::map<uint64_t, DObject* >::const_iterator mapIterator;

  ObjectManager(DStruct* dstruct, DValue const& args) : DCppObjectSingleton<ObjectManager>(dstruct, args),  __currentID(0) 
  {
    this->init();
  }

  ObjectManager(ObjectManager const& copy) : DCppObjectSingleton<ObjectManager>(copy),  __currentID(copy.__currentID), __objectsID(copy.__objectsID)
  {
    this->init();
  }

  DUInt64       __currentID;
  RealValue     <DFunctionObject*>      _registerObject, _object;


  DUInt64       registerObject(DValue const& arg)
  {
    DObject* object = arg.get<DObject*>();
    mapIterator i = this->__objectsID.begin();
    for (; i != this->__objectsID.end(); ++i)
      if (i->second == object)
        return i->first;

    DUInt64 id = this->__currentID;
    this->__objectsID[id] = object;
    this->__currentID++;
    return (id);
  }

  DObject*      object(DValue const& arg) 
  {
    DUInt64 id = arg.get<DUInt64>();
    mapIterator object = this->__objectsID.find(id);
    if (object != this->__objectsID.end())
    {   
      object->second->addRef();
      return (object->second);
    }
    return (DNone);
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
      DAttribute(DType::DUInt64Type, "registerObject", DType::DObjectType), 
      DAttribute(DType::DObjectType, "object", DType::DUInt64Type), 
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

#endif
