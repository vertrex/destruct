#include "registryclient.hpp"
#include "clientobject.hpp"

RegistryClient::RegistryClient(std::string const& addr, uint32_t port) : Client(addr, port), __networkStream(NULL)
{

}

DObject* RegistryClient::start(void)
{
  Destruct::DStructs& destruct = Destruct::DStructs::instance();
  this->__networkStream = destruct.find("NetworkStream")->newObject(RealValue<DInt32>(this->connectionSocket())); //XXX doublon init in client?

  DStruct* registryS = destruct.find("Registry");
  
  ClientObject* root = new ClientObject(RealValue<DObject*>(this->networkStream()), RealValue<DObject*>(this->serializeRPC()), RealValue<DObject*>(this->deserializeRPC()), 0, registryS);

  return (root);
}

void   RegistryClient::printKey(DObject* key) ///remove not usefull anymore ! 
{
 DUnicodeString name = key->getValue("name"); //XXX
 std::cout << "get name " << std::endl;

 DObject* values = key->getValue("values"); 
 DObject* valuesList = values->getValue("list");
 DUInt64  valuesListSize = valuesList->call("size");

 DObject* subKeys = key->getValue("subkeys");
 DObject* subKeysList = subKeys->getValue("list");
 DUInt64  subKeysListSize = subKeysList->call("size");

 std::cout << "Key : "  << std::endl
           << "      name : " << name << std::endl; //not usefull anymore !

 if (valuesListSize)
 {
   std::cout << "      values ("  << valuesListSize <<  ") : " << std::endl;
   for (DUInt64 index = 0; index < valuesListSize; ++index)
   {
     DObject* value = valuesList->call("get", RealValue<DUInt64>(index));
     std::cout << "\t" << value->getValue("name").get<DUnicodeString>() << std::endl;
     value->destroy();
   }
 }

 if (subKeysListSize)
 {
   std::cout << "      keys ("  << subKeysListSize << ") : " << std::endl;
   for (DUInt64 index = 0; index < subKeysListSize; ++index)
   {
     DObject* subKey = subKeysList->call("get", RealValue<DUInt64>(index));
     this->printKey(subKey);
     subKey->destroy();
   }
 }
 subKeys->destroy();
 subKeysList->destroy();
 values->destroy();
 valuesList->destroy();
}
