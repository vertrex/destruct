#include "devicerpc.hpp"

#include "server.hpp"
#include "deviceserver.hpp"

#include "client.hpp"

using namespace Destruct;

DeviceRPC::DeviceRPC()
{
  Destruct::DType::init();
  //Device::declare(); //not used as we only return a dstream yet
  Client::declare();
}

DeviceRPC::~DeviceRPC()
{
  Destruct::DType::clean();
}

void DeviceRPC::serve(uint32_t port)
{
 //use destruct import
 DeviceServer  server(port);
 server.initRoot();
 server.daemonize();
 //server.serve();
}

const std::string DeviceRPC::usage(void)
{
  return ("Usage device :\n"
          "\t-d (port) : run as server (default port 3583)\n");
}

int main(int argc, char** argv)
{
  if (argc < 2)
  {
   std::cout << DeviceRPC::usage();
   return (1); 
  }

  DeviceRPC   deviceRPC;
  try 
  {
    if (std::string(argv[1]) == std::string("-d"))
    {
      if (argc == 3)
      {
        deviceRPC.serve(atoi((argv[2])));
      }
      else
      {
        deviceRPC.serve(0xdff);
      }
    }
     else
      std::cout << DeviceRPC::usage();
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
