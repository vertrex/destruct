#ifndef __registryclient__
#define __registryclient__

#include <stdint.h>
#include <map>

#include "client.hpp"


class RegistryClient : public Client
{
public:
  EXPORT		    RegistryClient(std::string const& addr, uint32_t port);
  EXPORT virtual DObject*   start(void);
  EXPORT void		    printKey(DObject* key);
private: 
  DObject*		    __networkStream; //XXX ?? already in client
};

#endif
