#include <iostream>

#include "inherit.hpp"
#include "dexception.hpp"
#include "dtype.hpp"
#include "drealvalue.hpp"
#include "dsimpleobject.hpp"

#include "protocol/dstream.hpp"
#include "protocol/dserialize.hpp"
#include "protocol/dmutableobject.hpp"

#include "compose.hpp"

/**
 * Test launcher
 */
Inheritance::Inheritance() : __destruct(Destruct::Destruct::instance())
{
  DType::init();
  this->declare();
}

Inheritance::~Inheritance()
{
  DType::clean();
}

void  Inheritance::declare(void)
{
  /**
   * Simple Object
   */
  DStruct*  simpleA = new DStruct(0, "SimpleA", DSimpleObject::newObject);
  simpleA->addAttribute(Destruct::DAttribute(DType::DUnicodeStringType, "text1"));
  simpleA->addAttribute(Destruct::DAttribute(DType::DUnicodeStringType, "text2"));
  simpleA->addAttribute(Destruct::DAttribute(DType::DObjectType, "object1"));
  simpleA->addAttribute(Destruct::DAttribute(DType::DObjectType, "object2"));
  this->__destruct.registerDStruct(simpleA);

  DStruct*  simpleB = new DStruct(simpleA, "SimpleB", DSimpleObject::newObject);
  simpleB->addAttribute(DAttribute(DType::DUnicodeStringType, "text3"));
  simpleB->addAttribute(DAttribute(DType::DObjectType, "object3"));
  this->__destruct.registerDStruct(simpleB);

  /**
   * Cpp Object
   */
  DStruct* cppA = makeNewDCpp<CppA>("CppA");
  this->__destruct.registerDStruct(cppA);

  DStruct* cppB = makeNewDCpp<CppB>("CppB");
  this->__destruct.registerDStruct(cppB);

  /**
   *  Compose with DCppObject 
   */
  //XXX utilise un composable par template  : probleme comment cree un composable depuis python                            
  DStruct* composableBStruct = new DStruct(NULL, "ComposeB", ComposableObject<CppB>::newObject, CppB::ownAttributeBegin(), CppB::ownAttributeEnd());
  this->__destruct.registerDStruct(composableBStruct);


  DStruct* composeABStruct = new DComposedStruct(cppA, composableBStruct, "ComposedAB");
  this->__destruct.registerDStruct(composeABStruct);

  //Cmpose with simple object

  DStruct* composeABCppSimple = new DComposedStruct(simpleA, composableBStruct, "SimpleACppB");
  this->__destruct.registerDStruct(composeABCppSimple);

}

void Inheritance::test(void)
{
  std::cout << "==== Simple object B inherit simple object A ====" << std::endl;
  
  DObject* simpleB = this->__destruct.generate("SimpleB");

  simpleB->setValue("text1", RealValue<DUnicodeString>("Set in simpleB"));
  simpleB->setValue("text2", RealValue<DUnicodeString>("Set in simpleB"));
  simpleB->setValue("text3", RealValue<DUnicodeString>("Set in simpleB"));

  this->show(simpleB);

  std::cout << "==== ComposableObject ====" << std::endl;

  DObject* composition = this->__destruct.generate("ComposedAB"); 

  //test 
  std::cout << "set value: " << std::endl;
  composition->setValue("text1", RealValue<DUnicodeString>("text 1"));
  composition->setValue("text3", RealValue<DUnicodeString>("text 3"));
  composition->setValue("text2", RealValue<DUnicodeString>("text 2"));

  std::cout << "show composedAB: " << std::endl; 
  this->show(composition); 

  std::cout << "Call changeA() " << std::endl;
  composition->call("changeA");
  std::cout << "Call callA() " << std::endl;
  composition->call("callA");
  this->show(composition); 

  
  std::cout << "==== ComposableObject Simple Cpp ====" << std::endl;

  composition = this->__destruct.generate("SimpleACppB"); 

  //test 
  composition->setValue("text1", RealValue<DUnicodeString>("text 1"));
  composition->setValue("text3", RealValue<DUnicodeString>("text 3"));
  composition->setValue("text2", RealValue<DUnicodeString>("text 2"));

  std::cout << "show simpleACppB : " << std::endl; 
  this->show(composition); 

  std::cout << "Call changeA() " << std::endl;
  composition->call("changeA");
  this->show(composition); 

  //std::cout << "Get non existent value" << std::endl;
  //composition->getValue("tintin");
}

void           Inheritance::show(Destruct::DObject* object) const
{
  DStream* cout = static_cast<DStream*>(this->__destruct.generate("DStreamCout"));
  DSerializers::to("Text")->serialize(*cout, object);
}

int     main(int argc, char** argv)
{
  Inheritance ih;
 
  try 
  {
    ih.test();
  }
  catch (DException const& exception)
  {
    std::cout << "Error : " << std::endl << exception.error() << std::endl; 
  }
  catch (const std::string& error)
  {
     std::cout << "Error : " << std::endl << error << std::endl;
  }

  return (0);
}
