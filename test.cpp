#include "destruct_test.hpp"
#include "dtype.hpp"

int main(int argc, char **argv)
{
  Destruct::DType::init();
  Destruct::DestructTest destructTest = Destruct::DestructTest(true);
  destructTest.run();
  Destruct::DType::clean();
}
