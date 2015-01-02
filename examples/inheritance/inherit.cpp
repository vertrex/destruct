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
  DStruct*  simpleA = new DStruct(0, "SimpleA", DSimpleObject::newObject);
  simpleA->addAttribute(Destruct::DAttribute(DType::DUnicodeStringType, "text"));
  simpleA->addAttribute(Destruct::DAttribute(DType::DObjectType, "object"));
  this->__destruct.registerDStruct(simpleA);

  DStruct*  simpleB = new DStruct(simpleA, "SimpleB", DSimpleObject::newObject);
  simpleB->addAttribute(DAttribute(DType::DUnicodeStringType, "text2"));
  simpleB->addAttribute(DAttribute(DType::DObjectType, "object2"));
  simpleB->addAttribute(DAttribute(DType::DUnicodeStringType, "text"));
  this->__destruct.registerDStruct(simpleB);

  DStruct*  simpleC = new Destruct::DStruct(simpleB, "SimpleC", DSimpleObject::newObject);
  simpleC->addAttribute(DAttribute(DType::DUnicodeStringType, "text3"));
  simpleC->addAttribute(DAttribute(DType::DObjectType, "object3"));
  simpleC->addAttribute(DAttribute(DType::DUnicodeStringType, "text"));
  simpleC->addAttribute(DAttribute(DType::DObjectType, "object2"));
  this->__destruct.registerDStruct(simpleC);

  DStruct* cppA = makeNewDCpp<CppA>("CppA");
  this->__destruct.registerDStruct(cppA);

  DStruct* cppB = makeNewDCpp<CppB>("CppB");
  this->__destruct.registerDStruct(cppB);
}

void Inheritance::test(void)
{
  std::cout << "==== Simple object B inherit simple object A ====" << std::endl;
  
  DObject* simpleA = this->__destruct.generate("SimpleA");
  DObject* simpleB = this->__destruct.generate("SimpleB");

  simpleB->setValue("text", RealValue<DUnicodeString>("Set in simpleB"));
  simpleB->setValue("text2", RealValue<DUnicodeString>("Set in simpleB"));

  this->show(simpleA);
  this->show(simpleB);

  std::cout << "==== Simple object C inherit simple object B ====" << std::endl;
  
  DObject* simpleC = this->__destruct.generate("SimpleC");

  simpleC->setValue("text", RealValue<DUnicodeString>("Set in simpleC"));
  simpleC->setValue("text2", RealValue<DUnicodeString>("Set in simpleC"));
  simpleC->setValue("text3", RealValue<DUnicodeString>("Set in simpleC"));

  this->show(simpleC);

  std::cout << "==== CppObject B inherit CppObject A ====" << std::endl;
  
  DObject* cppA = this->__destruct.generate("CppA");
  this->show(cppA);

  DObject* cppB = this->__destruct.generate("CppB");
  this->show(cppB);

  DObject* args = this->__destruct.generate("DVectorObject"); 
  args->call("push", RealValue<DObject*>(cppA));
  args->call("push", RealValue<DObject*>(cppB));


  std::cout << "==== ComposableObject ====" << std::endl;

  DStruct* composableAStruct = new DStruct(NULL, "ComposeA", ComposableObject<CppA>::newObject, CppA::ownAttributeBegin(), CppA::ownAttributeEnd());
  DStruct* composableBStruct = new DStruct(NULL, "ComposeB", ComposableObject<CppB>::newObject, CppB::ownAttributeBegin(), CppB::ownAttributeEnd());

  DObject* composableA = composableAStruct->newObject();
  DObject* composableB = composableBStruct->newObject();

  this->show(composableA);
  this->show(composableB);
  
  this->__destruct.registerDStruct(composableAStruct);
  this->__destruct.registerDStruct(composableBStruct);
  

  DStruct* composeABStruct = new DStruct(NULL, "ComposedAB", Compose::newObject);

  DStruct::DAttributeIterator attribute = composableAStruct->attributeBegin();
  for (; attribute != composableAStruct->attributeEnd(); ++attribute)
    composeABStruct->addAttribute(*attribute); 
  
  attribute = composableBStruct->attributeBegin();
  for (; attribute != composableBStruct->attributeEnd(); ++attribute)
    composeABStruct->addAttribute(*attribute);

  //this->__destruct.registerDStruct(composeABStruct); 
  //DObject* composition = composeABStruct->newObject();

  DObject* structsName = this->__destruct.generate("DVectorString");
  structsName->call("push", RealValue<DUnicodeString>("ComposeA"));
  structsName->call("push", RealValue<DUnicodeString>("ComposeB"));

  DObject* composition = new Compose(composeABStruct, RealValue<DObject*>(structsName));


  //test 
  composition->setValue("text1", RealValue<DUnicodeString>("text 1"));
  composition->setValue("text3", RealValue<DUnicodeString>("text 3"));
  composition->setValue("text2", RealValue<DUnicodeString>("text 2"));

  std::cout << "show composedAB: " << std::endl; 
  this->show(composition); 

  std::cout << "Call showText() " << std::endl;
  composition->call("showText");
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
