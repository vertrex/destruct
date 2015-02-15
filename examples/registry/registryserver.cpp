#include "dstructs.hpp"
#include "registryserver.hpp"

RegistryServer::RegistryServer(int32_t port) : Server(port)
{
}

void  RegistryServer::initRoot(void)
{
  std::cout << "init registry" << std::endl;
  Destruct::DStructs& destruct = Destruct::DStructs::instance();
  DObject* registry = destruct.generate("Registry");
  this->objectManager().registerObject(registry);
}
