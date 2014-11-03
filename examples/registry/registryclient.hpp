#ifndef __registryclient__
#define __registryclient__

#include <stdint.h>
#include <map>

#include "client.hpp"


class RegistryClient : public Client
{
public:
                        RegistryClient(std::string const& addr, uint32_t port);
  virtual DObject*      start(void);
  void                  printKey(DObject* key);
  void                  printValue(DObject* key);
};

#endif
