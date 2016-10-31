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
  
  Server server = Server(RealValue<DObject*>(arg));
  server.daemonize();
}

void RPC::serve(uint32_t port, std::string cert, std::string clientCert)
{
  DObject* arg = DStructs::instance().generate("ClientArgument");
  arg->setValue("port", RealValue<DUInt32>(port));

  DObject* auth = DStructs::instance().generate("RPCAuth");
  auth->setValue("cert", RealValue<DUnicodeString>(cert));  //"cert\\rpczmq_cert.txt"
  auth->setValue("clientCert", RealValue<DUnicodeString>(clientCert)); //"cert\\rpczmq_client_cert.txt"
  arg->setValue("auth", RealValue<DObject*>(auth));

  Server server = Server(RealValue<DObject*>(arg));
  server.daemonize();
}

int usage(void)
{
  std::cout << "rpczmq:" << std::endl
            << "\t-h\t show usage" << std::endl
            << "\t-p\t port_number" << std::endl
            << "\t-a\t certificate_path,client_public_certificate_path" << std::endl
	    << "\teg: -p 3583 -a cert\\rpczmq_cert.txt,cert\\rpczmq_client_cert.txt" << std::endl;

  return (1);
}

int main(int argc, char** argv)
{
  RPC   rpc;
  uint32_t port = 0xdff;
  bool auth = false;
  std::string certificatePath = "";
  std::string clientCertificatePath = "";

  if (argc == 2)
    return (usage());
  if (argc >= 3)
  {
    if (std::string(argv[1]) == std::string("-p"))
    {
      port = atoi(argv[2]);
    }
    else if (std::string(argv[1]) == std::string("-a"))
    {
      std::string authtuple = std::string(argv[2]);
      std::size_t split = authtuple.find(",");
      if (split != std::string::npos)
      {
        auth = true;
        certificatePath = authtuple.substr(0, split);
        clientCertificatePath = authtuple.substr(split + 1);
      }
    }
  }
  if (argc >= 5)
  {
    if (std::string(argv[3]) == std::string("-p"))
    {
      port = atoi(argv[4]);
    }
    else if (std::string(argv[3]) == std::string("-a"))
    {
      std::string authtuple = std::string(argv[4]);
      std::size_t split = authtuple.find(",");
      if (split != std::string::npos)
      {
        auth = true;
        certificatePath = authtuple.substr(0, split);
        clientCertificatePath = authtuple.substr(split + 1);
      }
    }
  }

  try 
  {
    if (auth == false)
    {
      std::cout << "Launching server on port " << port << " without authentification" << std::endl;
      rpc.serve(port);
    }
    else
    {
      std::cout << "Launching server on port " << port << std::endl
                << "Authentification activated with key " << certificatePath << std::endl
                << "Authorizing client with public key " << clientCertificatePath << std::endl;
      rpc.serve(port, certificatePath, clientCertificatePath);
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
