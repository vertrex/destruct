#include <iostream>

#include "refcount.hpp"
#include "dexception.hpp"
#include "dtype.hpp"
#include "drealvalue.hpp"
#include "dsimpleobject.hpp"

#include "protocol/dmutableobject.hpp"

using namespace Destruct;

Refcount::Refcount() : __destruct(DStructs::instance())
{
  DType::init();
  this->declare();
}

Refcount::~Refcount()
{
  DType::clean();
}

void Refcount::test(void)
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
  DObject* object = this->__destruct.generate(testObjectName);
  RealValue<DObject*> v2(object);
  object->destroy();
  }

  std::cout << "----create vector by realvalue<> = " << std::endl;
  {
  DObject* object = this->__destruct.generate(testObjectName);
  RealValue<DObject*> v = object;
  object->destroy();
  }

  std::cout << "-----------test 4 ------" << std::endl;
  {
  DObject* object(this->__destruct.generate(testObjectName));
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
  //std::cout << "----------test 7 ------" << std::endl;
  //{
   //RealValue<DObject*> object = this->__destruct.generate(testObjectName);
   //}
  std::cout << "----------test 8 ------" << std::endl;
  {
  RealValue<DObject*> object(this->__destruct.generate(testObjectName));
  //std::cout << "copy object to other rv" << std::endl;
  RealValue<DObject*> object2(object);
  ((DObject*)object)->destroy();
  }
  std::cout << "-------test 9-------" << std::endl;
  {
          //std::cout << "--create by copy" << std::endl;
   DObject* object(this->__destruct.generate(testObjectName));
   //std::cout << "--copy object to rv" << std::endl;
   RealValue<DObject*> object2(object);
   //std::cout << "--copy object to other rv" << std::endl;
   RealValue<DObject*> object3(object); 
   //std::cout << "--copy object to other rv" << std::endl;
   RealValue<DObject*> object4(object);
   object->destroy();
   //std::cout << "scope end" << std::endl;
  }

  std::cout << "----------test10 -------" << std::endl;
  {
   RealValue<DObject*> object(this->__destruct.generate(testObjectName));
   RealValue<DObject*> object2(object);
   RealValue<DObject*> object3(object);
   DObject* robj3 = object3;
   //std::cout << robj3 << std::endl;
   //std::cout << robj3->refCount() << std::endl;
   robj3->destroy();
  }


  std::cout << "------test11-----" << std::endl;
  {
    DValue value(RealValue<DObject*>(this->__destruct.generate(testObjectName)));
    DObject* obj = value.get<DObject*>();
    //obj->destroy();
    obj->destroy();
  }
  std::cout << "----test12------" << std::endl;
  {
    DValue value(RealValue<DObject*>(this->__destruct.generate(testObjectName)));
    DObject* object = value.get<DObject*>();
    //object->destroy();
    object->destroy();
  }
  std::cout << "-----test13----" << std::endl;
  {
    DValue value(RealValue<DObject*>(this->__destruct.generate(testObjectName)));
    DValue value2(value);
    DValue value3(value);
    DObject* object = value.get<DObject*>();
    //object->destroy(); //delete get
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
  RealValue<DObject*> ret =  this->testRVRV(object);
  DObject* r = ret;
  r->destroy();
  }
  std::cout << "----test: RealValue func(RealValue) 3--" << std::endl;
  {
          //DObject* object = this->__destruct.generate(testObjectName);
  RealValue<DObject*> ret =  this->testRVRV(RealValue<DObject*>(this->__destruct.generate(testObjectName)));
  DObject* r = ret;
  r->destroy();
  }
  std::cout << "----test: RealValue func(RealValue) 4--" << std::endl;
  {
          //DObject* object = this->__destruct.generate(testObjectName);
  RealValue<DObject*> ret =  this->testRVRV(this->__destruct.generate(testObjectName));
  DObject* r = ret;
  r->destroy();
  }
  std::cout << "----test: RealValue DValue const& object--" << std::endl;
  {
    DObject* object = this->__destruct.generate(testObjectName);
    DValue v = this->testDVCRefDV(RealValue<DObject*>(object)); //const& ok :) 
    DObject* res = v.get<DObject*>();
    //res->destroy();
    res->destroy(); //== object->destroy()
  }
  std::cout << "----test: RealValue DValue& object--" << std::endl;
  {
    DObject* object = this->__destruct.generate(testObjectName);
    DValue i = RealValue<DObject*>(object); //non const ref must copy in temp dvalue ! ? 
    DValue v = this->testDVRefDV(i);
    DObject* res = v.get<DObject*>();
    //res->destroy();
    res->destroy(); //== object->destroy()
  }   
  std::cout << "test: DValue testDVDV(DValue value)" << std::endl;
  {
    DObject* object = this->__destruct.generate(testObjectName);
    DValue i = RealValue<DObject*>(object);
    DValue v = this->testDVRefDV(i);
    DObject* res = v.get<DObject*>();
    //res->destroy();
    res->destroy(); 
  }
  std::cout << "test : DObject* testObjRV(RealValue<DObject*> object)" << std::endl;
  {
    DObject* object = this->__destruct.generate(testObjectName);
    this->testObjRV(object);
    object->destroy();
  }

  std::cout << "test : DCppRefCount returnObject" << std::endl;
  {
    DObject* cppRefCount = this->__destruct.generate("DCppRefCount");
    DObject* retObject = cppRefCount->call("returnObject").get<DObject*>();
    retObject->destroy();
    cppRefCount->destroy();
  }
return ;
  std::cout << "test : DCppRefCount returnObjectAsDvalue" << std::endl;
  {
    DObject* cppRefCount = this->__destruct.generate("DCppRefCount");
    DObject* retObject = cppRefCount->call("returnObjectAsDValue"); //.get<DObject*>();
    //std::cout << "retObjectAsValue ref count " << retObject->refCount() << std::endl;
    retObject->destroy();
    cppRefCount->destroy();
  }

  std::cout << "test : voidDVCref " << std::endl;
  {
    DObject* object = this->__destruct.generate(testObjectName);
    //std::cout << "voidDVCref ref before " << object->refCount() << std::endl;
    voidDVCRef(RealValue<DObject*>(object));
    //std::cout << "object ref after " << object->refCount() << std::endl;
    object->destroy();
  }
return ;
  std::cout << "test : setDobject " << std::endl;
  {
    DObject* cppRefCount = this->__destruct.generate("DCppRefCount");
    //std::cout << "setObject ref count " << cppRefCount->refCount() << std::endl;
    DObject* object = this->__destruct.generate(testObjectName);
    cppRefCount->call("setDObject", RealValue<DObject*>(object));  //recursively set in self ...
    object->destroy();
    cppRefCount->destroy();
  }

  std::cout << "test : setDobject as value " << std::endl;
  {
    DObject* cppRefCount = this->__destruct.generate("DCppRefCount");
    DObject* object = this->__destruct.generate(testObjectName);
    cppRefCount->setValue("valObject", RealValue<DObject*>(object));
    object->destroy();
    cppRefCount->destroy();
  }

return; 
  std::cout << "test : setDobject equal " << std::endl;
  {
    DObject* cppRefCount = this->__destruct.generate("DCppRefCount");
    DObject* object = this->__destruct.generate(testObjectName);
    cppRefCount->call("setDObjectEq", RealValue<DObject*>(object));  //recursively set in self ...
    std::cout << "cpprefcount ret ref count " << cppRefCount->refCount() << std::endl;
    std::cout << "object set  ref count " << object->refCount() << std::endl;
    object->destroy();
    cppRefCount->destroy();
  }
return;
  std::cout << "test : vectorObject" << std::endl;
  {
    DObject* vectorObject = this->__destruct.generate("DVectorObject");
    DObject* cppRefCount = this->__destruct.generate("DCppRefCount");

    vectorObject->call("push", RealValue<DObject*>(cppRefCount));
    vectorObject->call("push", RealValue<DObject*>(cppRefCount));
    vectorObject->call("push", RealValue<DObject*>(cppRefCount));
    vectorObject->call("push", RealValue<DObject*>(cppRefCount));


    DValue v = vectorObject->call("get", RealValue<DUInt64>(0));
    DValue iv = vectorObject->call("get", RealValue<DUInt64>(0));
    DValue ve = vectorObject->call("get", RealValue<DUInt64>(0));
    DValue vr = vectorObject->call("get", RealValue<DUInt64>(0));
    
    cppRefCount->destroy();
    vectorObject->destroy();
  }

  std::cout << "========END TEST FUNC ===================="<< std::endl;
}

RealValue<DObject*>   Refcount::testRVRV(RealValue<DObject*> rv)
{
  return rv;
}

DValue                Refcount::testDVCRefDV(DValue const& object)
{
  return object;
}

DValue                Refcount::testDVRefDV(DValue & object)
{
  return object;
}

DValue                Refcount::testDVDV(DValue value)
{
  return value;
}

DObject*              Refcount::testObjObj(DObject* object)
{
  return object;
}

RealValue<DObject*>   Refcount::testRVObj(DObject* object)
{
  return object;
}

DObject*              Refcount::testObjRV(RealValue<DObject*> object)
{
  return object;
}


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



void            Refcount::declare(void)
{
  DStruct*  argumentStruct = new DStruct(0, "ModuleArguments", DSimpleObject::newObject);
  argumentStruct->addAttribute(DAttribute(DType::DUnicodeStringType, "moduleName"));
  argumentStruct->addAttribute(DAttribute(DType::DObjectType, "arguments"));// DList<DObject*>()
  this->__destruct.registerDStruct(argumentStruct);

  DStruct*  localArgumentsStruct = new DStruct(0, "LocalArguments", DSimpleObject::newObject);
  localArgumentsStruct->addAttribute(DAttribute(DType::DObjectType, "path")); 
  this->__destruct.registerDStruct(localArgumentsStruct);

  DStruct* partitionArgumentsStruct = new DStruct(0, "PartitionArguments", DSimpleObject::newObject);
  partitionArgumentsStruct->addAttribute(DAttribute(DType::DUnicodeStringType, "file")); 
  this->__destruct.registerDStruct(partitionArgumentsStruct);

  DStruct* dcppRefCount = makeNewDCpp<DCppRefCount>("DCppRefCount");
  this->__destruct.registerDStruct(dcppRefCount);
}

void           Refcount ::show(DObject* object) const
{
  DObject* cout = this->__destruct.generate("DStreamCout");
  DObject* serializer = this->__destruct.generate("DeserializeText", RealValue<DObject*>(cout));
  serializer->call("DObject", RealValue<DObject*>(object));
  serializer->destroy();
  cout->destroy();
}

int     main(int argc, char** argv)
{
  Refcount rc;
 
  try 
  {
    rc.test();
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
