#ifndef __SERVER__
#define __SERVER__

#include <stdint.h>
#include <map>

#include "networkstream.hpp"

class RPCServer
{
public:
                        RPCServer(Destruct::NetworkStream networkStream);
  void                  getValue(Destruct::DObject* object);
  void                  setValue(Destruct::DObject* object);
  void                  call(Destruct::DObject* object);
  void                  call0(Destruct::DObject* object);
private:
  Destruct::NetworkStream         __networkStream;
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

  void                  findDStruct(Destruct::NetworkStream stream);
  void                  unknown(Destruct::NetworkStream stream);
  void                  registerObject(Destruct::DObject* object);
  Destruct::DObject*    objectById(uint64_t id);
  Destruct::NetworkStream stream(void); //return Destruct::NetworkStream
private:
  Destruct::DObject*      root;
  int                   __listenSocket;
  int32_t               __connectionSocket;
  void                  __bind(void);
  void                  __listen(void);
  int64_t               __currentID; 
  std::map<uint64_t, Destruct::DObject* > __objectsID;
};

#endif
