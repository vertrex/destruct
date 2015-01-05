#include "dtype.hpp"

#include "load.hpp"

int main(int argc, char **argv)
{
  std::cout << "loading directory " << "destruct/examples" << std::endl;


  Destruct::DType::init();
  Loader loader = Loader();

  std::string filePath = "/home/vertrex/destruct/examples/dtest/libdestruct_test.so";
  loader.loadFile(filePath);
  filePath = "/home/vertrex/destruct/examples/registry/libregistry.so";
  loader.loadFile(filePath);
  filePath = "/home/vertrex/destruct/examples/threading/libdthreading.so";
  loader.loadFile(filePath);
}
