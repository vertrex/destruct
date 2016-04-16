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
#include "regf.hpp"
#include "subkeys.hpp"
#include "values.hpp"
#include "namedkey.hpp"
#include "valuekey.hpp"
#include "protocol/traceobject.hpp"

#include "client.hpp"


extern "C"
{
  EXPORT void DestructExport()
  {
    Registry::declare();
  }
}

void    Registry::declare(void)
{
  Destruct::DStructs& destruct = Destruct::DStructs::instance();

  registerDCpp(Registry)
  registerDCpp(Regf)
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

Registry::Registry(Registry const& rhs) : DCppObject<Registry>(rhs), __destruct(Destruct::DStructs::instance())
{
  this->copy(this, rhs);
}

Registry::~Registry()
{
}

DObject*        Registry::open(DValue const& args)
{
  RealValue<DObject*> regf = new Regf(this->__destruct.find("Regf"), RealValue<DObject*>(DNone));
  
  DMutableObject* arg = static_cast<DMutableObject*>(this->__destruct.generate("DMutable"));
  arg->setValueAttribute(DType::DUnicodeStringType, "filePath", args); 
  arg->setValueAttribute(DType::DInt8Type, "input",  RealValue<DInt8>(DStream::Input));
  RealValue<DObject*> stream = this->__destruct.find("DStream")->newObject(RealValue<DObject*>(arg));
  arg->destroy();

  RealValue<DObject*> deserializer = this->__destruct.find("DeserializeRaw")->newObject(stream);
  ((DObject*)stream)->destroy();
  ((DObject*)deserializer)->call("DObject", regf);
        

  return (regf);
}
