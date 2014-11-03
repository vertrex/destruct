#include "registryrpc.hpp"
#include "registry.hpp"
//#include <iostream>

//#include "rpc.hpp"
#include "server.hpp"
#include "client.hpp"
#include "registryserver.hpp"
#include "registryclient.hpp"
//#include "dtype.hpp"

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
  Destruct::Destruct& destruct = Destruct::Destruct::instance();
  DObject* registry = destruct.generate("Registry");

  std::cout << "opening file " << filePath << std::endl;

  DObject* regf = registry->call("open", RealValue<DUnicodeString>(filePath)).get<DObject*>();

  std::cout << "Regf deserialized " << std::endl;

  std::string fileName(filePath, filePath.rfind("/") + 1);
  Registry::toFile(fileName + ".bin", regf, "Binary");
  Registry::toFile(fileName + "registry.text", regf, "Text");
  //Registry::show(regf); 
}


void RegistryRPC::serve(void)
{
 RegistryServer  server;
 server.serve();
}

void RegistryRPC::connect(std::string const& addr, uint32_t port)
{
  RegistryClient client(addr, port);
  DObject* registry = client.start();

  std::cout << "root object type " << registry->instanceOf()->name() << std::endl;
 
  DValue regfv = registry->call("open", RealValue<DUnicodeString>("/home/vertrex/dump/registry/system")); 
  DObject* regf = regfv.get<DObject*>();

  //std::cout << "registry file name : " << regf->call("name").asUnicodeString() << std::endl;
  std::cout    << "              version : " << regf->getValue("major").asUnicodeString() << std::endl;
          //<< "." << regf->call("minor").asUnicodeString() << std::endl;
  DObject* rootKey = regf->getValue("key").get<DObject*>();
 
  std::cout << "rootKey type : " << rootKey->instanceOf()->name() << std::endl;
  client.printKey(rootKey);

  std::cout << "finish !" << std::endl; 
}

const std::string RegistryRPC::usage(void)
{
  return ("Usage registry :\n"
          "\t-l file : read registry file\n"
          "\t-d : run server\n"
          "\t-c file : connect and process file on server\n");
}

int main(int argc, char** argv)
{
  if (argc < 2)
  {
   std::cout << RegistryRPC::usage();
   return (1); 
  }

  RegistryRPC   registryRPC;

//if --local args 
  try 
  {
    if (std::string(argv[1]) == std::string("-l"))
      registryRPC.local(std::string(argv[2]));
    else if (std::string(argv[1]) == std::string("-d"))
      registryRPC.serve();
    else if (std::string(argv[1]) == std::string("-c"))
      registryRPC.connect("127.0.0.1", 0xdff);
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

  apres fait une lib avec juste registry 
  et un prog rpc generique 

  juset a dire rpc --root registry filepath
               rpc --root ntfs filepath
  cree l object et le deserialize 
 */
}

