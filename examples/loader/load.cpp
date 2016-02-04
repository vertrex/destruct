#include "dtype.hpp"
#include "dexception.hpp"

#include "load.hpp"

int main(int argc, char **argv)
{
  std::cout << "loading directory " << "destruct/examples" << std::endl;


  Destruct::DType::init();
  Loader loader = Loader();

  try
  {
//XXX leaking path name :) must recurse a dir in argv
#ifndef WIN32
  //loader.loadFile(filePath);
  std::string filePath = "../modules/libregistry.so";
  loader.loadFile(filePath);
  filePath = "../modules/libdthreading.so";
  loader.loadFile(filePath);
  filePath = "../modules/libdestruct_inherit.so";
  loader.loadFile(filePath);
  filePath = "../modules/libdestruct_rpc.so";
  loader.loadFile(filePath);
#else
  //XXX load other lib 
  std::string filePath = "lib\\dthreading.dll";
  loader.loadFile(filePath);
#endif
  }
  catch (Destruct::DException const& exception)
  {
    std::cout << exception.error() << std::endl;
  }

  Destruct::DType::clean();
}
