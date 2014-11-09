#include <iostream>

#include "rpc.hpp"
#include "server.hpp"
#include "client.hpp"
#include "dtype.hpp"



RPC::RPC()
{
  Destruct::DType::init();
}

RPC::~RPC()
{
  Destruct::DType::clean();
}

void RPC::serve(uint32_t port)
{
  TestServer  server(port);
  server.serve();
}

void RPC::connect(std::string const& addr, uint32_t port)
{
  TestClient client(addr, port);
  client.start();
}


/**
 *  TestClient
 */
TestClient::TestClient(std::string const& addr, uint32_t port) : Client(addr, port)
{
}

DObject* TestClient::start(void)
{
  Destruct::DStruct* fileS = this->remoteFind("File"); 
  if (!fileS)
    throw std::string("Directory struct not found");

  Destruct::DStruct* directoryS = this->remoteFind("Directory"); 
  if (!directoryS)
    throw std::string("Directory struct not found");

  Destruct::DStruct* vectorS = this->remoteFind("DVectorObject"); 
  if (!vectorS)
    throw std::string("Directory struct not found");

  //0 is root server object
  ClientObject* remote = new ClientObject(*this->networkStream(), this->serializeRPC(), 0, directoryS); 
  std::cout << "root name : " << remote->getValue("name").get<DUnicodeString>() << std::endl;

  std::cout << "Set remote value to 'rename-by-remote'" << std::endl;
  remote->setValue("name", RealValue<DUnicodeString>("rename-by-remote"));

  std::cout << "Root name after setValue('name') : " << remote->getValue("name").get<DUnicodeString>() << std::endl;

  this->networkStream()->write("show");
  this->networkStream()->flush();
  std::cout << "Root  path : " << remote->call("path").get<DUnicodeString>() << std::endl;

  DObject* remoteChild = remote->getValue("children").get<DObject*>();
  std::cout << "Iterating on child " << std::endl;
  DUInt64 size = remoteChild->call("size").get<DUInt64>();
 
  for (DUInt64 i = 0; i < size; ++i)
  { 
    for (DUInt64 x = 0; x < 10; ++x) 
    {
      Destruct::DObject* child = remoteChild->call("get", RealValue<DUInt64>(i)).get<DObject*>();
      std::cout <<  "child(" << i << ") : " 
              << "'" << child->getValue("name").get<DUnicodeString>() << "'"
              << " is of type : " <<  child->instanceOf()->name() 
              << std::endl;

     } break;
  }

  this->print(remote);
  std::cout << "done !" << std::endl;
  return (remote);
}

/**
 * TestServer
 */
TestServer::TestServer(uint32_t port) : Server(port)
{
}

/**
  Create Fake FS :

  Root/File1
       File2
       Directory1/File3 
*/
void            TestServer::initRoot(void)
{
  DStruct* fileStruct = makeNewDCpp<File>("File");
  DStruct* directoryStruct = makeNewDCpp<Directory>("Directory");
  Destruct::Destruct& dstruct = Destruct::Destruct::instance();

  dstruct.registerDStruct(fileStruct);
  dstruct.registerDStruct(directoryStruct);

  DObject* root = directoryStruct->newObject();
  this->objectManager().registerObject(root);
  DObject* children =  root->getValue("children").get<DObject*>();
 
  DObject* file1 = fileStruct->newObject();

  file1->setValue("name", RealValue<DUnicodeString>("File1"));
  children->call("push", RealValue<DObject*>(file1));

  File* file2 = new File(fileStruct, RealValue<DObject*>(DNone)); 
  file2->name = "File2"; 
  children->call("push", RealValue<DObject*>(file2));

  DObject* directory1 = directoryStruct->newObject();
  children->call("push", RealValue<DObject*>(directory1));
  DObject* d1children = directory1->getValue("children").get<DObject*>();
/*  
  Directory* directory1 = new Directory(directoryStruct, RealValue<DObject*>(DNone));
  directory1->name = "Directory1";
  DObject* d1children = directory1->children;
  */

  File* file3 = new File(fileStruct, RealValue<DObject*>(DNone));  
  this->objectManager().registerObject(file3);
  file3->name = "File3"; 
  d1children->call("push", RealValue<DObject*>(file3));
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
        rpc.server(atoi(argv[2]));
    }
    else if (std::string(argv[1]) == std::string("-c"))
      rpc.connect("127.0.0.1", 0xdff);
    else
      std::cout << "Launch server : -d" << std::endl
                << "Launch client : -c" << std::endl;       
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
