#ifndef __registryserver__
#define __registryserver__

#include <stdint.h>
#include <map>

#include "server.hpp"


class RegistryServer : public Server
{
public:
                        RegistryServer();
  virtual void          initRoot(void);
};

#endif
