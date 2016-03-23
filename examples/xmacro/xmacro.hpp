#include "dstructs.hpp"
#include "destruct.hpp"
#include "protocol/dcppobject.hpp"

using namespace Destruct;

/**
 *

        Whish something like

        Test -> class ref

        DInt32, size
        DUnicodeString, testFunc, DUnicodeString

#define ATTRIBUTES_TABLE\
  X(size, DInt32)
  X(DUnicodeString, testFunc, DUnicodeString)

#define CLASS_NAME
  Y(Test)



 **/
 


class Test : public DCppObject<Test>
{
public:
  Test(DStruct* dstruct, DValue const& args);

  #define ATTRIBUTES_TABLE \
    X(DUnicodeString, name)\
    X(DObject, values)\

  #define METHOD_TABLE\
    Y(DUnicodeString, testFunc, DUnicodeString)

  #include "xmacro.def"

  ATTRIBUTE_COUNT(3)



/*
  RealValue<DUnicodeString>     name;
  RealValue<DObject*>           values;
  RealValue<DFunctionObject*>   _testFunc;
  
  DUnicodeString      testFunc(DValue const& args);


  static Destruct::DAttribute* ownAttributeBegin()
  {
    static Destruct::DAttribute  attributes[] = 
    {
       Destruct::DAttribute(Destruct::DType::DUnicodeStringType, "name"),
       Destruct::DAttribute(Destruct::DType::DObjectType, "values"),

       Destruct::DAttribute(Destruct::DType::DUnicodeStringType, "testFunc", Destruct::DType::DUnicodeStringType),
    };
    return (attributes);
  }
*/

/*
  static Destruct::DPointer<Test>* memberBegin()
  {
    static Destruct::DPointer<Test> memberPointer[] = 
    {
      Destruct::DPointer<Test>(&Test::name),
      Destruct::DPointer<Test>(&Test::values),
      Destruct::DPointer<Test>(&Test::_testFunc, &Test::testFunc),
    };
    return (memberPointer);
  }
*/
};
