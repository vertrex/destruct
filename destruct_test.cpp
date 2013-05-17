#include <iostream>
#include <memory>

#include "dstruct.hpp"
#include "dattribute.hpp"
#include "dsimpleobject.hpp"
#include "dcppobject.hpp"
#include "drealvalue.hpp"
#include "dunicodestring.hpp"
#include "dfuncvalue.hpp"
#include "dserialize.hpp"

#include "destruct_test.hpp"

#define output(msg)\
  if (this->__output)\
    std::cout << msg << std::endl;

namespace Destruct
{

DestructTest::DestructTest(bool output) : __output(output)
{
}

void DestructTest::run()
{
  /* 
   * Use only SimpleObject (inherit DynamicObject)
   * Create a base class (BaseNode)
   * Create a class that inherit baseNode (NtfsNode)
   * Create a class that have an NtfsNode object as member  (Nested)
   */
  /*this->createDynamicClass(); */
  this->createBaseClass();
  this->createNtfsClass(); 
  this->createNestedClass();
  this->createPrefetchBaseClass();
  this->createModifiableClass();
 
  this->createFuncClass();
}

void DestructTest::createModifiableClass(void)
{
// this class is not created with dobject so we can test if we can add method in python FIX =0
  DStruct* base = new DStruct(0, "Modify", DSimpleObject::newObject);
  
  base->addAttribute(DAttribute("Size", DType::DInt64Type));
  base->addAttribute(DAttribute("Children count", DType::DInt64Type));
  base->addAttribute(DAttribute("Offset", DType::DInt64Type));
  this->structRegistry()->registerDStruct(base);
}

void DestructTest::createBaseClass(void)
{
  DStruct* base = new DStruct(0, "BaseNode", DSimpleObject::newObject);
  this->structRegistry()->registerDStruct(base);
  this->structRegistry()->find("BaseNode");
  
  base->addAttribute(DAttribute("Size", DType::DInt64Type));
  base->addAttribute(DAttribute("Children count", DType::DInt64Type));
  base->addAttribute(DAttribute("Offset", DType::DInt64Type));

  this->showAttribute(base);
}


void DestructTest::createNtfsClass(void)
{
  DStruct* base = this->structRegistry()->find("BaseNode");
  if (base == NULL)
  {
    output("error retrieving basenode");
    return;
  }

  DStruct* ntfsNodeStruct = new DStruct(base, "NtfsNode", DSimpleObject::newObject);
  this->structRegistry()->registerDStruct(ntfsNodeStruct);
 
  ntfsNodeStruct->addAttribute(DAttribute("MBRStartOffset", DType::DInt64Type));
 
  this->showAttribute(ntfsNodeStruct);

  DObject* ts = ntfsNodeStruct->newObject();

  ts->setValue("MBRStartOffset", RealValue<DInt64>(1024));
  ts->setValue("Size", RealValue<DInt64>(20000));
  this->showObjectAttribute(ts);
 
  ts->destroy();
}

void DestructTest::createNestedClass(void)
{
  DStruct* nestedStructDef = new DStruct(NULL, "Nested", DSimpleObject::newObject);

  nestedStructDef->addAttribute(DAttribute("NestedStart", DType::DInt64Type));
  nestedStructDef->addAttribute(DAttribute("ObjectNested", DType::DObjectType));
  nestedStructDef->addAttribute(DAttribute("NestedEnd", DType::DUnicodeStringType));

  this->structRegistry()->registerDStruct(nestedStructDef);

  this->showAttribute(nestedStructDef);

  DStruct* ntfsBase = this->structRegistry()->find("NtfsNode");

  DObject* nestedStruct = nestedStructDef->newObject();

  nestedStruct->setValue("NestedStart", RealValue<DInt64>(32));
// create nested object and add value

  DObject* nb = ntfsBase->newObject();
  nb->setValue("Size", RealValue<DInt64>(1));

  nestedStruct->setValue("ObjectNested", RealValue<DObject*>(nb));
  nestedStruct->setValue("NestedEnd", RealValue<DUnicodeString>("My unicode string"));

  this->showObjectAttribute(nestedStruct);

  //destroy should always be called when object is out of scope 
  nestedStruct->destroy();
  nb->destroy();
}

void DestructTest::createPrefetchBaseClass(void)
{
  //MIX dyanmic base class & cpp class
  //// CPP only 
  //PrefetchXPClass  prefetchXPClass(21232132131, 10);
  this->structRegistry()->registerDStruct(new DStruct(0, "PrefetchBase", 0, PrefetchBaseClass::ownAttributeBegin(), PrefetchBaseClass::ownAttributeEnd()));

  DStruct* base = this->structRegistry()->find("PrefetchBase");

   output("Make cpp Prefetch XP")

//Create a classical DObject (could be accessible trough python) constructed with a pure cpp interface

  this->structRegistry()->registerDStruct(new DStruct(makeClass<PrefetchXPClass>(base, "PrefetchXP")));
  DStruct* prefetchXPDef = this->structRegistry()->find("PrefetchXP");
  DObject*  prefetchXPObject(prefetchXPDef->newObject());
  prefetchXPObject->setValue("Header", RealValue<DInt64>(12));
  output(prefetchXPObject->getValue("Header").asUnicodeString())

//Create a pure CPP object 

  output("create a pur cpp object and set execution time to 24")
  PrefetchXPClass prefetchXP(24, 2);
  output(prefetchXP.lastExecutionTime())


  output("create a derived dcppobject from the pure cpp object and show exec time with get value ")

  //cppobj copy the object so if modified the original object will not be modified
  DCppObject<PrefetchXPClass> prefetchXPcppobj(prefetchXPDef, prefetchXP);

  output(prefetchXPcppobj.getValue("Last execution time").asUnicodeString())

  output("set cppobject execution time to 42")

  prefetchXPcppobj.setValue("Last execution time", RealValue<DInt64>(42));

  output("read value from pure oject")

  output(prefetchXP.lastExecutionTime())
  prefetchXP.setLastExecutionTime(64);
  output(prefetchXP.lastExecutionTime())

  output(prefetchXPcppobj.getValue("Last execution time").asUnicodeString())
 
 //we add a getOriginal to modify the original, it's also possible to implement a constructor who use a pointer rather than a copy so original object will be modified
  PrefetchXPClass internalCopy = prefetchXPcppobj.getOriginal();

  output(internalCopy.lastExecutionTime())

  prefetchXPObject->destroy();
}

void    DestructTest::showAttribute(DStruct* def)
{
   DSerializers::to("XML")->serialize(std::cout, *def);
   DSerializers::to("Text")->serialize(std::cout, *def);
}


void    DestructTest::showObjectAttribute(DObject* object, int depth)
{
   DSerializers::to("XML")->serialize(std::cout, *object);
   DSerializers::to("Text")->serialize(std::cout, *object);
}

void DestructTest::setObjectValue(DObject* object) // ha met c unzerialize :) 
{
  object->setValue("num", RealValue<DInt64>(424242));
  object->setValue("text", RealValue<DUnicodeString>("My text."));

  const DStruct* dstruct = object->instanceOf();
  DObject* parentObject = dstruct->newObject();
  parentObject->setValue("num", RealValue<DInt64>(414141));
  parentObject->setValue("text", RealValue<DUnicodeString>("My parent object"));
  object->setValue("parent", RealValue<DObject*>(parentObject));
}

std::string   member(DObject* self)
{
  return std::string("call me baby !");
}

DObject* DestructTest::getObjectValue()
{
 /* Destruct* destruct = this->structRegistry();
  DStruct* pfDef = destruct->find("PrefetchXP");
  DObject* pf = pfDef->newObject();
  pf->setValue("Header", RealValue<DInt64>(44444));*/
  DStruct* dstruct  = new DStruct(0, "CallMe", DSimpleObject::newObject);

  dstruct->addAttribute(DAttribute(std::string("func"), DType::DUnicodeStringType));
  this->structRegistry()->registerDStruct(dstruct);

  DSimpleObject* sobj = (DSimpleObject*)dstruct->newObject();
//  DObject* obj = dstruct->newObject();
  //DSimpleObject* sobj = dynamic_cast<DSimpleObject*>(obj);
  sobj->replaceValue("func", DFunctionValue<std::string>(sobj, member));
//  obj->setDefault(sobj);

  output("Function result " <<  sobj->getValue("func").asUnicodeString())
  this->showObjectAttribute(sobj);  

  return (sobj);
}


void DestructTest::createFuncClass(void)
{
/*
 *    Here we go !
 */
  DStruct* dstruct  = new DStruct(0, "CallMe", DSimpleObject::newObject);

  dstruct->addAttribute(DAttribute(std::string("func"), DType::DUnicodeStringType));
  this->structRegistry()->registerDStruct(dstruct);

   DSimpleObject* sobj = (DSimpleObject*)dstruct->newObject();
//  DObject* obj = dstruct->newObject();
  //DSimpleObject* sobj = dynamic_cast<DSimpleObject*>(obj);

  sobj->replaceValue("func", DFunctionValue<std::string>(sobj, member));

  output("Function result " <<  sobj->getValue("func").asUnicodeString())
  
  this->showObjectAttribute(sobj);
  delete sobj; 
}

Destruct* DestructTest::structRegistry(void)
{
  return (&Destruct::Destruct::instance());
}


}
