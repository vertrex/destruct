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
  Destruct::DType::clean();
}

Destruct::DObject*  DB::populateSession(void)
{
  this->__session = static_cast<Session*>(this->__destruct.generate("Session"));
  this->__session->modules = this->__destruct.generate("DVectorObject"); 
  /* local */
  Destruct::DObject* local = this->__destruct.generate("LocalArguments"); 

  Destruct::DObject* localPath = this->__destruct.generate("DVectorString");
  localPath->call("push", Destruct::RealValue<Destruct::DUnicodeString>("/home/vertrex/dump/case/cfreds/cfreds.raw.raw"));
  local->setValue("path", Destruct::RealValue<Destruct::DObject*>(localPath));

  Destruct::DObject* moduleArguments = this->__destruct.generate("ModuleArguments");
  moduleArguments->setValue("moduleName", Destruct::RealValue<Destruct::DUnicodeString>("local"));
  moduleArguments->setValue("arguments", Destruct::RealValue<Destruct::DObject*>(local));

  ((DObject*)this->__session->modules)->call("push", Destruct::RealValue<Destruct::DObject*>(moduleArguments));

  /* partitions */
  Destruct::DObject* partition = this->__destruct.generate("PartitionArguments");
  partition->setValue("file", Destruct::RealValue<Destruct::DUnicodeString>("cfreds.raw.raw/Partitons 1"));

  moduleArguments = this->__destruct.generate("ModuleArguments");
  moduleArguments->setValue("moduleName", Destruct::RealValue<Destruct::DUnicodeString>("partition"));
  moduleArguments->setValue("arguments", Destruct::RealValue<Destruct::DObject*>(partition));

  ((DObject*)this->__session->modules)->call("push", Destruct::RealValue<Destruct::DObject*>(moduleArguments));

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
  Destruct::DSerializers::to("Text")->serialize(*cout, *object);
}

void            DB::load(Destruct::DValue const& filePath)
{
  std::cout << "Loading database from file " << filePath.asUnicodeString() << std::endl;

  Destruct::DMutableObject* arg = static_cast<Destruct::DMutableObject*>(this->__destruct.generate("DMutable"));
  arg->setValueAttribute(Destruct::DType::DUnicodeStringType, "filePath", filePath); 
  arg->setValueAttribute(Destruct::DType::DInt8Type, "input",  Destruct::RealValue<DInt8>(Destruct::DStream::Input));

  Destruct::DStream* dstream = static_cast<Destruct::DStream*>(this->__destruct.generate("DStream", Destruct::RealValue<Destruct::DObject*>(arg)));
  Destruct::DSerialize* serializer = Destruct::DSerializers::to("Binary");

  Destruct::DValue value = serializer->deserialize(*dstream, Destruct::DType::DObjectType);
  std::cout << "Loaded ! found first object : " << value.asUnicodeString() << std::endl; 

  this->__session = static_cast<Session*>(value.get<DObject*>());

  delete dstream;
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
