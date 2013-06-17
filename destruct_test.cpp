#include <iostream>
#include <fstream>
#include <memory>

#include "dstruct.hpp"
#include "dattribute.hpp"
#include "dsimpleobject.hpp"
#include "dcppobject.hpp"
#include "drealvalue.hpp"
#include "dunicodestring.hpp"
#include "dfuncvalue.hpp"
#include "dserialize.hpp"
#include "dstream.hpp"

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

  this->createNtfsBootSector();
  this->deserializeNtfsBootSector();

  this->createArchive();
  this->readArchive();
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
  DSerializers::to("XML")->serialize(cout, *def);
  DSerializers::to("Text")->serialize(cout, *def);
}


void    DestructTest::showObjectAttribute(DObject* object, int depth)
{
   std::string fname("test-" + object->instanceOf()->name() + ".xml");
   
   DStream binoutxml(fname, DStream::Output);
   DSerializers::to("XML")->serialize(binoutxml, *object);

   fname = "test-" + object->instanceOf()->name() + ".txt";
   DStream binouttxt(fname, DStream::Output);
   DSerializers::to("Text")->serialize(binouttxt, *object);

   fname = "test-" + object->instanceOf()->name() + ".raw";
   DStream binoutraw(fname, DStream::Output);
   DSerializers::to("Raw")->serialize(binoutraw, *object);
}

void DestructTest::setObjectValue(DObject* object)
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


//void DestructTest::createNtfsPointedBootSector(void)
//{
  //DStruct* dstruct = new DStruct(0, "BPB", DSimpleObject::newObject);

  //dstruct->addAttribute(DAttribute("bytesPerSector", DType::DUInt16Type));
  //dstruct->addAttribute(DAttribute("sectorsPerCluster", DType::DUInt8Type));
  //dstruct->addAttribute(DAttribute("reserved1", DType::DUInt8Type));
  //dstruct->addAttribute(DAttribute("reserved2", DType::DUInt8Type));
  //dstruct->addAttribute(DAttribute("reserved3", DType::DUInt8Type));
  //dstruct->addAttribute(DAttribute("reserved4", DType::DUInt8Type));
  //dstruct->addAttribute(DAttribute("reserved5", DType::DUInt8Type));
  //dstruct->addAttribute(DAttribute("reserved6", DType::DUInt8Type));
  //dstruct->addAttribute(DAttribute("reserved7", DType::DUInt8Type));
  //dstruct->addAttribute(DAttribute("mediaDescriptor", DType::DUInt8Type));

  //dstruct->addAttribute(DAttribute("reserved2-0", DType::DUInt32Type));
  //dstruct->addAttribute(DAttribute("reserved2-1", DType::DUInt32Type));
  //dstruct->addAttribute(DAttribute("reserved2-2", DType::DUInt32Type));
  //dstruct->addAttribute(DAttribute("reserved2-3", DType::DUInt32Type));
  //dstruct->addAttribute(DAttribute("reserved2-4", DType::DUInt8Type));
  //dstruct->addAttribute(DAttribute("reserved2-5", DType::DUInt8Type));

  //dstruct->addAttribute(DAttribute("totalSectors", DType::DUInt64Type));
  //dstruct->addAttribute(DAttribute("MFTLogicalClusterNumber", DType::DUInt64Type));
  //dstruct->addAttribute(DAttribute("MFTMirrorLogicalClusterNumber", DType::DUInt64Type));
  //dstruct->addAttribute(DAttribute("clustersPerMFTRecord", DType::DUInt8Type));
  //dstruct->addAttribute(DAttribute("reserved3-0", DType::DUInt8Type));
  //dstruct->addAttribute(DAttribute("reserved3-1", DType::DUInt8Type));
  //dstruct->addAttribute(DAttribute("reserved3-2", DType::DUInt8Type));
  //dstruct->addAttribute(DAttribute("clustersPerIndexBuffer", DType::DInt8Type));
  //dstruct->addAttribute(DAttribute("reserved4-0", DType::DUInt8Type));
  //dstruct->addAttribute(DAttribute("reserved4-1", DType::DUInt8Type));
  //dstruct->addAttribute(DAttribute("reserved4-2", DType::DUInt8Type));
  //dstruct->addAttribute(DAttribute("volumeSerialNumber", DType::DUInt64Type));
  //dstruct->addAttribute(DAttribute("reserved5", DType::DUInt32Type));

  //this->structRegistry()->registerDStruct(dstruct); 

  //DStruct* dstructBootStrap = new DStruct(0, "bootStrap", DSimpleObject::newObject);
  //for (int i = 0; i < 53; i++)
  //{ 
    //std::ostringstream name;
    //name << "bootStrap-";
    //name << i;
    //dstructBootStrap->addAttribute(DAttribute(name.str(), DType::DUInt64Type));
  //}
  //dstructBootStrap->addAttribute(DAttribute("bootstrap-53", DType::DUInt16Type));
  //this->structRegistry()->registerDStruct(dstructBootStrap);

  //DStruct* dstructBootSector = new DStruct(0, "NtfsBootSector", DSimpleObject::newObject);
  //dstructBootSector->addAttribute(DAttribute("jump0", DType::DUInt8Type)); 
  //dstructBootSector->addAttribute(DAttribute("jump1", DType::DUInt8Type)); 
  //dstructBootSector->addAttribute(DAttribute("jump2", DType::DUInt8Type)); 
  //dstructBootSector->addAttribute(DAttribute("OEMID", DType::DUInt64Type)); 
                                                      ////  "BPB" "struct BPB" 
                                                     ////   DStruct::BPB& ?
////                                                     *BPB -> pointer to ?
////  dstructBootSector->addAttribute(DAttribute("BPB", "BPB")); //, "BPB !"  DAttribute("BPB", "BPB") (pour les dobject)
  //dstructBootSector->addAttribute(DAttribute("BPB", DType::DObjectType)); //, "BPB !"  DAttribute("BPB", "BPB") (pour les dobject)
  //dstructBootSector->addAttribute(DAttribute("bootStrap", DType::DObjectType));// DType::DStruct, "name" 

  //dstructBootSector->addAttribute(DAttribute("endOfSector", DType::DUInt16Type));

  //this->structRegistry()->registerDStruct(dstructBootSector);

  ///* create a bootsector */

  //DObject* bootSector = dstructBootSector->newObject(); 
  //bootSector->setValue("jump0", RealValue<DUInt8>(0xeb));
  //bootSector->setValue("jump1", RealValue<DUInt8>(0x52));
  //bootSector->setValue("jump2", RealValue<DUInt8>(0x90));
  //bootSector->setValue("OEMID", RealValue<DUInt64>(0x202020205346544e));

  //DObject* bpb = dstruct->newObject();
  //bpb->setValue("bytesPerSector", RealValue<DUInt16>(4096));//512));
  //bpb->setValue("sectorsPerCluster", RealValue<DUInt8>(0x1));
  //bpb->setValue("mediaDescriptor", RealValue<DUInt8>(0xf8)); //disk 0xf0 -> floppy
  //bpb->setValue("totalSectors", RealValue<DUInt64>(0x911e10ffffffff));
  //bpb->setValue("MFTLogicalClusterNumber", RealValue<DUInt64>(0x200000));
  //bpb->setValue("MFTMirrorLogicalClusterNumber", RealValue<DUInt64>(0x488f08));
  //bpb->setValue("clustersPerMFTRecord", RealValue<DUInt8>(0x2));
  //bpb->setValue("clustersPerIndexBuffer", RealValue<DInt8>(-1));//0x08));//0x08));
  //bpb->setValue("volumeSerialNumber", RealValue<DUInt64>(0xdeadbeefdeadbeef));
  //bootSector->setValue("BPB", RealValue<DObject*>(bpb));

  //DObject* bootStrap = dstructBootStrap->newObject();
  //bootSector->setValue("bootStrap", RealValue<DObject*>(bootStrap));
  //bootSector->setValue("endOfSector", RealValue<DUInt16>(0xAA55));

  //this->showObjectAttribute(bootSector);

 ////XXX read ntfs boot sector :)
//}

//use inheritance !

void DestructTest::createNtfsBootSector(void)
{
  DStruct* bootSectorHeader= new DStruct(0, "BootSectorHeader", DSimpleObject::newObject);
  bootSectorHeader->addAttribute(DAttribute("jump0", DType::DUInt8Type)); 
  bootSectorHeader->addAttribute(DAttribute("jump1", DType::DUInt8Type)); 
  bootSectorHeader->addAttribute(DAttribute("jump2", DType::DUInt8Type)); 
  bootSectorHeader->addAttribute(DAttribute("OEMID", DType::DUInt64Type)); 
  this->structRegistry()->registerDStruct(bootSectorHeader);

  DStruct* dstruct = new DStruct(bootSectorHeader, "BPB", DSimpleObject::newObject);

  dstruct->addAttribute(DAttribute("bytesPerSector", DType::DUInt16Type));
  dstruct->addAttribute(DAttribute("sectorsPerCluster", DType::DUInt8Type));
  dstruct->addAttribute(DAttribute("reserved1", DType::DUInt8Type));
  dstruct->addAttribute(DAttribute("reserved2", DType::DUInt8Type));
  dstruct->addAttribute(DAttribute("reserved3", DType::DUInt8Type));
  dstruct->addAttribute(DAttribute("reserved4", DType::DUInt8Type));
  dstruct->addAttribute(DAttribute("reserved5", DType::DUInt8Type));
  dstruct->addAttribute(DAttribute("reserved6", DType::DUInt8Type));
  dstruct->addAttribute(DAttribute("reserved7", DType::DUInt8Type));
  dstruct->addAttribute(DAttribute("mediaDescriptor", DType::DUInt8Type));

  dstruct->addAttribute(DAttribute("reserved2-0", DType::DUInt32Type));
  dstruct->addAttribute(DAttribute("reserved2-1", DType::DUInt32Type));
  dstruct->addAttribute(DAttribute("reserved2-2", DType::DUInt32Type));
  dstruct->addAttribute(DAttribute("reserved2-3", DType::DUInt32Type));
  dstruct->addAttribute(DAttribute("reserved2-4", DType::DUInt8Type));
  dstruct->addAttribute(DAttribute("reserved2-5", DType::DUInt8Type));

  dstruct->addAttribute(DAttribute("totalSectors", DType::DUInt64Type));
  dstruct->addAttribute(DAttribute("MFTLogicalClusterNumber", DType::DUInt64Type));
  dstruct->addAttribute(DAttribute("MFTMirrorLogicalClusterNumber", DType::DUInt64Type));
  dstruct->addAttribute(DAttribute("clustersPerMFTRecord", DType::DUInt8Type));
  dstruct->addAttribute(DAttribute("reserved3-0", DType::DUInt8Type));
  dstruct->addAttribute(DAttribute("reserved3-1", DType::DUInt8Type));
  dstruct->addAttribute(DAttribute("reserved3-2", DType::DUInt8Type));
  dstruct->addAttribute(DAttribute("clustersPerIndexBuffer", DType::DInt8Type));
  dstruct->addAttribute(DAttribute("reserved4-0", DType::DUInt8Type));
  dstruct->addAttribute(DAttribute("reserved4-1", DType::DUInt8Type));
  dstruct->addAttribute(DAttribute("reserved4-2", DType::DUInt8Type));
  dstruct->addAttribute(DAttribute("volumeSerialNumber", DType::DUInt64Type));
  dstruct->addAttribute(DAttribute("reserved5", DType::DUInt32Type));

  this->structRegistry()->registerDStruct(dstruct); 

  DStruct* dstructBootStrap = new DStruct(dstruct, "bootStrap", DSimpleObject::newObject);
  for (int i = 0; i < 53; i++)
  { 
    std::ostringstream name;
    name << "bootStrap-";
    name << i;
    dstructBootStrap->addAttribute(DAttribute(name.str(), DType::DUInt64Type));
  }
  dstructBootStrap->addAttribute(DAttribute("bootstrap-53", DType::DUInt16Type));
  this->structRegistry()->registerDStruct(dstructBootStrap);

  DStruct* dstructBootSector = new DStruct(dstructBootStrap, "NtfsBootSector", DSimpleObject::newObject);
  dstructBootSector->addAttribute(DAttribute("endOfSector", DType::DUInt16Type));

  this->structRegistry()->registerDStruct(dstructBootSector);

  /* create a bootsector */

  DObject* bootSector = dstructBootSector->newObject(); 
  bootSector->setValue("jump0", RealValue<DUInt8>(0xeb));
  bootSector->setValue("jump1", RealValue<DUInt8>(0x52));
  bootSector->setValue("jump2", RealValue<DUInt8>(0x90));
  bootSector->setValue("OEMID", RealValue<DUInt64>(0x202020205346544e));

  //DObject* bpb = dstruct->newObject();
  bootSector->setValue("bytesPerSector", RealValue<DUInt16>(512));//512));
  bootSector->setValue("sectorsPerCluster", RealValue<DUInt8>(0x1));
  bootSector->setValue("mediaDescriptor", RealValue<DUInt8>(0xf8)); //disk 0xf0 -> floppy
  bootSector->setValue("totalSectors", RealValue<DUInt64>(0x911e10));
  bootSector->setValue("MFTLogicalClusterNumber", RealValue<DUInt64>(0x200000));
  bootSector->setValue("MFTMirrorLogicalClusterNumber", RealValue<DUInt64>(0x488f08));
  bootSector->setValue("clustersPerMFTRecord", RealValue<DUInt8>(0x2));
  bootSector->setValue("clustersPerIndexBuffer", RealValue<DInt8>(0x8));
  bootSector->setValue("volumeSerialNumber", RealValue<DUInt64>(0xdeadbeefdeadbeef));
  //bootSector->setValue("BPB", RealValue<DObject*>(bpb));

  //DObject* bootStrap = dstructBootStrap->newObject();
  //bootSector->setValue("bootStrap", RealValue<DObject*>(bootStrap));
  bootSector->setValue("endOfSector", RealValue<DUInt16>(0xAA55));

  this->showObjectAttribute(bootSector);

 //XXX read ntfs boot sector :)
}

void    DestructTest::deserializeNtfsBootSector(void)
{
   std::string fname("cfreds-bs.raw");
   DObject* ntfsBootSector = Destruct::instance().find("NtfsBootSector")->newObject();
   DStream inbootsector(fname, DStream::Input);
   DSerializers::to("Raw")->deserialize(inbootsector, *ntfsBootSector);
   
   //std::cout << std::hex << ntfsBootSector->getValue("jump0").asUnicodeString() << std::endl;
   //std::cout << std::hex << ntfsBootSector->getValue("jump1").asUnicodeString() << std::endl;
   //std::cout << std::hex << ntfsBootSector->getValue("jump2").asUnicodeString() << std::endl;
   //std::cout << ntfsBootSector->getValue("OEMID").get<DUInt64>() << std::endl;
   //std::cout << std::hex << ntfsBootSector->getValue("endOfSector").asUnicodeString() << std::endl;
   ntfsBootSector->setValue("bytesPerSector", RealValue<DUInt16>(4096));

   fname = "cfreds-bs-mod-size.raw";
   DStream outraw(fname, DStream::Output);
   DSerializers::to("Raw")->serialize(outraw, *ntfsBootSector);

   {
   fname = "destruct-test.bin";
   DStream outbin(fname, DStream::Output);
   DSerializers::to("Binary")->serialize(outbin, *Destruct::instance().find("NtfsBootSector"));
   //XXX we yse a scioe {} because close() is not implemented but called in destructor, and we must close the output stream to open it in input
   }

   DStream binin(fname, DStream::Input);
   DStruct* ntfsBS = DSerializers::to("Binary")->deserialize(binin);

   if (ntfsBS == NULL)
   {
     std::cout << "Can't deserialize DStruct* to binary " << std::endl;
     return ;
   }

   fname = "ntfsbootsector-deserialize.txt";
   DStream outboottext(fname, DStream::Output);
   DSerializers::to("Text")->serialize(outboottext, *ntfsBS);

   //std::stringstream f;// = "xntfs_";// + i;
   //f << "xntfs_";
 
   //DStream o("xntfs", DStream::Output);
   //for (unsigned int i = 0; i < 1000000; i++)
   //{
   //DSerializers::to("Raw")->serialize(o, *ntfsBootSector);
   //}
}

void DestructTest::createArchive(void)
{
  //This is a test for a DFL or DFF destruct archive using lower level functions 
  Destruct& db = Destruct::instance();   

  size_t count = db.count();

  DStream binout("destruct.sav", DStream::Output);
  DSerialize* toBinary = DSerializers::to("Binary");

//toBynar.write("0xDFF0";) header or something ?
//version number

// Something like inttype order and count 

  binout.write((char*)&count, sizeof(count));
  for (size_t i = 0; i < count; ++i)
  {
    DStruct* dstruct = db.find(i);
    toBinary->serialize(binout, *dstruct);
  }      

//write needed object with content ! 
//....
// Test cpp class

//compress stream to gz or/and encrypt it ?
}

void DestructTest::readArchive(void)
{
  Destruct& db = Destruct::instance();

  size_t count = db.count();

  DStream binin("destruct.sav");
  DSerialize* toBinary = DSerializers::to("Binary");

  binin.read((char*)&count, sizeof(count));
  for (size_t i = 0; i < count; ++i)
  {
    DStruct* dstruct = toBinary->deserialize(binin);
    if (dstruct)
    {
      std::cout << "have deserialized " << dstruct->name() << " attributes count : "  << dstruct->attributeCount() << std::endl;
      db.registerDStruct(dstruct);
    }
    else
      std::cout << "can't deserialize database " << std::endl;
  }
}

Destruct* DestructTest::structRegistry(void)
{
  return (&Destruct::Destruct::instance());
}


}
