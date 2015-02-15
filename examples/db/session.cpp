#include "session.hpp"
#include "dstructs.hpp"
#include "protocol/dstream.hpp"
#include "protocol/dserialize.hpp"
#include "protocol/dmutableobject.hpp"

Session::Session(DStruct* dstruct, Destruct::DValue const& args) : DCppObject<Session>(dstruct, args), __destruct(Destruct::DStructs::instance())
{
  this->init();
}

Session::~Session()
{
  std::cout << "~Session" << std::endl;
}

void    Session::save(Destruct::DValue const& filePath)
{
  std::cout << "Session::SAVE" << std::endl;
  Destruct::DMutableObject* arg = static_cast<Destruct::DMutableObject*>(this->__destruct.generate("DMutable"));
  arg->setValueAttribute(Destruct::DType::DUnicodeStringType, "filePath", filePath); 
  arg->setValueAttribute(Destruct::DType::DInt8Type, "input",  Destruct::RealValue<DInt8>(Destruct::DStream::Output));
  Destruct::DStream* dstream = static_cast<Destruct::DStream*>(this->__destruct.generate("DStream", Destruct::RealValue<Destruct::DObject*>(arg)));
  arg->destroy();
  Destruct::DSerialize* serializer = Destruct::DSerializers::to("Binary");


  std::cout << "Saving database to file " << filePath.asUnicodeString() << std::endl; 
  //std::cout << "session deserialize" << std::endl;
  serializer->serialize(*dstream, this->modules); //passe pas par DValue donc destroy psa this
  std::cout << "Saved " << filePath.asUnicodeString() << std::endl; 
  dstream->destroy();
  delete serializer;
  //delete dstream;
}
