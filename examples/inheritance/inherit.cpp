#include <iostream>

#include "inherit.hpp"
#include "dexception.hpp"
#include "dtype.hpp"
#include "drealvalue.hpp"
#include "dsimpleobject.hpp"

#include "protocol/dmutableobject.hpp"

#include "compose.hpp"

extern "C"
{
  EXPORT void declare(void)
  {
    Inheritance::declare();
  }
}


/**
 * Test launcher
 */
Inheritance::Inheritance() : __destruct(Destruct::DStructs::instance())
{
  DType::init();
  Inheritance::declare();
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
  Destruct::DStructs&  destruct = Destruct::DStructs::instance();

  DStruct*  simpleA = new DStruct(0, "SimpleA", DSimpleObject::newObject);
  simpleA->addAttribute(Destruct::DAttribute(DType::DUnicodeStringType, "text1"));
  simpleA->addAttribute(Destruct::DAttribute(DType::DUnicodeStringType, "text2"));
  simpleA->addAttribute(Destruct::DAttribute(DType::DObjectType, "object1"));
  simpleA->addAttribute(Destruct::DAttribute(DType::DObjectType, "object2"));
  destruct.registerDStruct(simpleA);

  DStruct*  simpleB = new DStruct(simpleA, "SimpleB", DSimpleObject::newObject);
  simpleB->addAttribute(DAttribute(DType::DUnicodeStringType, "text3"));
  simpleB->addAttribute(DAttribute(DType::DObjectType, "object3"));
  destruct.registerDStruct(simpleB);

  /**
   * Cpp Object
   */
  DStruct* cppA = makeNewDCpp<CppA>("CppA");
  destruct.registerDStruct(cppA);

  /**
   *  Compose with DCppObject 
   */
  ////XXX utilise un composable par template  : probleme comment cree un composable depuis python                            
  //DStruct* cppB = new DStruct(NULL, "CppB", ComposableObject<CppB>::newObject, CppB::ownAttributeBegin(), CppB::ownAttributeEnd());
  //this->__destruct.registerDStruct(cppB);

  ////xxx cree une nouvel class en python ? ou modifier dstruct pour prendre plusieurs struct en arg ? 
  //DStruct* composeABStruct = new DComposedStruct(cppA, cppB, "ComposedAB");
  //this->__destruct.registerDStruct(composeABStruct);

  //DStruct* composeABCppSimple = new DComposedStruct(simpleA, cppB, "SimpleACppB");
  //this->__destruct.registerDStruct(composeABCppSimple);

  DStruct* cppAcppB = new DStruct(cppA, "CppACppB", DCppObject<CppB>::newObject, CppB::ownAttributeBegin(), CppB::ownAttributeEnd());
  destruct.registerDStruct(cppAcppB);

  DStruct* simpleAcppB = new DStruct(simpleA, "SimpleACppB", DCppObject<CppB>::newObject, CppB::ownAttributeBegin(), CppB::ownAttributeEnd());
  destruct.registerDStruct(simpleAcppB);
}

void Inheritance::test(void)
{
  std::cout << "==== Simple object B inherit simple object A ====" << std::endl;
  
  DObject* simpleB = this->__destruct.generate("SimpleB");

  simpleB->setValue("text1", RealValue<DUnicodeString>("text1"));
  simpleB->setValue("text2", RealValue<DUnicodeString>("text2"));
  simpleB->setValue("text3", RealValue<DUnicodeString>("text3"));

  this->show(simpleB);
  simpleB->destroy();
  //this->testComposition(simpleB);

  //std::cout << "==== ComposableObject ====" << std::endl;
  //DObject* composition = this->__destruct.generate("ComposedAB"); 
  //this->testComposition(composition);
  
  //std::cout << "==== ComposableObject Simple Cpp ====" << std::endl;
  //composition = this->__destruct.generate("SimpleACppB"); 
  //this->testComposition(composition);

  std::cout << "==== Inherit CppACppB ====" << std::endl;
  DObject* composition = this->__destruct.generate("CppACppB");
  this->testComposition(composition);
  composition->destroy();
  std::cout << "==== Inherit SimpleACppB ===" << std::endl;
  composition = this->__destruct.generate("SimpleACppB");
 
  this->testComposition(composition);
  composition->destroy();
}

void           Inheritance::testComposition(DObject* composition) const
{
  std::cout << "==== Test composition " << composition->instanceOf()->name() << " ====" << std::endl;
  std::cout << "set value: " << std::endl;
  composition->setValue("text1", RealValue<DUnicodeString>("text 1"));
  composition->setValue("text3", RealValue<DUnicodeString>("text 3"));
  composition->setValue("text2", RealValue<DUnicodeString>("text 2"));

  std::cout << "show composition: " << std::endl; 
  this->show(composition); 

  std::cout << "Call changeA() " << std::endl;
  composition->call("changeA");
 
  try
  {
    std::cout << "Call callA() " << std::endl;
    composition->call("callA");
    this->show(composition); 
  }
  catch (DException const& exception)
  {
    std::cout << "Test composition Error : " << exception.error() << std::endl;
  }
  //std::cout << "Get non existent value" << std::endl;
  //composition->getValue("tintin");
  std::cout << "====  End ===" << std::endl;
}

void           Inheritance::show(DStruct* dstruct) const
{
  std::cout << "Show struct : " << std::endl 
            << "{" << dstruct->name() << std::endl;

  int32_t count = (int32_t)dstruct->attributeCount();
  for (int32_t index = 0; index < count; ++index)
  {
    DAttribute attribute = dstruct->attribute(index);
    std::cout << attribute.name() << "\t: " << DType(attribute.type().getType()).name() <<std::endl;
  }
}

void           Inheritance::show(DObject* object) const
{
  DObject* cout = this->__destruct.generate("DStreamCout");
  DObject* serializer = this->__destruct.generate("SerializeText", RealValue<DObject*>(cout));
  serializer->call("DObject", RealValue<DObject*>(object));
  serializer->destroy();
  cout->destroy();
}
