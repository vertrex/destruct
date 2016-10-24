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
  DObject* arg = DStructs::instance().generate("ClientArgument");
  arg->setValue("port", RealValue<DUInt32>(port));
  
  DObject* auth = DStructs::instance().generate("RPCAuth");
  auth->setValue("cert", RealValue<DUnicodeString>("cert\\rpczmq_cert.txt")); //XXX default value pass as argument or default XXX
  auth->setValue("clientCert", RealValue<DUnicodeString>("cert\\rpczmq_client_cert.txt"));
  arg->setValue("auth", RealValue<DObject*>(auth)); 

  Server server = Server(RealValue<DObject*>(arg));
  server.daemonize(); //zmq loop anyway
  
}

int main(int argc, char** argv)
{
  if (argc < 2)
  {
    std::cout << "Launch server : (port) " << std::endl;
	return (1);
  }
  RPC   rpc;
  
  try 
  {
    if (std::string(argv[1]) == std::string("-d"))// -p port -c cert ...
    {
      if (argc == 2)
        rpc.serve(0xdff);
      else
        rpc.serve(atoi(argv[2]));
    }
    else
      std::cout << "Launch server : (port) " << std::endl;
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
