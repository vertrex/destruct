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

#include "streamvfile.hpp"
#include "client.hpp"


extern "C"
{
  void declare()
  {
    Registry::declare();
  }
}

void    Registry::declare(void)
{
  Destruct::DStructs& destruct = Destruct::DStructs::instance();

  registerDCpp(Registry)
  //registerDCpp(StreamFile)
  registerDCpp(RegistryOpt)
  registerDCpp(Regf)
  registerDCpp(RegfName)
  registerDCpp(RegfTime64)
  registerDCpp(NamedKey)
  registerDCpp(NameLength)
  registerDCpp(Subkeys)
  registerDCpp(ValueKey)
  registerDCpp(RegistryValues)
  registerDCpp(RegistryValueData)
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
  DObject* streamVFile = this->__destruct.find("DStream")->newObject(RealValue<DObject*>(arg));
  arg->destroy();

  DObject* deserializer = this->__destruct.find("DeserializeRaw")->newObject(RealValue<DObject*>(streamVFile));
  deserializer->call("DObject", RealValue<DObject*>(regf));

  if (regf->validate().get<DUInt8>())
    std::cout << "Registry file is valid" << std::endl;
  else
    std::cout << "Registry file is invalid" << std::endl;

  NamedKey* key = new NamedKey(this->__destruct.find("NamedKey"), RealValue<DObject*>(DNone));

  DUInt64 x = 0x1000 + regf->keyrecord; //XXX put in Regf 
  streamVFile->call("seek", RealValue<DUInt64>(x));
  deserializer->call("DObject", RealValue<DObject*>(key));
  regf->key = key;

  key->destroy();  
  deserializer->destroy();
  streamVFile->destroy();
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

/** Loading and saving method **/
bool                    Registry::load(DValue value)
{
  return (false);
}

DValue   Registry::save(void) const
{
  return (RealValue<DObject*>(DNone));
}
