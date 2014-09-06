#ifndef __RPC_SERVER__
#define __RPC_SERVER__

#include <map>
#include <string>
#include "dobject.hpp"
#include "networkstream.hpp"
#include "protocol/dserialize.hpp"

class ObjectManager
{
public:
                                          ObjectManager();
  uint64_t                                registerObject(Destruct::DObject* object);
  Destruct::DObject*                      object(uint64_t id) const;
private:
  uint64_t                                __currentID; 
  std::map<uint64_t, Destruct::DObject* > __objectsID;
};

class RPCServer
{
public:
                                RPCServer(Destruct::NetworkStream networkStream, ObjectManager & objectManager);
  void                          findDStruct(void);
  void                          getValue(Destruct::DObject* object);
  void                          setValue(Destruct::DObject* object);
  void                          call(Destruct::DObject* object);
  void                          call0(Destruct::DObject* object);
  void                          unknown(const std::string& cmd);
  Destruct::NetworkStream&                 networkStream(void);
private:
  Destruct::NetworkStream       __networkStream;
  Destruct::DSerialize*         __serializer;
};

#endif
