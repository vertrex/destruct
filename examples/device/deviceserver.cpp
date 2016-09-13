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
 
  //DMutableObject* arg = static_cast<DMutableObject*>(destruct.generate("DMutable"));
  //arg->setValueAttribute(DType::DUnicodeStringType, "filePath", RealValue<DUnicodeString>("/home/vertrex/dump/case/cfreds-raw/cfreds.raw.raw")); 
  //arg->setValueAttribute(DType::DInt8Type, "input",  RealValue<DInt8>(DStream::Input));

  //DObject* device = destruct.generate("DStream", RealValue<DObject*>(arg));
  DObject* deviceList = destruct.generate("DeviceList");


  this->objectManager()->call("registerObject", RealValue<DObject*>(deviceList));
}
