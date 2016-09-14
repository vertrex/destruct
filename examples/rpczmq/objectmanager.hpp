#ifndef __OBJECT_MANAGER_HPP__
#define __OBJECT_MANAGER_HPP__ 

#include "drealvalue.hpp"
#include "dobject.hpp"
#include "dattribute.hpp"
#include "dtype.hpp"
#include "protocol/dcppobject.hpp"
#include <map>

using namespace Destruct;

class ObjectManager : public DCppObjectSingleton<ObjectManager>
{
public:
  typedef std::map<uint64_t, DObject* > mapType;
  typedef std::map<uint64_t, DObject* >::const_iterator mapIterator;

  ObjectManager(DStruct* dstruct, DValue const& args);
  ObjectManager(ObjectManager const& copy);

  DUInt64       __currentID;
  RealValue     <DFunctionObject*>      _registerObject, _object, _clear;


  DUInt64       registerObject(DValue const& arg);
  DObject*      object(DValue const& arg);
  void          clear(void);
private:
  mapType        __objectsID;
protected:
  ~ObjectManager();
public:
  static size_t ownAttributeCount()
  {
    return (3);
  }

  static DAttribute* ownAttributeBegin()
  {
    static DAttribute attributes[] = 
    {
      DAttribute(DType::DUInt64Type, "registerObject", DType::DObjectType), 
      DAttribute(DType::DObjectType, "object", DType::DUInt64Type), 
      DAttribute(DType::DNoneType, "clear", DType::DNoneType), 
    };

    return (attributes);
  }

  static DPointer<ObjectManager>* memberBegin()
  {
     static DPointer<ObjectManager> memberPointer[] =
     {
       DPointer<ObjectManager>(&ObjectManager::_registerObject, &ObjectManager::registerObject),
       DPointer<ObjectManager>(&ObjectManager::_object, &ObjectManager::object),
       DPointer<ObjectManager>(&ObjectManager::_clear, &ObjectManager::clear),
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
