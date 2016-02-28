#include <iostream>

#include "datatype.hpp"
#include "dstructs.hpp"
#include "drealvalue.hpp"
#include "dvalue.hpp"
#include "dobject.hpp"
#include "dnullobject.hpp"
#include "protocol/dmutableobject.hpp"
#include "protocol/dstream.hpp"
#include "protocol/dserialize.hpp"

extern "C"
{
  EXPORT void DestructExport(void)
  {
    FileType::declare();
  }
}

/**
 * DataType
 **/

DataType::DataType(DStruct* dstruct, DValue const& args) : DCppObject<DataType>(dstruct, args)
{
  this->init();
}

DValue DataType::testType(DValue const& args)
{
  DStructs& dstructs = DStructs::instance();
  DStream* stream = (DStream*)args.get<DObject*>();
  DSerialize* serializer = DSerializers::to("Raw");

  try
  {
    DStruct* dstruct = dstructs.find("DVectorObject");
    DObject* object = dstruct->newObject();
    serializer->deserialize(*stream, object);

    object->call("size");
    std::cout << "it 's compatible with " << object->instanceOf()->name() << std::endl;
    object->destroy();
  }
  catch (DException const& error)
  {
  }

  //for each struct:
  //try:
  //DSerialize(stream, structs
  //result.addType(type)
  //except Error:
  //pass 
        

  return RealValue<DUnicodeString>("Unknown");
}

/**
 * FileType
 **/
FileType::FileType() : __dstructs(DStructs::instance())
{
  DType::init();
  FileType::declare();

  this->__dataType = this->__dstructs.find("DataType")->newObject();
}

void    FileType::showType(const std::string& filePath)
{
  DMutableObject* arg = static_cast<Destruct::DMutableObject*>(this->__dstructs.generate("DMutable"));
  arg->setValueAttribute(DType::DUnicodeStringType, "filePath", RealValue<DUnicodeString>(filePath)); 
  arg->setValueAttribute(DType::DInt8Type, "input", RealValue<DInt8>(DStream::Input));
  DStream* dstream = static_cast<DStream*>(this->__dstructs.generate("DStream", RealValue<DObject*>(arg)));

  RealValue<DUnicodeString> foundType =((DObject*)this->__dataType)->call("testType", RealValue<DObject*>(dstream)).get<DUnicodeString>();
  std::cout <<  filePath << " : " << foundType << std::endl;;
}

FileType::~FileType()
{
  DType::clean();
}

void FileType::declare(void)
{
  Destruct::DStructs&  destruct = Destruct::DStructs::instance();
  Destruct::DStruct* dataType = makeNewDCpp<DataType>("DataType");
  destruct.registerDStruct(dataType);     
}

void showType(std::string const& filePath)
{

}
