#ifndef __SERVER__
#define __SERVER__

#include "objectmanager.hpp"
#include "serverfunctionobject.hpp"
#include "networkstream.hpp"

#include "serializerpc.hpp"
#include "protocol/dserialize.hpp"

class Server
{
public:
  Server(uint32_t port);
  ~Server();

  virtual void                          initRoot(void) = 0;
  void                                  serve();
  void                                  showRoot(void);
  void                                  unknown(const std::string& cmd);
  void                                  findDStruct(void);
  ObjectManager<Destruct::DObject*>&    objectManager(void);
  ObjectManager<ServerFunctionObject*>& functionObjectManager(void);
private:
  int                                   __listenSocket;
  int32_t                               __connectionSocket;
  void                                  __bind(int32_t port);
  void                                  __listen(void);
  NetworkStream*                        __networkStream;
  DSerialize*                           __serializer;
  ObjectManager<Destruct::DObject*>     __objectManager;
  ObjectManager<ServerFunctionObject*>  __functionObjectManager;
};

#endif
