#include "registryserver.hpp"

RegistryServer::RegistryServer()
{

}

void  RegistryServer::initRoot(void)
{
  std::cout << "init registry" << std::endl;
  Destruct::Destruct& destruct = Destruct::Destruct::instance();
  DObject* registry = destruct.generate("Registry");
  this->objectManager().registerObject(registry);

}
