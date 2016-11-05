#include "dstructs.hpp"

#include "client.hpp"
#include "stubobject.hpp"
#include "serializerpc.hpp"
#include "networkstream.hpp"

#include "dsimpleobject.hpp"
#include "server.hpp"
#include "objectmanager.hpp"
#include "serverobject.hpp"
#include "serverfunctionobject.hpp"

extern "C"
{
  EXPORT void DestructExport()
  {
    Destruct::DStructs& destruct = Destruct::DStructs::instance();
    destruct.registerDStruct(makeNewDCpp<NetworkStream>("NetworkStream"));
    destruct.registerDStruct(makeNewDCpp<SerializeRPC>("SerializeRPC"));
    destruct.registerDStruct(makeNewDCpp<DeserializeRPC>("DeserializeRPC"));

    destruct.registerDStruct(makeNewDCppSingleton<ObjectManager >("ObjectManager"));
    destruct.registerDStruct(makeNewDCpp<ServerFunctionObject>("ServerFunctionObject"));


    DStruct* dstruct = makeNewDCpp<Server>("Server"); 
    destruct.registerDStruct(dstruct);

    dstruct = makeNewDCpp<Client>("Client");
    destruct.registerDStruct(dstruct);

    dstruct = new DStruct(NULL, "ClientArgument", DSimpleObject::newObject);
    dstruct->addAttribute(DAttribute(DType::DUInt32Type, "port"));
    dstruct->addAttribute(DAttribute(DType::DUnicodeStringType, "address"));
    dstruct->addAttribute(DAttribute(DType::DObjectType, "auth"));
    destruct.registerDStruct(dstruct); 

    dstruct = new DStruct(NULL, "RPCAuth", DSimpleObject::newObject);
    dstruct->addAttribute(DAttribute(DType::DUnicodeStringType, "cert")); //public and private key for client/server
    dstruct->addAttribute(DAttribute(DType::DUnicodeStringType, "certStore"));//path of accepted public key store for auth server
    dstruct->addAttribute(DAttribute(DType::DUnicodeStringType, "clientCert"));//accepted client cert 
    destruct.registerDStruct(dstruct); ;
  }
}
