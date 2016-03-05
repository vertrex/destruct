/*
 * DFF -- An Open Source Digital Forensics Framework
 * Copyright (C) 2009-2011 ArxSys
 * This program is free software, distributed under the terms of
 * the GNU General Public License Version 2. See the LICENSE file
 * at the top of the source tree.
 *  
 * See http: *www.digital-forensic.org for more information about this
 * project. Please do not directly contact any of the maintainers of
 * DFF for assistance; the project provides a web site, mailing lists
 * and IRC channels for your use.
 * 
 * Author(s):
 *  Solal Jacob <sja@digital-forensic.org>
 */

#include "registry.hpp"
#include "registryopt.hpp"
#include "regf.hpp"
#include "subkeys.hpp"
#include "values.hpp"
#include "namedkey.hpp"
#include "valuekey.hpp"
#include "protocol/traceobject.hpp"

#include "client.hpp"


extern "C"
{
  void DestructExport()
  {
    Registry::declare();
  }
}

void    Registry::declare(void)
{
  Destruct::DStructs& destruct = Destruct::DStructs::instance();

  registerDCpp(Registry)
  registerDCpp(RegistryOpt)
  registerDCpp(Regf)
  registerDCpp(RegfName)
  registerDCpp(NamedKey) //RegistryNamedKey
  registerDCpp(Subkeys) //RegistrySubkeys
  registerDCpp(ValueKey)  //RegistryValueKey
  registerDCpp(RegistryValues)
  registerDCpp(RegistryBigData)
  registerDCpp(RegistryDataNone)
  registerDCpp(RegistryDataSZ)
  registerDCpp(RegistryDataMultiSZ)
  registerDCpp(RegistryDataDWord)
  registerDCpp(RegistryDataDWordBE)
  registerDCpp(RegistryDataBinary)
  registerDCpp(RegistryDataQWord)
}

Registry::Registry(DStruct* dstruct, const DValue& args) : DCppObject<Registry>(dstruct),  __destruct(Destruct::DStructs::instance())
{
  this->init();
}

Registry::Registry(Registry const& copy) : DCppObject<Registry>(copy), __destruct(Destruct::DStructs::instance())
{
  this->init();
}

Registry::~Registry()
{
}

DObject*        Registry::open(DValue const& args)
{
  Regf* regf = new Regf(this->__destruct.find("Regf"), RealValue<DObject*>(DNone));
  
  DMutableObject* arg = static_cast<DMutableObject*>(this->__destruct.generate("DMutable"));
  arg->setValueAttribute(DType::DUnicodeStringType, "filePath", args); 
  arg->setValueAttribute(DType::DInt8Type, "input",  RealValue<DInt8>(DStream::Input));
  DObject* stream = this->__destruct.find("DStream")->newObject(RealValue<DObject*>(arg));
  arg->destroy();

  DObject* deserializer = this->__destruct.find("DeserializeRaw")->newObject(RealValue<DObject*>(stream));
  deserializer->call("DObject", RealValue<DObject*>(regf)); //deserializer must destroy object ?

  deserializer->destroy();
  stream->destroy();
  return (regf);
}

void            Registry::toFile(std::string filePath, DObject* object, std::string type)
{
  Destruct::DStructs& destruct = Destruct::DStructs::instance();

  DMutableObject* arg = static_cast<DMutableObject*>(destruct.generate("DMutable"));
  arg->setValueAttribute(DType::DUnicodeStringType, "filePath", RealValue<DUnicodeString>(filePath)); 
  arg->setValueAttribute(DType::DInt8Type, "input", RealValue<DInt8>(DStream::Output));
  DObject* dstream = destruct.generate("DStream", RealValue<DObject*>(arg));
  arg->destroy();
 
  DObject* serializer = destruct.find("Serialize" + type)->newObject(RealValue<DObject*>(dstream));
  serializer->call("DObject", RealValue<DObject*>(object));
  serializer->destroy();
  dstream->destroy(); 
}

void            Registry::show(DObject* object)
{
  Destruct::DStructs& destruct = Destruct::DStructs::instance();
  DObject* dstream = destruct.generate("DStreamCout", RealValue<DObject*>());
 
  DObject* serializer = destruct.find("SerializeText")->newObject(RealValue<DObject*>(dstream));
  serializer->call("DObject", RealValue<DObject*>(object));
  serializer->destroy();
  dstream->destroy(); 
}
