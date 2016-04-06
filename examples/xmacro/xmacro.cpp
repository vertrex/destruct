#include "xmacro.hpp"

Test::Test(DStruct* dstruct, DValue const& args) : DCppObject<Test>(dstruct, args)
{
  this->init();
}

DUnicodeString Test::testFunc(DValue const& args)
{
  return (this->name + " ret by func");
}

int main(int argc, char** argv)
{
  DStruct* testS = makeNewDCpp<Test>("Test");
  DObject* test = testS->newObject();

  test->setValue("name", RealValue<DUnicodeString>("setted"));
  std::cout << test->call("testFunc").get<DUnicodeString>() << std::endl;
}

/*

#define ATTRIBUTES_TABLE \
  X(DUnicodeString, name)\
  X(DObject, values)\

#define METHOD_TABLE\
  Y(DUnicodeString, testFunc, DUnicodeString)


#define X(a, b) Destruct::DAttribute(Destruct::DType::a##Type, #b), 
#define Y(a, b, c) Destruct::DAttribute(Destruct::DType::a##Type, #b, Destruct::DType::c##Type), 
static Destruct::DAttribute* ownAttributeBegin()
{
  static Destruct::DAttribute  attributes[] =
  {
    ATTRIBUTES_TABLE      
    METHOD_TABLE 
  };
  return (attributes);
}
#undef X
#undef Y

#define X(a, b) Destruct::DPointer<Test>(&Test::b),
#define Y(a, b, c) Destruct::DPointer<Test>(&Test::_##b, &Test::b) 
  static Destruct::DPointer<Test>* memberBegin()
  {
    static Destruct::DPointer<Test> memberPointer[] = 
    {
      ATTRIBUTES_TABLE  
      METHOD_TABLE
    };
    return (memberPointer);
  }
#undef X
#undef Y

*/
