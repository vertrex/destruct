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
#include "key.hpp"
#include "value.hpp"
#include "protocol/debugobject.hpp"

#include "streamvfile.hpp"

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
  registerDCpp(StreamFile)
  registerDCpp(RegistryOpt)
  registerDCpp(Regf)
  registerDCpp(RegfName)
  registerDCpp(RegfTime64)
  registerDCpp(RegistryKey)
  registerDCpp(NameLength)
  registerDCpp(Subkeys)
  registerDCpp(RegistryValue)
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

DValue Registry::open(DValue const& args)
{
  Regf* regf = new Regf(this->__destruct.find("Regf"), RealValue<DObject*>(DNone));
  StreamFile* streamVFile = new StreamFile(this->__destruct.find("StreamFile"), args);

  

  DSerialize* serializer = DSerializers::to("Raw");
  serializer->deserialize(*streamVFile, regf);

  if (regf->validate().get<DUInt8>())
    std::cout << "Registry file is valid" << std::endl;
  else
    std::cout << "Registry file is invalid" << std::endl;

  RegistryKey* rkey = new RegistryKey(this->__destruct.find("RegistryKey"), RealValue<DObject*>(DNone));
  //DObject* key = this->__destruct.generate("RecursiveDebugObject", RealValue<DObject*>(rkey));
  DObject* key = rkey;

  DInt64 x = 0x1000 + regf->keyrecord;
  streamVFile->seek(x);
  serializer->deserialize(*streamVFile, rkey);
  regf->key = key;
  delete serializer;

  return (RealValue<DObject* >(regf));
}

void            Registry::toFile(std::string filePath, DObject* object, std::string type)
{
  Destruct::DStructs& destruct = Destruct::DStructs::instance();

  DMutableObject* arg = static_cast<DMutableObject*>(destruct.generate("DMutable"));
  arg->setValueAttribute(DType::DUnicodeStringType, "filePath", RealValue<DUnicodeString>(filePath)); 
  arg->setValueAttribute(DType::DInt8Type, "input", RealValue<DInt8>(DStream::Output));
  DStream* dstream = static_cast<DStream*>(destruct.generate("DStream", RealValue<DObject*>(arg)));
  arg->destroy();

  DSerialize* serializer = DSerializers::to(type);

  serializer->serialize(*dstream, object);
  delete serializer;
  dstream->destroy(); 
}

void            Registry::show(DObject* object)
{
  DStream* cout = static_cast<DStream*>(Destruct::DStructs::instance().generate("DStreamCout"));
  DSerialize* text = DSerializers::to("Text");

  text->serialize(*cout, object);

  delete text;
  cout->destroy();
  
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
