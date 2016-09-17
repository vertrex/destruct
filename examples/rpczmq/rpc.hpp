#ifndef __RPC__
#define __RPC__

#include <stdint.h>

#include "destruct.hpp"

#include "client.hpp"
#include "server.hpp"
#include "clientobject.hpp"
#include "fsobject.hpp"

class RPC
{
public:
  RPC();
  ~RPC();

  void  serve(uint32_t port);
  void  connect(std::string const& addr, uint32_t port);
};

#endif
