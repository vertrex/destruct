#include <iostream>

#include "db.hpp"
#include "dexception.hpp"
#include "dtype.hpp"
#include "drealvalue.hpp"
#include "dsimpleobject.hpp"

#include "protocol/dstream.hpp"
#include "protocol/dmutableobject.hpp"

DB::DB() : __destruct(DStructs::instance()), __session(NULL)
{
  DType::init();
  this->declare();
}

DB::~DB()
{
  if (this->__session)
    this->__session->destroy();
  DType::clean();
}

DObject*  DB::populateSession(void)
{
  std::cout << "POPUALTE SESSION" << std::endl;
  DObject* sessionObject = this->__destruct.generate("Session");
  this->__session = static_cast<Session*>(sessionObject);
  
  DObject* modules = this->__destruct.generate("DVectorObject");

  DObject* local = this->__destruct.generate("LocalArguments"); 

  DObject* localPath = this->__destruct.generate("DVectorString");
  localPath->call("push", RealValue<DUnicodeString>("/home/vertrex/dump/case/cfreds/cfreds.raw.raw"));
  localPath->call("push", RealValue<DUnicodeString>("/home/vertrex/dump/case/cfreds/test"));
  localPath->call("push", RealValue<DUnicodeString>("/home/vertrex/dump/case/cfreds/test3"));
  local->setValue("path", RealValue<DObject*>(localPath));
  localPath->destroy();

  DObject* moduleArguments = this->__destruct.generate("ModuleArguments");
  moduleArguments->setValue("moduleName", RealValue<DUnicodeString>("local"));
  moduleArguments->setValue("arguments", RealValue<DObject*>(local));
  local->destroy();

  modules->call("push", RealValue<DObject*>(moduleArguments));
  moduleArguments->destroy();

  DObject* partition = this->__destruct.generate("PartitionArguments");
  partition->setValue("file", RealValue<DUnicodeString>("cfreds.raw.raw/Partitons 1"));

  this->__session->modules = modules;

  moduleArguments = this->__destruct.generate("ModuleArguments");
  moduleArguments->setValue("moduleName", RealValue<DUnicodeString>("partition"));
  moduleArguments->setValue("arguments", RealValue<DObject*>(partition));
  ((DObject*)this->__session->modules)->call("push", RealValue<DObject*>(moduleArguments));

  partition->destroy();
  moduleArguments->destroy();
  modules->destroy();

  //Save a DMapString

  //DObject* stringMap = this->__destruct.generate("DMapString");

  //DObject* item = stringMap->call("newItem").get<DObject*>();
  //item->setValue("index", RealValue<DUnicodeString>("1"));
  //item->setValue("value", RealValue<DUnicodeString>("a"));
  //stringMap->call("setItem", RealValue<DObject*>(item)); 

  //item->setValue("index", RealValue<DUnicodeString>("2"));
  //item->setValue("value", RealValue<DUnicodeString>("b"));
  //stringMap->call("setItem", RealValue<DObject*>(item));
  //item->destroy();

  //((DObject*)this->__session->modules)->call("push", RealValue<DObject*>(stringMap)); //leak or serial leak
  //stringMap->destroy();
 
  return (this->__session); 
}

void            DB::declare(void)
{
  DStruct*  session = makeNewDCpp<Session>("Session");
  this->__destruct.registerDStruct(session);

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
}

void            DB::show(DObject* object) const
{
  DObject* cout = this->__destruct.generate("DStreamCout");
  DObject* serializeText = this->__destruct.generate("SerializeText", RealValue<DObject*>(cout));
  serializeText->call("DObject", RealValue<DObject*>(object));
  cout->destroy();
  serializeText->destroy();
}

void            DB::load(DValue const& filePath)
{
  std::cout << "Loading database from file " << filePath.asUnicodeString() << std::endl;

  DMutableObject* arg = static_cast<DMutableObject*>(this->__destruct.generate("DMutable"));
  arg->setValueAttribute(DType::DUnicodeStringType, "filePath", filePath); 
  arg->setValueAttribute(DType::DInt8Type, "input",  RealValue<DInt8>(DStream::Input));

  DObject* dstream = this->__destruct.generate("DStream", RealValue<DObject*>(arg));
  arg->destroy();
  DObject* deserializer = this->__destruct.generate("DeserializeBinary", RealValue<DObject*>(dstream));

  DValue dstruct = deserializer->call("DStruct");

  std::cout << "Found struct " << dstruct.get<DStruct*>()->name() << std::endl;

  DObject* vectorStruct = deserializer->call("DObject").get<DObject*>();

  for (DUInt64 i = 0; i < vectorStruct->call("size").get<DUInt64>(); ++i)
  {
    DStruct* dstruct = vectorStruct->call("get", RealValue<DUInt64>()).get<DStruct*>();
    std::cout << "Found " << dstruct->name() << std::endl;
  }
  vectorStruct->destroy();

  DValue value = deserializer->call("DObject");
 
  dstream->destroy();
  deserializer->destroy();

  this->__session = static_cast<Session*>(value.get<DObject*>());
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
      db.show(db.session());

    }
    else if (argc > 1 && std::string(argv[1]) == std::string("-s"))
    {
      db.populateSession();
      db.show(db.session());
      db.session()->save(RealValue<DUnicodeString>("db.sav"));
    }
    else
    {
      std::cout << "Load database : -l" << std::endl
                << "Save database : -s" << std::endl;       
      return (1);
    }
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
