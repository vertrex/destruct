#ifndef __CLIENT__
#define __CLIENT__

#include <iostream>
#include <stdint.h>

#include "networkstream.hpp" 
#include "dstruct.hpp"
#include "server.hpp"

using namespace Destruct;

class Client
{
public:
                        Client(std::string const& addr, uint32_t port);
                        ~Client();
  void                  start(void); //return NetworkStream 
  bool                  print(DStruct* dstruct) const;
  int32_t               _send(void* buff, int32_t size) const;
  int32_t               _receive(void* buff, int32_t len);
  NetworkStream         stream(void); //return NetworkStream
  Destruct::DStruct*    remoteFind(const std::string name);
private:
  void                  __close(void);
  void                  __connect(std::string const& addr, uint32_t port);
  int32_t               __connectionSocket;
  ObjectManager         __objectManager;
};

#endif
