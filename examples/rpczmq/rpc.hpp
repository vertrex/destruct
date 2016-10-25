#ifndef __RPC_HPP__
#define __RPC_HPP__

#include <stdint.h>

#include "destruct.hpp"

#include "client.hpp"
#include "server.hpp"
#include "stubobject.hpp"

class RPC
{
public:
  RPC();
  ~RPC();

  int  usage(void);
  void  serve(uint32_t port);
  void  serve(uint32_t port, std::string cert, std::string clientCert);
};

#endif
