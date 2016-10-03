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
  Server server(port);
  server.daemonize(); //same as serv because of connection zmq loop 
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
