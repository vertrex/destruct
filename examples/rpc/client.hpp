#ifndef __CLIENT__
#define __CLIENT__

#include <iostream>
#include <stdint.h>

#include "networkstream.hpp" 
#include "rpcserver.hpp"

using namespace Destruct;

class Client
{
public:
                        Client(std::string const& addr, uint32_t port);
                        ~Client();
  void                  start(void);
private:
  bool                  __print(DStruct* dstruct) const;
  bool                  __print(DObject* dobject) const;
  Destruct::DStruct*    __remoteFind(NetworkStream&, const std::string name);
  void                  __close(void);
  void                  __connect(std::string const& addr, uint32_t port);
  int32_t               __connectionSocket;
  ObjectManager         __objectManager;
};

#endif
