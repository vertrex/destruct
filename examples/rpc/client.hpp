#ifndef __CLIENT__
#define __CLIENT__

#include <iostream>
#include <stdint.h>

#include "networkstream.hpp" 
#include "objectmanager.hpp"
#include "serializerpc.hpp"
using namespace Destruct;

class ServerFunctionObject;

class Client
{
public:
  Client(std::string const& addr, uint32_t port);
  ~Client();

  virtual DObject*                      start(void) = 0;

  bool                                  print(DObject* dobject) const;
  bool                                  print(DStruct* dstruct) const;
  Destruct::DStruct*                    remoteFind(const std::string name);

  int32_t                               connectionSocket(void) const;
  NetworkStream*                        networkStream(void) const;
  DSerialize*                           serializeRPC(void) const;
  ObjectManager<DObject*>&              objectManager(void);
  ObjectManager<ServerFunctionObject*>& functionObjectManager(void);
private:
  void                                  __connect(std::string const& addr, uint32_t port);
  void                                  __close(void);
  int32_t                               __connectionSocket;
  NetworkStream*                        __networkStream;
  DSerialize*                           __serializeRPC; 
  ObjectManager<DObject* >              __objectManager;
  ObjectManager<ServerFunctionObject*>  __functionObjectManager;
};

#endif
