#include "registryrpc.hpp"
#include "registry.hpp"

using namespace Destruct;

RegistryRPC::RegistryRPC()
{
  Destruct::DType::init();
  Registry::declare();
}

RegistryRPC::~RegistryRPC()
{
  Destruct::DType::clean();
}

void RegistryRPC::local(const std::string filePath)
{
  Destruct::DStructs& destruct = Destruct::DStructs::instance();
  DObject* registry = destruct.generate("Registry");

  std::cout << "opening file " << filePath << std::endl;
  DObject* regf = registry->call("open", RealValue<DUnicodeString>(filePath));
  std::cout << "Regf deserialized " << std::endl;

  std::string fileName(filePath, filePath.rfind("/") + 1);
  RegistryRPC::toFile(fileName + ".bin", regf, "Binary");
  RegistryRPC::toFile(fileName + ".raw", regf, "Raw");
  RegistryRPC::toFile(fileName + "registry.text", regf, "Text");
  //RegistryRPC::show(regf);
  regf->destroy();
  registry->destroy();
}

//ret object and use same code as local
void RegistryRPC::connect(std::string const& filePath, std::string const& addr, uint32_t port)
{
  //use destruct import 
  DObject* loader = DStructs::instance().generate("Import"); 
 
  try 
  { 
    loader->call("file", RealValue<DUnicodeString>("../modules/libdestruct_rpczmq.so"));
  } 
  catch(...)
  {
    loader->call("file", RealValue<DUnicodeString>("destruct_rpczmq.dll"));
  }

  DObject* argument = DStructs::instance().generate("ClientArgument");

  argument->setValue("address", RealValue<DUnicodeString>(addr));
  argument->setValue("port", RealValue<DUInt32>(port));

  std::cout << "Connecting to " << addr << ":" << port << std::endl;
  DObject*  client = DStructs::instance().generate("Client", RealValue<DObject*>(argument));
  std::cout << "Connected" << std::endl;
  //client = DStructs::instance().generate("RecursiveThreadSafeObject", RealValue<DObject*>(client));

  DObject* serverLoader = client->call("generate", RealValue<DUnicodeString>("Import"));
 
  try
  { 
    serverLoader->call("file", RealValue<DUnicodeString>("../modules/libdestruct_registry.so"));
  }
  catch (...)
  {
    serverLoader->call("file", RealValue<DUnicodeString>("destruct_registry.dll"));
  }
  DObject* registry = client->call("generate", RealValue<DUnicodeString>("Registry"));

 
  DObject* regf = registry->call("open", RealValue<DUnicodeString>(filePath));
  //std::cout << "pring key" << std::endl;
  std::string fileName(filePath, filePath.rfind("/") + 1);
  RegistryRPC::toFile(fileName + ".bin", regf, "Binary");
  //RegistryRPC::toFile(fileName + ".raw", regf, "Raw");
  //RegistryRPC::toFile(fileName + "registry.text", regf, "Text");
  ////RegistryRPC::show(regf);
  //regf->destroy();
  //registry->destroy(); 
  //DObject* rootKey = regf->getValue("key");
  //this->printKey(rootKey);
}

const std::string RegistryRPC::usage(void)
{
  return ("Usage registry :\n"
          "\t-l file : read registry file\n"
          "\t-c file address port : connect and process file on server\n");
}

int main(int argc, char** argv)
{
  if (argc < 2)
  {
   std::cout << RegistryRPC::usage();
   return (1); 
  }

  RegistryRPC   registryRPC;
//use import, load rpc and subclass ?
//sync struct with client so it doesn t need any destruct declaration
  try 
  {
    if (std::string(argv[1]) == std::string("-l") && argc == 3)
      registryRPC.local(std::string(argv[2]));
    else if (std::string(argv[1]) == std::string("-c"))
    {
      if (argc == 5)
      {
        registryRPC.connect(argv[2], argv[3], atoi(argv[4]));
      }
      else
        std::cout << RegistryRPC::usage();
    }
    else
      std::cout << RegistryRPC::usage();
  }
  catch (Destruct::DException const& exception)
  {
    std::cout << "Error : " << std::endl << exception.error() << std::endl; 
  }
  catch (const std::string& error)
  {
    std::cout << "Error : " << std::endl << error << std::endl;
  }

 /** option **/
 /*
    --local : read file locally 
    --server : act as server 
    --connect : connect as client
      --conect --distant-processing : (default computa distance)
      --connect --local-processing : ou une stream et fait juste les call read/seek sur la stream et compute locally
 */
}

void            RegistryRPC::toFile(std::string filePath, DObject* object, std::string type)
{
  Destruct::DStructs& destruct = Destruct::DStructs::instance();

  DMutableObject* arg = static_cast<DMutableObject*>(destruct.generate("DMutable"));
  arg->setValueAttribute(DType::DUnicodeStringType, "filePath", RealValue<DUnicodeString>(filePath)); 
  arg->setValueAttribute(DType::DInt8Type, "input", RealValue<DInt8>(DStream::Output));
  DObject* dstream = destruct.generate("DStream", RealValue<DObject*>(arg));
  arg->destroy();
 
  DObject* serializer = destruct.find("Serialize" + type)->newObject(RealValue<DObject*>(dstream));
  serializer->call("DObject", RealValue<DObject*>(object));
  serializer->destroy();
  dstream->destroy(); 
}

void            RegistryRPC::show(DObject* object)
{
  Destruct::DStructs& destruct = Destruct::DStructs::instance();
  DObject* dstream = destruct.generate("DStreamCout", RealValue<DObject*>());
 
  DObject* serializer = destruct.find("SerializeText")->newObject(RealValue<DObject*>(dstream));
  serializer->call("DObject", RealValue<DObject*>(object));
  serializer->destroy();
  dstream->destroy(); 
}


void   RegistryRPC::printKey(DObject* key) ///remove not usefull anymore ! 
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
