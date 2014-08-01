#include "session.hpp"
#include "destruct.hpp"
#include "protocol/dstream.hpp"
#include "protocol/dserialize.hpp"
#include "protocol/dmutableobject.hpp"

Session::Session(DStruct* dstruct, Destruct::DValue const& args) : DCppObject<Session>(dstruct, args), __destruct(Destruct::Destruct::instance())
{
  this->init();
}

Session::~Session()
{
}

void    Session::save(Destruct::DValue const& filePath)
{
  Destruct::DMutableObject* arg = static_cast<Destruct::DMutableObject*>(this->__destruct.generate("DMutable"));
  arg->setValueAttribute(Destruct::DType::DUnicodeStringType, "filePath", filePath); 
  arg->setValueAttribute(Destruct::DType::DInt8Type, "input",  Destruct::RealValue<DInt8>(Destruct::DStream::Output));

  Destruct::DStream* dstream = static_cast<Destruct::DStream*>(this->__destruct.generate("DStream", Destruct::RealValue<Destruct::DObject*>(arg)));
  Destruct::DSerialize* serializer = Destruct::DSerializers::to("Binary");

  std::cout << "Saving database to file " << filePath.asUnicodeString() << std::endl; 
  serializer->serialize(*dstream, *this);
  std::cout << "Saved " << filePath.asUnicodeString() << std::endl; 

  delete dstream;
}
