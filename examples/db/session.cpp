#include "session.hpp"
#include "dstructs.hpp"
#include "protocol/dstream.hpp"
#include "protocol/dmutableobject.hpp"

Session::Session(DStruct* dstruct, DValue const& args) : DCppObject<Session>(dstruct, args), __destruct(DStructs::instance())
{
  this->init();
}

Session::~Session()
{
  std::cout << "~Session" << std::endl;
}

void    Session::save(DValue const& filePath)
{
  std::cout << "Session::SAVE" << std::endl;
  DMutableObject* arg = static_cast<DMutableObject*>(this->__destruct.generate("DMutable"));
  arg->setValueAttribute(DType::DUnicodeStringType, "filePath", filePath); 
  arg->setValueAttribute(DType::DInt8Type, "input",  RealValue<DInt8>(DStream::Output));
  DObject* stream = this->__destruct.generate("DStream", RealValue<DObject*>(arg));
 
  arg->destroy();
  DObject* serializer = this->__destruct.generate("SerializeBinary", RealValue<DObject*>(stream));

  std::cout << "Saving dstruct " << std::endl;

  DStructs& dstructs = DStructs::instance();
  DStruct* localArgumentsStruct = dstructs.find("LocalArguments");
  DStruct* moduleArgumentStruct = dstructs.find("ModuleArguments");
  DValue partitionArgValue = RealValue<DStruct*>(dstructs.find("PartitionArguments"));

  DValue localArgValue = RealValue<DStruct*>(localArgumentsStruct);
  DValue moduleArgValue = RealValue<DStruct*>(moduleArgumentStruct);

  /**
   *  serialize struct
   */

  //DStruct* test = localArgValue.get<DStruct*>();
  serializer->call("DStruct", RealValue<DStruct*>(localArgumentsStruct));

  //serializer->serialize(*dstream, *localArgumentsStruct);  

  /**
   *  Serialize vector struct
   */

  DObject* vectorStruct = dstructs.generate("DVectorStruct");
  vectorStruct->call("push", moduleArgValue);
  vectorStruct->call("push", partitionArgValue);

  //serializer->serialize(*dstream, vectorStruct);
  serializer->call("DObject", RealValue<DObject*>(vectorStruct));

  std::cout << "Saving database to file " << filePath.asUnicodeString() << std::endl; 
  //std::cout << "session deserialize" << std::endl;
  serializer->call("DObject", this->modules);

  //serializer->serialize(*dstream, this->modules); //passe pas par DValue donc destroy psa this
  std::cout << "Saved " << filePath.asUnicodeString() << std::endl; 
  stream->destroy();
  serializer->destroy();
  //delete serializer;
  //delete dstream;
}
