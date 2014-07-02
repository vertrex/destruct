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

void RPC::serve(void)
{
  Server  server;
  server.serve();
}

void RPC::connect(std::string const& addr, uint32_t port)
{
  Client client(addr, port);
  client.start();
}

int main(int argc, char** argv)
{
  if (argc < 2)
    return (1);

  RPC   rpc;
  
  try 
  {
    if (std::string(argv[1]) == std::string("-d"))
      rpc.serve();
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
