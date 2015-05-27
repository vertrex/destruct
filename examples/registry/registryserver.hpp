#ifndef __registryserver__
#define __registryserver__

#include <stdint.h>
#include <map>

#include "server.hpp"


class RegistryServer : public Server
{
public:
  EXPORT                RegistryServer(int32_t port);
  virtual void          initRoot(void);
};

#endif
