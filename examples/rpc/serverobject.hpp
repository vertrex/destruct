#ifndef __RPC_SERVER__
#define __RPC_SERVER__

#include "networkstream.hpp"
#include "objectmanager.hpp"
#include "protocol/dserialize.hpp"

using namespace Destruct;

class ServerFunctionObject;

class ServerObject
{
public:
                                ServerObject(Destruct::NetworkStream networkStream, ObjectManager<DObject*>& objectManager, ObjectManager<ServerFunctionObject*>& functionObjectManager);
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
  DSerialize*                   __serializer;
};

#endif
