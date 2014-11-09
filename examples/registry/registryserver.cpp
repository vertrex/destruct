#include "destruct.hpp"
#include "registryserver.hpp"

RegistryServer::RegistryServer(int32_t port) : Server(port)
{
}

void  RegistryServer::initRoot(void)
{
  std::cout << "init registry" << std::endl;
  Destruct::Destruct& destruct = Destruct::Destruct::instance();
  DObject* registry = destruct.generate("Registry");
  this->objectManager().registerObject(registry);
}
