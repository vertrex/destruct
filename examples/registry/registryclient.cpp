#include "registryclient.hpp"
#include "clientobject.hpp"

RegistryClient::RegistryClient(std::string const& addr, uint32_t port) : Client(addr, port)
{

}

DObject* RegistryClient::start(void)
{
  std::cout << "connect to host" << std::endl;
  NetworkStream networkStream(NULL, RealValue<DInt32>(this->connectionSocket()));

  Destruct::Destruct& destruct = Destruct::Destruct::instance();

  DStruct* registryS = destruct.find("Registry");
  ClientObject* root = new ClientObject(networkStream, 0, registryS, this->objectManager(), this->functionObjectManager()); 

  return (root);

}

void   RegistryClient::printKey(DObject* key)
{
 DObject* name = key->getValue("keyName").get<DObject*>();

 DObject* values = key->getValue("values").get<DObject*>(); 
 DObject* valuesList = values->getValue("list").get<DObject*>();
 DUInt64  valuesListSize = valuesList->call("size").get<DUInt64>();

 DObject* subKeys = key->getValue("subkeys").get<DObject*>();
 DObject* subKeysList = subKeys->getValue("list").get<DObject*>();
 DUInt64  subKeysListSize = subKeysList->call("size").get<DUInt64>();

 std::cout << "get size " << std::endl;
 std::cout << "Key : "  << std::endl
           << "      name : " << name->getValue("keyName").get<DUnicodeString>() << std::endl;

 if (valuesListSize)
 {
   std::cout << "      values ("  << valuesListSize <<  ") : " << std::endl;
   for (DUInt64 index = 0; index < valuesListSize; ++index)
   {
     DObject* value = valuesList->call("get", RealValue<DUInt64>(index)).get<DObject*>();
     printValue(value);
     value->destroy();
   }
   subKeys->destroy();
   subKeysList->destroy();
 }

 if (subKeysListSize)
 {
   std::cout << "      keys ("  << subKeysListSize << ") : " << std::endl;
   for (DUInt64 index = 0; index < subKeysListSize; ++index)
   {
     DObject* subKey = subKeysList->call("get", RealValue<DUInt64>(index)).get<DObject*>();
     this->printKey(subKey);
     subKey->destroy();
   }
   values->destroy();
   valuesList->destroy();
 }
}

void    RegistryClient::printValue(DObject* value)
{
  DObject* name = value->getValue("name").get<DObject*>();
  std::cout << "\t" << name->getValue("keyName").get<DUnicodeString>() << std::endl;
}
