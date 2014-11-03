#ifndef __registryrpc_hpp__
#define __registryrpc_hpp__

#include "registry_common.hpp"
//#include <stdint.h>
//#include "dobject.hpp"

class RegistryRPC
{
public:
 RegistryRPC();
 ~RegistryRPC();
 void  local(const std::string fileName);
 void  serve(void);
 void  connect(std::string const& addr, uint32_t port);
 static const std::string usage(void);
};


#endif
