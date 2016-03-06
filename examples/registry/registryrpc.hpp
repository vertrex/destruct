#ifndef __registryrpc_hpp__
#define __registryrpc_hpp__

#include "registry_common.hpp"

class RegistryRPC
{
public:
 RegistryRPC();
 ~RegistryRPC();

 void                     local(const std::string fileName);
 void                     serve(uint32_t port);
 void                     connect(std::string const& filePath, std::string const& addr, uint32_t port);
 static const std::string usage(void);
 static void              show(Destruct::DObject* object);
 EXPORT static void       toFile(std::string fileName, Destruct::DObject* object, std::string type);
};


#endif
