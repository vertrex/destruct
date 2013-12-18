#include "dtest/destruct_test.hpp"
#include "dtype.hpp"

int main(int argc, char **argv)
{
  Destruct::DType::init();
  Destruct::DestructTest destructTest = Destruct::DestructTest(true);
  destructTest.run();
  Destruct::DType::clean();
  //Destruct::DObject* iter = destructTest.getObjectValue();
  //for (unsigned int x = 0; x < 1000000; ++x)
  //{
  //Destruct::DValue s = iter->call("get", Destruct::RealValue<DInt32>(0));
  //std::cout << s.asUnicodeString() << std::endl;
  //}
}
