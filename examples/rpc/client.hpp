#ifndef __CLIENT__
#define __CLIENT__

#include <iostream>
#include <stdint.h>

#include "networkstream.hpp" 
#include "rpcserver.hpp"


using namespace Destruct;

class ServerFunctionObject;

class Client
{
public:
                        Client(std::string const& addr, uint32_t port);
                        ~Client();
  virtual DObject*      start(void);
  int32_t               connectionSocket(void) const;
  ObjectManager<DObject*>&        objectManager(void);
  ObjectManager<ServerFunctionObject*>&       functionObjectManager(void);
private:
  bool                  __print(DStruct* dstruct) const;
  bool                  __print(DObject* dobject) const;
  Destruct::DStruct*    __remoteFind(NetworkStream&, const std::string name);
  void                  __close(void);
  void                  __connect(std::string const& addr, uint32_t port);
  int32_t               __connectionSocket;
  ObjectManager<DObject* > __objectManager;
  ObjectManager<ServerFunctionObject*> __functionObjectManager;
};

#endif
