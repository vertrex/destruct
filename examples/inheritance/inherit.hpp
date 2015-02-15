#ifndef __INHERIT__
#define __INHERIT__

#include <stdint.h>

#include "dstructs.hpp"
#include "dobject.hpp"
#include "dunicodestring.hpp"
#include "drealvalue.hpp"
#include "protocol/dcppobject.hpp"

using namespace Destruct;

class CppA : public DCppObject<CppA>
{
public:
  CppA(DStruct* dstruct, DValue const& args) : DCppObject<CppA>(dstruct, args)
  {
    this->init();
  }  
      
  RealValue<DUnicodeString>       text1, text2;
  RealValue<DObject*>             object1, object2;
  RealValue<DFunctionObject* >    _callable1;

  void  callable1(void)
  {
    std::cout << "CppA callable1() called " << std::endl;
    this->text2  = "Changed by callable 1";
  }

  static size_t ownAttributeCount()
  {
    return (5);
  }

  static DAttribute* ownAttributeBegin()
  {
    static DAttribute attributes[] = 
    {
      DAttribute(DType::DUnicodeStringType, "text1"),
      DAttribute(DType::DUnicodeStringType, "text2"),
      DAttribute(DType::DObjectType, "object1"),
      DAttribute(DType::DObjectType, "object2"),
      DAttribute(DType::DNoneType, "callable1", DType::DNoneType), 
    };

    return (attributes);
  }

  static DPointer<CppA>* memberBegin()
  {
     static DPointer<CppA> memberPointer[] =
     {
       DPointer<CppA>(&CppA::text1),
       DPointer<CppA>(&CppA::text2),
       DPointer<CppA>(&CppA::object1),
       DPointer<CppA>(&CppA::object2),
       DPointer<CppA>(&CppA::_callable1, &CppA::callable1),
     };

    return (memberPointer);
  } 

  static DAttribute*  ownAttributeEnd()
  {
    return (ownAttributeBegin() + ownAttributeCount());
  }

  static DPointer<CppA>* memberEnd()
  {
    return (memberBegin() + ownAttributeCount());
  }
};

class CppB : public DCppObject<CppB>//public CppA
{
public:
  CppB(DStruct* dstruct, DValue const& args):  DCppObject<CppB>(dstruct, args)
  {
    this->init();
  }
 
  RealValue<DUnicodeString>     text3;
  RealValue<DObject*>           object3;
  RealValue<DFunctionObject*>   _changeA, _callA;

  void   changeA(void)
  {
    std::cout << "changeA(void) getValue text 1 : " <<  this->getValue("text1").asUnicodeString() << std::endl;
    this->setValue("text1", RealValue<DUnicodeString>("changed by changeA"));
  }

  void callA(void)
  {
    this->call("callable1");
  }

  static size_t ownAttributeCount()
  {
    return (4);
  }

  static DAttribute* ownAttributeBegin()
  {
    static DAttribute attributes[] = 
    {
      DAttribute(DType::DUnicodeStringType, "text3"),
      DAttribute(DType::DObjectType, "object3"),
      DAttribute(DType::DNoneType, "changeA", DType::DNoneType),
      DAttribute(DType::DNoneType, "callA", DType::DNoneType),
    };

    return (attributes);
  }

  static DAttribute*  ownAttributeEnd()
  {
    return (ownAttributeBegin() + ownAttributeCount());
  }

  static DPointer<CppB>* memberBegin()
  {
    static DPointer<CppB> memberPointer[] =
    {
      DPointer<CppB>(&CppB::text3),
      DPointer<CppB>(&CppB::object3),
      DPointer<CppB>(&CppB::_changeA, &CppB::changeA),
      DPointer<CppB>(&CppB::_callA, &CppB::callA),
    };

    return (memberPointer);
  } 

  static DPointer<CppB>* memberEnd()
  {
    return (memberBegin() + ownAttributeCount());
  }
};

/**
 * Main
 */
class Inheritance
{
public:
  Inheritance();
  ~Inheritance();

  static void  declare(void);
  void  test(void);
  void  show(DObject* object) const;
  void  show(DStruct* dstruct) const;
  void  testComposition(DObject* object) const;
private:
  Destruct::DStructs&   __destruct;
};

#endif
