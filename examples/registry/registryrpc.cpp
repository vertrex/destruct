#include "registryrpc.hpp"
#include "registry.hpp"

#include "server.hpp"
#include "client.hpp"
#include "registryserver.hpp"
#include "registryclient.hpp"

#include "client.hpp"

using namespace Destruct;

RegistryRPC::RegistryRPC()
{
  Destruct::DType::init();
  Registry::declare();
  Client::declare();
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

  DObject* regf = registry->call("open", RealValue<DUnicodeString>(filePath)).get<DObject*>();

  std::cout << "Regf deserialized " << std::endl;

  std::string fileName(filePath, filePath.rfind("/") + 1);
  Registry::toFile(fileName + ".bin", regf, "Binary");
  Registry::toFile(fileName + ".raw", regf, "Raw");
  Registry::toFile(fileName + "registry.text", regf, "Text");
  //Registry::show(regf); 
}


void RegistryRPC::serve(uint32_t port)
{
 RegistryServer  server(port);
 server.initRoot();
 //server.daemonize();
 server.serve();
}

void RegistryRPC::connect(std::string const& filePath, std::string const& addr, uint32_t port)
{
  RegistryClient client(addr, port);
  DObject* registry = client.start();

  DValue regfv = registry->call("open", RealValue<DUnicodeString>(filePath));
  DObject* regf = regfv.get<DObject*>();
 
  //std::string fileName(filePath, filePath.rfind("/") + 1);
  //Registry::toFile(fileName + "registry-rpc.text", regf, "Text");
  DObject* rootKey = regf->getValue("key").get<DObject*>();

  client.printKey(rootKey);

  return ;
/*
  DObject* subKeys = rootKey->getValue("subkeys").get<DObject*>();
  DObject* subKeysList = subKeys->getValue("list").get<DObject*>();

  DObject* iterator = subKeysList->call("iterator").get<DObject*>(); 
  
  for(; iterator->call("isDone").get<DInt8>() != 1; iterator->call("nextItem"))
  {
    DObject* subKey = iterator->call("currentItem").get<DObject*>();
    std::cout << "subKey " << subKey->instanceOf()->name() << std::endl;
  }
*/
}

const std::string RegistryRPC::usage(void)
{
  return ("Usage registry :\n"
          "\t-l file : read registry file\n"
          "\t-d (port) : run server (default port 3583\n"
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

  try 
  {
    if (std::string(argv[1]) == std::string("-l") && argc == 3)
      registryRPC.local(std::string(argv[2]));
    else if (std::string(argv[1]) == std::string("-d"))
    {
      if (argc == 3)
      {
        registryRPC.serve(atoi((argv[2])));
      }
      else
      {
        registryRPC.serve(0xdff);
      }
    }
    else if (std::string(argv[1]) == std::string("-c"))
    {
      if (argc == 5)
        registryRPC.connect(argv[2], argv[3], atoi(argv[4]));
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

  do generic lib for :
  rpc --root registry filepath
  rpc --root ntfs filepath
 */
}

