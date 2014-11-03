#ifndef __RPC_SERVER__
#define __RPC_SERVER__

#include <map>
#include <string>
#include "dobject.hpp"
#include "networkstream.hpp"
#include "protocol/dserialize.hpp"

using namespace Destruct;

class ServerFunctionObject;

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

class RPCServer
{
public:
                                RPCServer(Destruct::NetworkStream networkStream, ObjectManager<DObject*>& objectManager, ObjectManager<ServerFunctionObject*>& functionObjectManager);
  void                          findDStruct(void);
  void                          getValue(Destruct::DObject* object);
  void                          setValue(Destruct::DObject* object);
  void                          call(Destruct::DObject* object);
  void                          call0(Destruct::DObject* object);
  void                          functionCall(ServerFunctionObject* object);
  void                          functionCall0(ServerFunctionObject* object);
  void                          unknown(const std::string& cmd);
  Destruct::NetworkStream&      networkStream(void);
private:
  Destruct::NetworkStream       __networkStream;
  Destruct::DSerialize*         __serializer;
};

#endif
