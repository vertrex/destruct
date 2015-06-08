#include "registryclient.hpp"
#include "clientobject.hpp"

RegistryClient::RegistryClient(std::string const& addr, uint32_t port) : Client(addr, port), __networkStream(NULL)
{

}

DObject* RegistryClient::start(void)
{
  std::cout << "connect to host" << std::endl;
  Destruct::DStructs& destruct = Destruct::DStructs::instance();
  this->__networkStream = destruct.find("NetworkStream")->newObject(RealValue<DInt32>(this->connectionSocket())); //XXX doublon init in client?

  DStruct* registryS = destruct.find("Registry");
  
  ClientObject* root = new ClientObject(RealValue<DObject*>(this->networkStream()), RealValue<DObject*>(this->serializeRPC()), RealValue<DObject*>(this->deserializeRPC()), 0, registryS);

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
 
 name->destroy();}
 subKeys->destroy();
 subKeysList->destroy();
 values->destroy();
 valuesList->destroy();
}

void    RegistryClient::printValue(DObject* value)
{
  DObject* name = value->getValue("name").get<DObject*>();
  std::cout << "\t" << name->getValue("keyName").get<DUnicodeString>() << std::endl;
}
