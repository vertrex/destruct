#include "dstructs.hpp"
#include "deviceserver.hpp"

#include "protocol/dmutableobject.hpp"
#include "protocol/dstream.hpp"

DeviceServer::DeviceServer(int32_t port) : Server(port)
{
}

void  DeviceServer::initRoot(void)
{
  Destruct::DStructs& destruct = Destruct::DStructs::instance();
 
  DObject* deviceList = destruct.generate("DeviceList");
  this->objectManager()->call("registerObject", RealValue<DObject*>(deviceList));
}
