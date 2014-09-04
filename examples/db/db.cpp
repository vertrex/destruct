#include <iostream>

#include "db.hpp"
#include "dexception.hpp"
#include "dtype.hpp"
#include "drealvalue.hpp"
#include "dsimpleobject.hpp"

#include "protocol/dstream.hpp"
#include "protocol/dserialize.hpp"
#include "protocol/dmutableobject.hpp"

DB::DB() : __destruct(Destruct::Destruct::instance()), __session(NULL)
{
  Destruct::DType::init();
  this->declare();
}

DB::~DB()
{
  std::cout << "~DB" << std::endl;
  if (this->__session)
  {
  std::cout << "destroy session " << std::endl;
  this->__session->destroy();
  }
  Destruct::DType::clean();
}

void DB::testRefcount(void)
{
  std::cout << "====================Testing refcount for all kind of value & type============ " << std::endl;
  std::cout << "----create & delete vector" << std::endl;

  //std::string testObjectName = "ModuleArguments";
  std::string testObjectName = "DVectorObject";

  std::cout << "----generate & destroy----" << std::endl;
  {
  DObject* vector = this->__destruct.generate(testObjectName); 
  vector->destroy();
  }

  std::cout << "----create vector by realvalue()" << std::endl;
  {
  Destruct::DObject* object = this->__destruct.generate(testObjectName);
  RealValue<Destruct::DObject*> v2(object);
  object->destroy();
  }

  std::cout << "----create vector by realvalue<> = " << std::endl;
  {
  Destruct::DObject* object = this->__destruct.generate(testObjectName);
  RealValue<Destruct::DObject*> v = object;
  object->destroy();
  }

  std::cout << "-----------test 4 ------" << std::endl;
  {
  Destruct::DObject* object(this->__destruct.generate(testObjectName));
  object->destroy();
  }

  std::cout << "-----------test 5 ------" << std::endl;
  {
  RealValue<DObject*> object = this->__destruct.generate(testObjectName);   
  DObject* robject = object;
  robject->destroy();
  }

  std::cout << "----------test 6 ------" << std::endl;
  {
  RealValue<DObject*> object(this->__destruct.generate(testObjectName));
  DObject* robject = object;
  robject->destroy();
  }
  std::cout << "----------test 7 ------" << std::endl;
  {
          //RealValue<DObject*> object(this->__destruct.generate(testObjectName)); //delete pas 
  }
  std::cout << "----------test 8 ------" << std::endl;
  {
  RealValue<DObject*> object(this->__destruct.generate(testObjectName));
  std::cout << "copy object to other rv" << std::endl;
  RealValue<DObject*> object2(object); //XXX delete apres la deuxieme fois ????
  ((DObject*)object)->destroy();
  }
  std::cout << "-------test 9-------" << std::endl;
  {
   std::cout << "--create by copy" << std::endl;
   DObject* object(this->__destruct.generate(testObjectName));
   std::cout << "--copy object to rv" << std::endl;
   RealValue<DObject*> object2(object); //XXX delete apres la deuxieme fois ????
   std::cout << "--copy object to other rv" << std::endl;
   RealValue<DObject*> object3(object); //XXX delete apres la deuxieme fois ????
   std::cout << "--copy object to other rv" << std::endl;
   RealValue<DObject*> object4(object); //XXX delete apres la deuxieme fois ????
   object->destroy();
   std::cout << "scope end" << std::endl;
  }

  std::cout << "----------test10 -------" << std::endl;
  {
   std::cout << "--create by copy" << std::endl;
   RealValue<DObject*> object(this->__destruct.generate(testObjectName));
   std::cout << "--copy rv object to other rv" << std::endl;
   RealValue<DObject*> object2(object); //XXX delete apres la deuxieme fois ????
   std::cout << "--copy rv object again to other rv" << std::endl;
   RealValue<DObject*> object3(object); //XXX delete apres la deuxieme fois ????
   std::cout << "get obj val " << std::endl;
   DObject* robj3 = object3;
   std::cout << "destroy one obj" << std::endl;
   std::cout << robj3 << std::endl;
   std::cout << robj3->refCount() << std::endl;
   robj3->destroy();
   std::cout << "scope end val will be killed " << std::endl;
  }


  std::cout << "------test11-----" << std::endl;
  {
    Destruct::DValue value(RealValue<DObject*>(this->__destruct.generate(testObjectName)));
    DObject* obj = value.get<DObject*>();
    obj->destroy();
    obj->destroy();
  }
  std::cout << "----test12------" << std::endl;
  {
    Destruct::DValue value(RealValue<DObject*>(this->__destruct.generate(testObjectName)));
    DObject* object = value.get<DObject*>(); //DRealValue = ...
    object->destroy(); //destroy get ! 
    object->destroy(); //destroy original object !
  }
  std::cout << "-----test13----" << std::endl;
  {
    Destruct::DValue value(RealValue<DObject*>(this->__destruct.generate(testObjectName)));
    Destruct::DValue value2(value); //XXX delete 2 x segfault !  
    Destruct::DValue value3(value); //XXX delete 2 x segfault !  
    DObject* object = value.get<DObject*>(); //DRealValue = ...
    object->destroy(); //delete get
    object->destroy(); //delete newObject
  }

  std::cout << "===========End TEST VALUE ======== ==" << std::endl;
  
  std::cout << "===========TEST FUNCTION ARG & RET ==" << std::endl;
  std::cout << "----test: RealValue func(RealValue)--" << std::endl;
  {
  DObject* object = this->__destruct.generate(testObjectName);
  DObject* r =  this->testRVRV(object);
  r->destroy();
  }
  std::cout << "----test: RealValue func(RealValue) 2--" << std::endl;
  {
  DObject* object = this->__destruct.generate(testObjectName);
  RealValue<Destruct::DObject*> ret =  this->testRVRV(object);
  DObject* r = ret;
  r->destroy();
  }
  std::cout << "----test: RealValue func(RealValue) 3--" << std::endl;
  {
          //DObject* object = this->__destruct.generate(testObjectName);
  RealValue<Destruct::DObject*> ret =  this->testRVRV(RealValue<Destruct::DObject*>(this->__destruct.generate(testObjectName)));
  DObject* r = ret;
  r->destroy();
  }
  std::cout << "----test: RealValue func(RealValue) 4--" << std::endl;
  {
          //DObject* object = this->__destruct.generate(testObjectName);
  RealValue<Destruct::DObject*> ret =  this->testRVRV(this->__destruct.generate(testObjectName));
  DObject* r = ret;
  r->destroy();
  }
  std::cout << "----test: RealValue DValue const& object--" << std::endl;
  {
    DObject* object = this->__destruct.generate(testObjectName);
    DValue v = this->testDVCRefDV(RealValue<Destruct::DObject*>(object)); //const& ok :) 
    DObject* res = v.get<DObject*>();
    res->destroy();
    res->destroy(); //== object->destroy()
  }
  std::cout << "----test: RealValue DValue & object--" << std::endl;
  {
    DObject* object = this->__destruct.generate(testObjectName);
    DValue i = RealValue<Destruct::DObject*>(object); //non const ref must copy in temp dvalue ! ? 
    DValue v = this->testDVRefDV(i);
    DObject* res = v.get<DObject*>();
    res->destroy();
    res->destroy(); //== object->destroy()
  }   
  std::cout << "test: DValue testDVDV(DValue value)" << std::endl;
  {
    DObject* object = this->__destruct.generate(testObjectName);
    DValue i = RealValue<Destruct::DObject*>(object);
    DValue v = this->testDVRefDV(i);
    DObject* res = v.get<DObject*>();
    res->destroy();
    res->destroy(); 
  }

  std::cout << "========END TEST FUNC ===================="<< std::endl;
}

RealValue<DObject*>   DB::testRVRV(RealValue<DObject*> rv)
{
  return rv;
}

DValue                DB::testDVCRefDV(DValue const& object)
{
  return object;
}

DValue                DB::testDVRefDV(DValue & object)
{
  return object;
}

DValue                DB::testDVDV(DValue value)
{
  return value;
}

DObject*              DB::testObjObj(DObject* object)
{
  return object;
}

RealValue<DObject*>   DB::testRVObj(DObject* object)
{
  return object;
}

DObject*              DB::testObjRV(RealValue<DObject*> object)
{
  return object;
}

Destruct::DObject*  DB::populateSession(void)
{
        //this->testRefcount();

  //std::cout << "--------- push "<< std::endl;

  //Destruct::DObject* object4 = this->__destruct.generate("DVectorObject");

  //Destruct::DObject* object5 = this->__destruct.generate("LocalArguments");
  //std::cout << "Local argument count " << object5->refCount() << std::endl;
////  for (int i = 0; i < 1000; ++i)
/////XXX XXX XXX push n'add pas de ref !!! donc le faire soit meme avant ! 
  //object5->addRef(); 
  //object4->call("push", Destruct::RealValue<Destruct::DObject*>(object5));
  //std::cout << "Local argument count after push " << object5->refCount() << std::endl;
  //std::cout << "localarguments destroy " << std::endl;
  //object5->destroy();
  //std::cout << "dvector destroy" << std::endl;
  //object4->destroy();
  //std::cout << "------ret" << std::endl;


  std::cout << "POPUALTE SESSION" << std::endl;
  Destruct::DObject* sessionObject = this->__destruct.generate("Session");
  this->__session = static_cast<Session*>(sessionObject);
  
  Destruct::DObject* modules = this->__destruct.generate("DVectorObject");

  Destruct::DObject* local = this->__destruct.generate("LocalArguments"); 

  Destruct::DObject* localPath = this->__destruct.generate("DVectorString");
  localPath->call("push", Destruct::RealValue<Destruct::DUnicodeString>("/home/vertrex/dump/case/cfreds/cfreds.raw.raw"));
  localPath->call("push", Destruct::RealValue<Destruct::DUnicodeString>("/home/vertrex/dump/case/cfreds/test"));
  localPath->call("push", Destruct::RealValue<Destruct::DUnicodeString>("/home/vertrex/dump/case/cfreds/test3"));
  local->setValue("path", Destruct::RealValue<Destruct::DObject*>(localPath));
  localPath->destroy();

  Destruct::DObject* moduleArguments = this->__destruct.generate("ModuleArguments");
  moduleArguments->setValue("moduleName", Destruct::RealValue<Destruct::DUnicodeString>("local"));
  moduleArguments->setValue("arguments", Destruct::RealValue<Destruct::DObject*>(local));
  local->destroy();

  modules->call("push", Destruct::RealValue<Destruct::DObject*>(moduleArguments));
  moduleArguments->destroy();

  Destruct::DObject* partition = this->__destruct.generate("PartitionArguments");
  partition->setValue("file", Destruct::RealValue<Destruct::DUnicodeString>("cfreds.raw.raw/Partitons 1"));

  //modules->destroy();
  this->__session->modules = modules;

  moduleArguments = this->__destruct.generate("ModuleArguments");
  moduleArguments->setValue("moduleName", Destruct::RealValue<Destruct::DUnicodeString>("partition"));
  moduleArguments->setValue("arguments", Destruct::RealValue<Destruct::DObject*>(partition));
  ((DObject*)this->__session->modules)->call("push", Destruct::RealValue<Destruct::DObject*>(moduleArguments));
  partition->destroy();
  moduleArguments->destroy();

  //throw std::string("abc");
  return (this->__session); 
}

void            DB::declare(void)
{
  Destruct::DStruct*  session = makeNewDCpp<Session>("Session");
  this->__destruct.registerDStruct(session);

  Destruct::DStruct*  argumentStruct = new Destruct::DStruct(0, "ModuleArguments", Destruct::DSimpleObject::newObject);
  argumentStruct->addAttribute(Destruct::DAttribute(Destruct::DType::DUnicodeStringType, "moduleName"));
  argumentStruct->addAttribute(Destruct::DAttribute(Destruct::DType::DObjectType, "arguments"));// DList<DObject*>()
  this->__destruct.registerDStruct(argumentStruct);

  Destruct::DStruct*  localArgumentsStruct = new Destruct::DStruct(0, "LocalArguments", Destruct::DSimpleObject::newObject);
  localArgumentsStruct->addAttribute(Destruct::DAttribute(Destruct::DType::DObjectType, "path")); 
  this->__destruct.registerDStruct(localArgumentsStruct);

  Destruct::DStruct* partitionArgumentsStruct = new Destruct::DStruct(0, "PartitionArguments", Destruct::DSimpleObject::newObject);
  partitionArgumentsStruct->addAttribute(Destruct::DAttribute(Destruct::DType::DUnicodeStringType, "file")); 
  this->__destruct.registerDStruct(partitionArgumentsStruct); 
}

void            DB::show(Destruct::DObject* object) const
{
  Destruct::DStream* cout = static_cast<Destruct::DStream*>(this->__destruct.generate("DStreamCout"));
  Destruct::DSerializers::to("Text")->serialize(*cout, object);
}

void            DB::load(Destruct::DValue const& filePath)
{
  std::cout << "Loading database from file " << filePath.asUnicodeString() << std::endl;

  Destruct::DMutableObject* arg = static_cast<Destruct::DMutableObject*>(this->__destruct.generate("DMutable"));
  arg->setValueAttribute(Destruct::DType::DUnicodeStringType, "filePath", filePath); 
  arg->setValueAttribute(Destruct::DType::DInt8Type, "input",  Destruct::RealValue<DInt8>(Destruct::DStream::Input));

  Destruct::DStream* dstream = static_cast<Destruct::DStream*>(this->__destruct.generate("DStream", Destruct::RealValue<Destruct::DObject*>(arg)));
  arg->destroy();
  Destruct::DSerialize* serializer = Destruct::DSerializers::to("Binary");

  Destruct::DValue value = serializer->deserialize(*dstream, Destruct::DType::DObjectType);
  
  dstream->destroy();
//  std::cout << "Loaded ! found first object : " << value.asUnicodeString() << std::endl; 

  //this->__session = static_cast<Session*>(value.get<DObject*>());
  Destruct::DObject* session = value.get<Destruct::DObject*>();
  std::cout << "SESSION LOADED REFCUNT " << session->refCount() << " " << session->instanceOf()->name() << std::endl;
  session->destroy(); 
  session->destroy(); 
}

Session*        DB::session(void)
{
  return (this->__session);
}

int     main(int argc, char** argv)
{
  DB    db;
 
  try 
  {
    if (argc > 1 && std::string(argv[1]) == std::string("-l"))
    {
      db.load(RealValue<DUnicodeString>("db.sav"));
   //   db.show(db.session());

    }
    else if (argc > 1 && std::string(argv[1]) == std::string("-s"))
    {
      db.populateSession();
    //  db.show(db.session());
      db.session()->save(RealValue<DUnicodeString>("db.sav"));
    }
    else
    {
      std::cout << "Load database : -l" << std::endl
                << "Save database : -s" << std::endl;       
      return (1);
    }
  }
  catch (Destruct::DException const& exception)
  {
    std::cout << "Error : " << std::endl << exception.error() << std::endl; 
  }
  catch (const std::string& error)
  {
     std::cout << "Error : " << std::endl << error << std::endl;
  }
  return (0);
}
