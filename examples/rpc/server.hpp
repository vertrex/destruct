#ifndef __SERVER__
#define __SERVER__

#include <stdint.h>
#include <map>

#include "networkstream.hpp"
#include "ddynamicobject.hpp"
#include "protocol/dcppobject.hpp"
#include "fsobject.hpp"
#include "protocol/dserialize.hpp"


class ObjectManager
{
public:
                        ObjectManager();
  uint64_t              registerObject(Destruct::DObject* object);
  Destruct::DObject*    object(uint64_t id) const;
private:
  uint64_t                                __currentID; 
  std::map<uint64_t, Destruct::DObject* > __objectsID;
};

class RPCServer
{
public:
                        RPCServer(Destruct::NetworkStream networkStream, ObjectManager & objectManager);
  void                  findDStruct(void);
  void                  getValue(Destruct::DObject* object);
  void                  setValue(Destruct::DObject* object);
  void                  call(Destruct::DObject* object);
  void                  call0(Destruct::DObject* object);
private:
  Destruct::NetworkStream    __networkStream;
  ObjectManager &            __objectManager;
  Destruct::DSerialize*      __serializer;
};


class Server
{
public:
                        Server(); 
  int32_t               _receive(void* buff, int32_t lenflags);
  int32_t               _send(void* buff, int32_t size) const;
  void                  serve(void);
  void                  initFS(void);
  void                  showFS(void);

  void                  unknown(Destruct::NetworkStream stream);
  Destruct::NetworkStream stream(void);
private:
  ObjectManager         __objectManager;
  int                   __listenSocket;
  int32_t               __connectionSocket;
  void                  __bind(void);
  void                  __listen(void);
};

#endif
