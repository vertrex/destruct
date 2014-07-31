#include <iostream>

#include "db.hpp"
#include "dexception.hpp"
#include "dtype.hpp"
#include "drealvalue.hpp"
#include "dsimpleobject.hpp"
#include "ddynamicobject.hpp"
#include "protocol/dstream.hpp"
#include "protocol/dmutableobject.hpp"
#include "protocol/dserialize.hpp"

DB::DB() : __destruct(Destruct::Destruct::instance())
{
  Destruct::DType::init();
  this->declare();
}

DB::~DB()
{
  Destruct::DType::clean();
}

Destruct::DObject*    DB::fakeDB(void)
{
  //DObject* processusArguments = Destruct::Destruct::instance().generate("")
  Destruct::DObject* db = this->__destruct.generate("DVectorObject"); 

  /* local */
  Destruct::DObject* local = this->__destruct.generate("LocalArguments"); 

  Destruct::DObject* localPath = this->__destruct.generate("DVectorString");
  localPath->call("push", Destruct::RealValue<Destruct::DUnicodeString>("/home/vertrex/dump/case/cfreds/cfreds.raw.raw"));
  local->setValue("path", Destruct::RealValue<Destruct::DObject*>(localPath));

  Destruct::DObject* moduleArguments = this->__destruct.generate("ModuleArguments");
  moduleArguments->setValue("moduleName", Destruct::RealValue<Destruct::DUnicodeString>("local"));
  moduleArguments->setValue("arguments", Destruct::RealValue<Destruct::DObject*>(local));

  db->call("push", Destruct::RealValue<Destruct::DObject*>(moduleArguments));

  /* partitions */
  Destruct::DObject* partition = this->__destruct.generate("PartitionArguments");
  partition->setValue("file", Destruct::RealValue<Destruct::DUnicodeString>("cfreds.raw.raw/Partitons 1"));

  moduleArguments = this->__destruct.generate("ModuleArguments");
  moduleArguments->setValue("moduleName", Destruct::RealValue<Destruct::DUnicodeString>("partition"));
  moduleArguments->setValue("arguments", Destruct::RealValue<Destruct::DObject*>(partition));

  db->call("push", Destruct::RealValue<Destruct::DObject*>(moduleArguments));

  return (db);
}


void    DB::declare(void)
{
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

void    DB::save(Destruct::DUnicodeString const& filePath)
{
  Destruct::DObject* db = this->fakeDB();

  Destruct::DMutableObject* arg = static_cast<Destruct::DMutableObject*>(this->__destruct.generate("DMutable"));
  arg->setValueAttribute(Destruct::DType::DUnicodeStringType, "filePath", Destruct::RealValue<Destruct::DUnicodeString>(filePath)); 
  arg->setValueAttribute(Destruct::DType::DInt8Type, "input",  Destruct::RealValue<DInt8>(Destruct::DStream::Output));

  Destruct::DStream* dstream = static_cast<Destruct::DStream*>(this->__destruct.generate("DStream", Destruct::RealValue<Destruct::DObject*>(arg)));
  Destruct::DSerialize* serializer = Destruct::DSerializers::to("Binary");

  std::cout << "Showing database " << std::endl;
  this->show(db); 

  std::cout << "Saving database to file " << filePath << std::endl; 
  serializer->serialize(*dstream, *db);
  std::cout << "Saved " << filePath << std::endl; 

  delete dstream;
}

void    DB::load(Destruct::DUnicodeString const& filePath) const
{
  std::cout << "Loading database from file " << filePath << std::endl;

  Destruct::DMutableObject* arg = static_cast<Destruct::DMutableObject*>(this->__destruct.generate("DMutable"));
  arg->setValueAttribute(Destruct::DType::DUnicodeStringType, "filePath", Destruct::RealValue<Destruct::DUnicodeString>(filePath)); 
  arg->setValueAttribute(Destruct::DType::DInt8Type, "input",  Destruct::RealValue<DInt8>(Destruct::DStream::Input));

  Destruct::DStream* dstream = static_cast<Destruct::DStream*>(this->__destruct.generate("DStream", Destruct::RealValue<Destruct::DObject*>(arg)));
  Destruct::DSerialize* serializer = Destruct::DSerializers::to("Binary");

  Destruct::DValue value = serializer->deserialize(*dstream, Destruct::DType::DObjectType);
  std::cout << "Loaded ! found first object : " << value.asUnicodeString() << std::endl; 
  std::cout << "Showing database " << std::endl;
  this->show(value.get<Destruct::DObject*>());

  delete dstream;
}

void    DB::show(Destruct::DObject* object) const
{
  Destruct::DStream* cout = static_cast<Destruct::DStream*>(this->__destruct.generate("DStreamCout"));
  Destruct::DSerializers::to("Text")->serialize(*cout, *object);
}

int main(int argc, char** argv)
{
  DB    db;
 
  try 
  {
    if (argc > 1 && std::string(argv[1]) == std::string("-l"))
      db.load("db.sav");
    else if (argc > 1 && std::string(argv[1]) == std::string("-s"))
      db.save("db.sav");
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
