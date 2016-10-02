#include <iostream>

#include "rpc.hpp"
#include "server.hpp"
#include "client.hpp"
#include "dtype.hpp"

RPC::RPC()
{
  Destruct::DType::init();
  Client::declare();
}

RPC::~RPC()
{
  Destruct::DType::clean();
}

void RPC::serve(uint32_t port)
{
  std::cout << "Create TesterServer" << std::endl;
  TestServer  server(port);
  DObject* root = server.initRoot(); 
  server.setRoot(root);
  std::cout << "Daemonize server " << std::endl;
//  server.daemonize();
  server.serve();
}

void RPC::connect(std::string const& addr, uint32_t port)
{
  TestClient client(addr, port);
  DObject* root = client.start();
  root->destroy();
}

/**
 *  TestClient
 */
TestClient::TestClient(std::string const& addr, uint32_t port) : Client(addr, port)
{
}

DObject* TestClient::start(void)
{
  Destruct::DStruct* fileS = this->find(RealValue<DUnicodeString>("File")); 
  if (!fileS)
    throw DException("TestClient::start Directory struct not found");

  Destruct::DStruct* directoryS = this->find(RealValue<DUnicodeString>("Directory")); 
  if (!directoryS)
    throw DException("TestClient::start Directory struct not found");

  Destruct::DStruct* vectorS = this->find(RealValue<DUnicodeString>("DVectorObject")); 
  if (!vectorS)
    throw DException("TestClient::start Directory struct not found");

  //0 is root server object
  ClientObject* remote = new ClientObject(RealValue<DObject*>(this->networkStream()), RealValue<DObject*>(this->serializeRPC()), RealValue<DObject*>(this->deserializeRPC()),  0, directoryS); 
  std::cout << "root name : " << remote->getValue("name").get<DUnicodeString>() << std::endl;

  std::cout << "Set remote value to 'rename-by-remote'" << std::endl;
  remote->setValue("name", RealValue<DUnicodeString>("rename-by-remote"));

  std::cout << "Root name after setValue('name') : " << remote->getValue("name").get<DUnicodeString>() << std::endl;

  this->serializeRPC()->call("DUnicodeString", RealValue<DUnicodeString>("show"));
  this->networkStream()->call("flush");
  std::cout << "Root  path : " << remote->call("path").get<DUnicodeString>() << std::endl;

  DObject* remoteChild = remote->getValue("children");
  std::cout << "Iterating on child " << remoteChild->instanceOf()->name() << std::endl;
  DUInt64 size = remoteChild->call("size");

  std::cout << "get children size " << size << std::endl; 
  //for (DUInt64 i = 0; i < size; ++i)
  //{ 
    DUInt64 i = 0;
    for (DUInt64 x = 0; x < 10; ++x) 
    {
      Destruct::DObject* child = remoteChild->call("get", RealValue<DUInt64>(i));
      std::cout <<  "child(" << i << ") : " 
              << "'" << child->getValue("name").get<DUnicodeString>() << "'"
              << " is of type : " <<  child->instanceOf()->name() 
              << std::endl;
      child->destroy();
     }// break;
  //}

  remoteChild->destroy();
  this->print(remote);
  std::cout << "done !" << std::endl;
  return (remote);
}

/**
 * TestServer
 */
TestServer::TestServer(uint32_t port) : Server(port)
{
  TestServer::declare();
}

/**
  Create Fake FS :

  Root/File1
       File2
       Directory1/File3 
*/
void            TestServer::declare(void)
{
  DStruct* fileStruct = makeNewDCpp<File>("File");
  DStruct* directoryStruct = makeNewDCpp<Directory>("Directory");
  Destruct::DStructs& dstruct = Destruct::DStructs::instance();

  dstruct.registerDStruct(fileStruct);
  dstruct.registerDStruct(directoryStruct);
}

DObject*        TestServer::initRoot(void)
{
  Destruct::DStructs& dstruct = Destruct::DStructs::instance();
  DStruct* fileStruct = dstruct.find("File");
  DStruct* directoryStruct = dstruct.find("Directory");

  DObject* root = directoryStruct->newObject();

  DObject* children = root->getValue("children");
 
  DObject* file1 = fileStruct->newObject();

  file1->setValue("name", RealValue<DUnicodeString>("File1"));
  children->call("push", RealValue<DObject*>(file1));
  file1->destroy();

  File* file2 = new File(fileStruct, RealValue<DObject*>(DNone)); 
  file2->name = "File2"; 
  children->call("push", RealValue<DObject*>(file2));
  file2->destroy();

  DObject* directory1 = directoryStruct->newObject();
  children->call("push", RealValue<DObject*>(directory1));
  DObject* d1children = directory1->getValue("children");
  directory1->destroy();
/*  
  Directory* directory1 = new Directory(directoryStruct, RealValue<DObject*>(DNone));
  directory1->name = "Directory1";
  DObject* d1children = directory1->children;
  */

  File* file3 = new File(fileStruct, RealValue<DObject*>(DNone));  
  file3->name = "File3"; 
  d1children->call("push", RealValue<DObject*>(file3));
  d1children->destroy();
  file3->destroy();
  return (root);
}


int main(int argc, char** argv)
{
  if (argc < 2)
    return (1);
  RPC   rpc;
  
  try 
  {
    if (std::string(argv[1]) == std::string("-d"))
    {
      if (argc == 2)
        rpc.serve(0xdff);
      else
        rpc.serve(atoi(argv[2]));
    }
    else if (std::string(argv[1]) == std::string("-c"))
      rpc.connect(std::string(argv[2]), 0xdff);
    else
    {
      std::cout << "Launch server : -d" << std::endl
                << "Launch client : -c" << std::endl;       
    }
  }
  catch (Destruct::DException const& exception)
  {
    std::cout << "Error : " << std::endl << exception.error() << std::endl; 
  }
  catch (const std::string& error)
  {
     std::cout << "Error : " << std::endl << error << std::endl;
  }
}
