/*
 * DFF -- An Open Source Digital Forensics Framework
 * Copyright (C) 2009-2011 ArxSys
 * This program is free software, distributed under the terms of
 * the GNU General Public License Version 2. See the LICENSE file
 * at the top of the source tree.
 *  
 * See http: *www.digital-forensic.org for more information about this
 * project. Please do not directly contact any of the maintainers of
 * DFF for assistance; the project provides a web site, mailing lists
 * and IRC channels for your use.
 * 
 * Author(s):
 *  Solal Jacob <sja@digital-forensic.org>
 */

#include "perf.hpp"
#include "performance.hpp"
#include <ctime>

using namespace Destruct;

Perf::Perf()
{
  Destruct::DType::init();
}

Perf::~Perf()
{
  Destruct::DType::clean();
}

DObject*  Perf::local(void)
{
  Destruct::DStructs& destruct = Destruct::DStructs::instance();
  DObject* performance = destruct.generate("Performance");

  return (performance);
}

//ret object and use same code as local
DObject*  Perf::connect(std::string const& addr, uint32_t port)
{
  //use destruct import 
  DObject* loader = DStructs::instance().generate("Import");
  if (loader->call("file", RealValue<DUnicodeString>("../modules/libdestruct_rpczmq.so")).get<DUInt8>() == 0)
  {
    loader->call("file", RealValue<DUnicodeString>("destruct_rpczmq.dll"));
  }

  DObject* argument = DStructs::instance().generate("ClientArgument");

  argument->setValue("address", RealValue<DUnicodeString>(addr));
  argument->setValue("port", RealValue<DUInt32>(port));

  std::cout << "Connecting to " << addr << ":" << port << std::endl;
  DObject*  client = DStructs::instance().generate("Client", RealValue<DObject*>(argument));
  std::cout << "Connected" << std::endl;

  DObject* serverLoader = client->call("generate", RealValue<DUnicodeString>("Import"));
  
  if (serverLoader->call("file", RealValue<DUnicodeString>("../modules/libdestruct_performance.so")).get<DUInt8>() == 0)
    serverLoader->call("file", RealValue<DUnicodeString>("destruct_performance.dll"));
  DObject* performance = client->call("generate", RealValue<DUnicodeString>("Performance"));

  return (performance);
}

const std::string Perf::usage(void) const
{
  return ("Usage perf :\n"
          "\t-l launch test locally \n"
          "\t-c address port : connect and process test on server\n");
}

void    Perf::stats(DObject* perf)
{
  //clock_t begin = std::clock();

  //DUInt64 ncall = 100000;
  //for (DUInt64 i = 0; i < ncall; ++i)
  //{
    //DUnicodeString result = perf->call("callStr", RealValue<DUnicodeString>("Test"));
  //}
  //clock_t end = std::clock();
  //double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
  //std::cout << "Call DUnicodestring callStr('test') " << ncall << " times in " << elapsed_secs << std::endl;
  //std::cout << std::fixed << ncall / elapsed_secs << " call by seconds" << std::endl;

  clock_t begin = std::clock();

  DUInt64 ncall = 100000;
  for (DUInt64 i = 0; i < ncall; ++i)
  {
    DBuffer result = perf->call("getBuffer", RealValue<DInt32>(8192));
  }
  clock_t end = std::clock();
  double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
  std::cout << "Call DUnicodestring callStr('test') " << ncall << " times in " << elapsed_secs << std::endl;
  std::cout << std::fixed << ncall / elapsed_secs << " call by seconds" << std::endl;
}

int main(int argc, char** argv)
{
  Perf          perf;

  if (argc < 2)
  {
   std::cout << perf.usage();
   return (1); 
  }

  DObject*      performance;

  try 
  {
    DObject* loader = DStructs::instance().generate("Import");
    if (loader->call("file", RealValue<DUnicodeString>("../modules/libdestruct_performance.so")).get<DUInt8>() == 0)
      loader->call("file", RealValue<DUnicodeString>("destruct_performance.dll"));

    if (std::string(argv[1]) == std::string("-l"))
      performance = perf.local();
    else if (std::string(argv[1]) == std::string("-c"))
    {
      if (argc == 4)
        performance = perf.connect(argv[2], atoi(argv[3]));
      else
      {
        std::cout << perf.usage();
        return 1;
      }
    }
    else
    {
      std::cout << perf.usage();
      return 1;
    }
    perf.stats(performance);
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
