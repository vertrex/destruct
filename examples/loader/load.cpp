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
#ifndef WIN32
  std::string filePath = "/home/vertrex/destruct/examples/dtest/libdestruct_test.so";
  loader.loadFile(filePath);
  filePath = "/home/vertrex/destruct/examples/registry/libregistry.so";
  loader.loadFile(filePath);
  filePath = "/home/vertrex/destruct/examples/threading/libdthreading.so";
  loader.loadFile(filePath);
#else 
  std::string filePath = "C:\\Users\\Solal\\destruct-build\\examples\\dtest\\Release\\destruct_test.dll";
  loader.loadFile(filePath);
  filePath = "C:\\Users\\Solal\\destruct-build\\examples\\threading\\Release\\dthreading.dll";
  loader.loadFile(filePath);
  //filePath = "/home/vertrex/destruct/examples/registry/libregistry.so";
  //loader.loadFile(filePath);
#endif
  }
  catch (Destruct::DException const& exception)
  {
    std::cout << exception.error() << std::endl;
  }
}
