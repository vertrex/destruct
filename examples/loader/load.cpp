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
  //std::string filePath = "/home/vertrex/destruct/examples/dtest/libdestruct_test.so";
  //loader.loadFile(filePath);
  std::string filePath = "/home/vertrex/destruct/examples/registry/libregistry.so";
  loader.loadFile(filePath);
  filePath = "/home/vertrex/destruct/examples/threading/libdthreading.so";
  loader.loadFile(filePath);
  filePath = "/home/vertrex/destruct/examples/inheritance/libdestruct_inherit.so";
  loader.loadFile(filePath);
  filePath = "/home/vertrex/destruct/examples/rpc/libdestruct_rpc.so";
  loader.loadFile(filePath);
#else
  //XXX load other lib 
  //std::string filePath = "C:\\Users\\Solal\\destruct-build\\examples\\dtest\\Release\\destruct_test.dll";
  //loader.loadFile(filePath);
  std::string filePath = "C:\\Users\\Solal\\destruct-build\\examples\\threading\\Release\\dthreading.dll";
  loader.loadFile(filePath);
  //filePath = "/home/vertrex/destruct/examples/registry/libregistry.so";
  //loader.loadFile(filePath);
#endif
  }
  catch (Destruct::DException const& exception)
  {
    std::cout << exception.error() << std::endl;
  }

  Destruct::DType::clean();
}
