#include <iostream>
#include <fstream>
#include <memory>

#include "dstruct.hpp"
#include "dattribute.hpp"
#include "dsimpleobject.hpp"
#include "drealvalue.hpp"
#include "dunicodestring.hpp"

#include "destruct_test.hpp"

#define output(msg)\
  if (this->__output)\
    std::cout << msg << std::endl;

#include "protocol/diterator.hpp"
#include "protocol/dvector.hpp"
#include "protocol/dclassobject.hpp"
#include "protocol/dserialize.hpp"
#include "protocol/dstream.hpp"

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
        //this->createDynamicClass(); 
  this->createBaseClass();
  this->createNtfsClass(); 
  this->createNestedClass();
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
  
  base->addAttribute(DAttribute(DType::DInt64Type, "Size"));
  base->addAttribute(DAttribute(DType::DInt64Type, "Children count"));
  base->addAttribute(DAttribute(DType::DInt64Type, "Offset"));
  this->structRegistry()->registerDStruct(base);
}

void DestructTest::createBaseClass(void)
{
  DStruct* base = new DStruct(0, "BaseNode", DSimpleObject::newObject);
  this->structRegistry()->registerDStruct(base);
  this->structRegistry()->find("BaseNode");
  
  base->addAttribute(DAttribute(DType::DInt64Type, "Size"));
  base->addAttribute(DAttribute(DType::DInt64Type, "Children count"));
  base->addAttribute(DAttribute(DType::DInt64Type, "Offset"));

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
 
  ntfsNodeStruct->addAttribute(DAttribute(DType::DInt64Type, "MBRStartOffset"));
 
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

  nestedStructDef->addAttribute(DAttribute(DType::DInt64Type, "NestedStart"));
  nestedStructDef->addAttribute(DAttribute(DType::DObjectType, "ObjectNested"));
  nestedStructDef->addAttribute(DAttribute(DType::DUnicodeStringType, "NestedEnd"));

  this->structRegistry()->registerDStruct(nestedStructDef);

  this->showAttribute(nestedStructDef);

  try //ni,p car utilse null plus rpaide 
  {
    DStruct* ntfsBase = this->structRegistry()->find("NtfsNode");
    if (ntfsBase == NULL)
  //throw DException ici plutot que ds find  ? python le gera ds ca lib et en c++ c en mode fast 
      return ;
    DObject* nestedStruct = nestedStructDef->newObject();

    nestedStruct->setValue("NestedStart", RealValue<DInt64>(32));
    // create nested object and add value
     DObject* nb = ntfsBase->newObject();
     nb->setValue("Size", RealValue<DInt64>(1));

     nestedStruct->setValue("ObjectNested", RealValue<DObject*>(nb));
     nestedStruct->setValue("NestedEnd", RealValue<DUnicodeString>("My unicode string"));

     this->showObjectAttribute(nestedStruct);

     nestedStruct->destroy();
     nb->destroy();

  } 
  catch (DException exception)
  {
    std::cout << exception.error() << std::endl;
  }
  
  //XXX throw error in find ? morre c++ / object style?)
}

void    DestructTest::showAttribute(DStruct* def)
{
  //DSerializers::to("XML")->serialize(cout, *def);  //XXX DSerializer
  //DSerializers::to("Text")->serialize(cout, *def);
}


void    DestructTest::showObjectAttribute(DObject* object, int depth)
{
  std::string fname("output/test-" + object->instanceOf()->name() + ".xml");
  
  //DStream binoutxml(fname, DStream::Output); //XXX Dserializer
  //DSerializers::to("XML")->serialize(binoutxml, *object);

  //fname = "output/test-" + object->instanceOf()->name() + ".txt";
  //DStream binouttxt(fname, DStream::Output);
  //DSerializers::to("Text")->serialize(binouttxt, *object);

  //fname = "output/test-" + object->instanceOf()->name() + ".raw";
  //DStream binoutraw(fname, DStream::Output);

  //DSerializers::to("Raw")->serialize(binoutraw, *object);
}

void DestructTest::setObjectValue(DObject* object)
{
//XXX test call python
  std::cout << "DestructTest::setObjectValue()->call(returnVoid)" << std::endl;
  std::cout << "CPP: object->returnVoid(0) " << object->call("returnVoid", RealValue<DInt32>(0)).asUnicodeString() << std::endl;

  std::cout << "DestructTest::setObjectValue()->call(callVoid)" << std::endl;
  std::cout << "CPP: x = object->callVoid() " <<   object->call("callVoid", RealValue<DObject*>(DNone)).asUnicodeString() << std::endl; 
  std::cout << "CPP: object->allVoid() " << object->call("allVoid", RealValue<DObject*>(DNone)).asUnicodeString() << std::endl;

  //for (int32_t i = 0; i < 1000000; i++)
  //{
     //object->call("returnVoid", RealValue<DInt32>(0));
     //object->call("callVoid", RealValue<DObject*>(DNone));
     //object->call("allVoid", RealValue<DObject*>(DNone));
  //}



//,ais si func doit renvoyer un dobject* 
//DObject* res = .vget<DObject*>(); ca va geuler aussi ?
//faut soit un dvalue special DValue == NoneValue !
//soit un realvalue  special ...
//soit un nullobject/voidobject qui peut etre set pour n importe qu elle value car y a un compareteur de cast ou je c pas koi ????
// un auto-cast to NULL 

  try 
  {
    std::cout << "call python result : " << object->call("get", RealValue<DInt32>(0)).asUnicodeString() << std::endl;
  } 
  //catch (const std::string e) 
  //{
    //std::cout << "Error calling python code : " << e << std::endl;
  //
  //XXX test dexcetpon doit etre const
  catch (DException exception)
  {
    std::cout << exception.error() << std::endl;
  }
////for (uint32_t x = 0; x < 1000000; x++)
//DValue r = object->call("get", RealValue<DInt32>(0));
  //
  return;
  object->setValue("num", RealValue<DInt64>(424242));
  object->setValue("text", RealValue<DUnicodeString>("My text."));

  //const DStruct* dstruct = object->instanceOf();
  DStruct* dstruct = object->instanceOf();
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
  DObject* sobj = this->createStringVector();
  //sobj->addRef();
  //this->showObjectAttribute(sobj);  
  //return (this->createStringVector());
  sobj->addRef();
  return (sobj);
}


void DestructTest::createFuncClass(void)
{
  std::cout << "Test list " << std::endl;
  DObject* iv = this->createIntVector();
  iv->destroy();
  std::cout << "Creafunc class  create string vector" << std::endl; 
  DObject* is = this->createStringVector();
  is->destroy();
  std::cout << "return create string vector" << std::endl;
}

DObject*        DestructTest::createIntVector(void)
{
  typedef DVector<DInt32, DType::DInt32Type> DVectorInt;
//XXX pas sur la stack pour python donc faut le delete !

//DVectorInt* dvectori = new DVectorInt;

  
//dvectori->push(RealValue<DInt32 >(0xbad));
//dvectori->push(RealValue<DInt32 >(0xc00fee));
//

//DStruct* dstructvector = makeNewDClass< DVectorInt  >(NULL, "DVectorDInt32");
//this->structRegistry()->registerDStruct(dstructvector);
//
//DClassObject<DVectorInt>* dcppvectori = makeNewDObject<DVectorInt>(dstructvector, *dvectori);
//
//std::cout << dcppvectori->call("get", RealValue<DUInt64>(0)).asUnicodeString() << std::endl;
//std::cout << dcppvectori->call("get", RealValue<DUInt64>(0)).asUnicodeString() << std::endl;
// 
//dcppvectori->call("push", RealValue<DInt32>(0xdffdff));
// 
//std::cout << dcppvectori->call("get", RealValue<DUInt64>(2)).asUnicodeString() << std::endl;

//return (dcppvectori);
  return (DNone);
}

DObject*        DestructTest::createStringVector(void)
{
        //typedef DVector<DUnicodeString, DType::DUnicodeStringType> DVectorString;
        //DVectorString* dvectors = new DVectorString(); 
        // 
        //dvectors->push(RealValue<DUnicodeString>(std::string("my_first_string")));
        //dvectors->push(RealValue<DUnicodeString>("my_second_string"));
        //
        //DStruct* dstructvectors = makeNewDClass < DVectorString >(NULL, "DVector<String");
        //////si non ca plante en python
        //this->structRegistry()->registerDStruct(dstructvectors);
        //////DVector<DunicodeString *>                                                 //*
        //DClassObject<DVectorString >* dcppvectors = makeNewDObject<DVectorString >(dstructvectors, *dvectors);
        //
        //DValue gfunctionValue = dcppvectors->getValue("get");
        //DFunctionObject* g = dcppvectors->getValue("get").get< DFunctionObject*  >();
        //
        //std::cout << g->call(RealValue<DUInt64>(0)).get<DUnicodeString>()<< std::endl;
        //std::cout << g->call(RealValue<DUInt64>(1)).get<DUnicodeString>()<< std::endl;
        //
        //DFunctionObject* conv = dcppvectors->getObject;
        //conv->call(RealValue<DUInt64>(1)); 

  //dcppvectors->setValue("push", dcppvectors->getObject);
  //return (dcppvectors);
  return (DNone);
}

void DestructTest::createNtfsBootSector(void)
{
  DStruct* bootSectorHeader= new DStruct(0, "BootSectorHeader", DSimpleObject::newObject);
  bootSectorHeader->addAttribute(DAttribute(DType::DUInt8Type, "jump0")); 
  bootSectorHeader->addAttribute(DAttribute(DType::DUInt8Type, "jump1")); 
  bootSectorHeader->addAttribute(DAttribute(DType::DUInt8Type, "jump2")); 
  bootSectorHeader->addAttribute(DAttribute(DType::DUInt64Type, "OEMID")); 
  this->structRegistry()->registerDStruct(bootSectorHeader);

  DStruct* dstruct = new DStruct(bootSectorHeader, "BPB", DSimpleObject::newObject);

  dstruct->addAttribute(DAttribute(DType::DUInt16Type, "bytesPerSector"));
  dstruct->addAttribute(DAttribute(DType::DUInt8Type, "sectorsPerCluser"));
  dstruct->addAttribute(DAttribute(DType::DUInt8Type, "reserved1"));
  dstruct->addAttribute(DAttribute(DType::DUInt8Type, "reserved2"));
  dstruct->addAttribute(DAttribute(DType::DUInt8Type, "reserved3"));
  dstruct->addAttribute(DAttribute(DType::DUInt8Type, "reserved4"));
  dstruct->addAttribute(DAttribute(DType::DUInt8Type, "reserved5"));
  dstruct->addAttribute(DAttribute(DType::DUInt8Type, "reserved6"));
  dstruct->addAttribute(DAttribute(DType::DUInt8Type, "reserved7"));
  dstruct->addAttribute(DAttribute(DType::DUInt8Type, "mediaDescriptor"));

  dstruct->addAttribute(DAttribute(DType::DUInt32Type, "reserved2-0"));
  dstruct->addAttribute(DAttribute(DType::DUInt32Type, "reserved2-1"));
  dstruct->addAttribute(DAttribute(DType::DUInt32Type, "reserved2-2"));
  dstruct->addAttribute(DAttribute(DType::DUInt32Type, "reserved2-3"));
  dstruct->addAttribute(DAttribute(DType::DUInt8Type, "reserved2-4"));
  dstruct->addAttribute(DAttribute(DType::DUInt8Type, "reserved2-5"));

  dstruct->addAttribute(DAttribute(DType::DUInt64Type, "totalSectors"));
  dstruct->addAttribute(DAttribute(DType::DUInt64Type, "MFTLogicalClusterNumber"));
  dstruct->addAttribute(DAttribute(DType::DUInt64Type, "MFTMirrorLogicalCluserNumber"));
  dstruct->addAttribute(DAttribute(DType::DUInt8Type, "clusersPerMFTRecord"));
  dstruct->addAttribute(DAttribute(DType::DUInt8Type, "reserved3-0"));
  dstruct->addAttribute(DAttribute(DType::DUInt8Type, "reserved3-1"));
  dstruct->addAttribute(DAttribute(DType::DUInt8Type, "reserved3-2"));
  dstruct->addAttribute(DAttribute(DType::DInt8Type, "clustersPerIndexBuffer"));
  dstruct->addAttribute(DAttribute(DType::DUInt8Type, "reserved4-0"));
  dstruct->addAttribute(DAttribute(DType::DUInt8Type, "reserved4-1"));
  dstruct->addAttribute(DAttribute(DType::DUInt8Type, "reserved4-2"));

  dstruct->addAttribute(DAttribute(DType::DUInt64Type, "volumeSerialNumber"));
  dstruct->addAttribute(DAttribute(DType::DUInt32Type, "reserved5"));

  this->structRegistry()->registerDStruct(dstruct); 

  DStruct* dstructBootStrap = new DStruct(dstruct, "bootStrap", DSimpleObject::newObject);
  for (int i = 0; i < 53; i++)
  { 
    std::ostringstream name;
    name << "bootStrap-";
    name << i;
    dstructBootStrap->addAttribute(DAttribute(DType::DUInt64Type, name.str()));
  }
  dstructBootStrap->addAttribute(DAttribute(DType::DUInt16Type,"bootstrap-53"));
  this->structRegistry()->registerDStruct(dstructBootStrap);

  DStruct* dstructBootSector = new DStruct(dstructBootStrap, "NtfsBootSector", DSimpleObject::newObject);
  dstructBootSector->addAttribute(DAttribute(DType::DUInt16Type, "endOfSector"));

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

  std::string fname = "output/test-BIG-" + bootSector->instanceOf()->name() + ".raw";
  //DStream binoutraw(fname, DStream::Output); //XXX DSerializer
  ////for (int x = 0; x < 1000000*2; x++) //deserialize 10 go
  ////DSerializers::to("Raw")->serialize(binoutraw, *bootSector);
  //DStream inbootsector(fname, DStream::Input);
  //for (int x = 0; x < 1000000*2; x++) //deserialize 1 go
  //{
  ////2 millions object new == a peu pres 6GO RAM !
  //DObject* bootSector = dstructBootSector->newObject(); 
  //DSerializers::to("Raw")->deserialize(inbootsector, *bootSector);
  //bootSector->destroy();
  //}
  //
  ////XXX read ntfs boot sector :)
}

void    DestructTest::deserializeNtfsBootSector(void)
{
   std::string fname("output/cfreds-bs.raw");
   //DObject* ntfsBootSector = Destruct::instance().find("NtfsBootSector")
   //DStruct* ntfsBootSectorStruct = Destruct::instance().find("NtfsBootSector");
   //DObject* ntfsBootSector = ntfsBootSectorStruct->newObject();
   //DStream inbootsector(fname, DStream::Input); //XXX DSerializer
   //DSerializers::to("Raw")->deserialize(inbootsector, *ntfsBootSector);
   
   ////std::cout << std::hex << ntfsBootSector->getValue("jump0").asUnicodeString() << std::endl;
   ////std::cout << std::hex << ntfsBootSector->getValue("jump1").asUnicodeString() << std::endl;
   ////std::cout << std::hex << ntfsBootSector->getValue("jump2").asUnicodeString() << std::endl;
   ////std::cout << ntfsBootSector->getValue("OEMID").get<DUInt64>() << std::endl;
   ////std::cout << std::hex << ntfsBootSector->getValue("endOfSector").asUnicodeString() << std::endl;
   //ntfsBootSector->setValue("bytesPerSector", RealValue<DUInt16>(4096));

   //fname = "output/cfreds-bs-mod-size.raw";
   //DStream outraw(fname, DStream::Output);
   //DSerializers::to("Raw")->serialize(outraw, *ntfsBootSector);

   //{
   //fname = "output/destruct-test.bin";
   //DStream outbin(fname, DStream::Output);
   //DSerializers::to("Binary")->serialize(outbin, *Destruct::instance().find("NtfsBootSector"));
   ////XXX we yse a scioe {} because close() is not implemented but called in destructor, and we must close the output stream to open it in input
   //}
   //
   //DStream binin(fname, DStream::Input);
   //DStruct* ntfsBS = DSerializers::to("Binary")->deserialize(binin);
   //
   //if (ntfsBS == NULL)
   //{
   //std::cout << "Can't deserialize DStruct* to binary " << std::endl;
   //return ;
   //}
   //
   //fname = "output/ntfsbootsector-deserialize.txt";
   //DStream outboottext(fname, DStream::Output);
   //DSerializers::to("Text")->serialize(outboottext, *ntfsBS);
   //
   ////std::stringstream f;// = "xntfs_";// + i;
   ////f << "xntfs_";
   // 
   ////DStream o("xntfs", DStream::Output);
   ////for (unsigned int i = 0; i < 1000000; i++)
   ////{
   ////DSerializers::to("Raw")->serialize(o, *ntfsBootSector);
   ////}
}

void DestructTest::createArchive(void)
{
  //This is a test for a DFL or DFF destruct archive using lower level functions 
  //Destruct& db = Destruct::instance();   

  //size_t count = db.count();

  //DStream binout("output/destruct.sav", DStream::Output); //XXX DSerializer
  //DSerialize* toBinary = DSerializers::to("Binary");

  ////toBynar.write("0xDFF0";) header or something ?
  ////version number
  //
  //// Something like inttype order and count 
  //
  //binout.write((char*)&count, sizeof(count));
  //for (size_t i = 0; i < count; ++i)
  //{
  //DStruct* dstruct = db.find(i);
  //toBinary->serialize(binout, *dstruct);
  //}      
  //
  ////write needed object with content ! 
  ////....
  //// Test cpp class
  //
  ////compress stream to gz or/and encrypt it ?
}

void DestructTest::readArchive(void)
{
        //Destruct& db = Destruct::instance();

        //size_t count = db.count();

  //DStream binin("output/destruct.sav");//XXX DSerializer
  //DSerialize* toBinary = DSerializers::to("Binary");
  //
  //binin.read((char*)&count, sizeof(count));
  //for (size_t i = 0; i < count; ++i)
  //{
  //DStruct* dstruct = toBinary->deserialize(binin);
  //if (dstruct)
  //{
  //std::cout << "have deserialized " << dstruct->name() << " attributes count : "  << dstruct->attributeCount() << std::endl;
  //db.registerDStruct(dstruct);
  //}
  //else
  //std::cout << "can't deserialize database " << std::endl;
  //}
}

Destruct* DestructTest::structRegistry(void)
{
  return (&Destruct::Destruct::instance());
}


}
