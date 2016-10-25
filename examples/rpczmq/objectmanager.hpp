#ifndef __OBJECTMANAGER_HPP__
#define __OBJECTMANAGER_HPP__ 

#include <map>

#include "drealvalue.hpp"
#include "dobject.hpp"
#include "dattribute.hpp"
#include "dtype.hpp"
#include "protocol/dcppobject.hpp"

using namespace Destruct;

class ObjectManager : public DCppObjectSingleton<ObjectManager>
{
public:
  //typedef std::map<uint64_t, DObject* > mapType;
  //typedef std::map<uint64_t, DObject* >::const_iterator mapIterator;


  ObjectManager(DStruct* dstruct, DValue const& args);
  ObjectManager(ObjectManager const& copy);

  RealValue     <DFunctionObject*>      _registerObject, _object, _clear;

  DUInt64       registerObject(DValue const& arg);
  DUInt64       registerFunctionObject(DType type, DFunctionObject* func);
  DObject*      object(DValue const& arg);

  void          clear(void);
private:
  std::vector<DObject*>              __objects;
  std::map<DObject*, DInt64>         __ids;
  std::map<DFunctionObject*, DInt64> __fids;
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
