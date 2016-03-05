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

#include "namedkey.hpp"

using namespace Destruct;

/**
 *  NamedKey
 */
NamedKey::NamedKey(DStruct* dstruct, DValue const& args) : DCppObject<NamedKey>(dstruct, args)
{
  this->init();
}

NamedKey::~NamedKey(void)
{
}

DObject*        NamedKey::deserializeRaw(DValue const& args)
{
  DObject* deserializer = args;
  DObject* stream = deserializer->getValue("stream");

  this->size = deserializer->call("DInt32");
  this->signature = deserializer->call("DUInt16");
  this->keyType = deserializer->call("DUInt16");
  this->timestamp = deserializer->call("DUInt64");
  this->unknown1 = deserializer->call("DUInt32");
  this->parentKeyOffset = deserializer->call("DUInt32");
  this->subkeyCount = deserializer->call("DUInt32");
  this->subkeyCountVolatile = deserializer->call("DUInt32");
  this->subkeyListOffset = deserializer->call("DUInt32");
  this->subkeyListOffsetVolatile = deserializer->call("DUInt32");
  this->valueCount = deserializer->call("DUInt32");
  this->valueListOffset  = deserializer->call("DUInt32");
  this->securityDescriptorOffset = deserializer->call("DUInt32");
  this->classNameOffset = deserializer->call("DUInt32");
  this->subkeyNameMaximumLength = deserializer->call("DUInt32");
  this->subkeyClassNameMaximumLength = deserializer->call("DUInt32");
  this->valueNameMaximumLength = deserializer->call("DUInt32");
  this->valueDataMaximumSize = deserializer->call("DUInt32");
  this->unknown2 = deserializer->call("DUInt32");
  this->keyNameLength = deserializer->call("DUInt16");
  this->classNameLength = deserializer->call("DUInt16");

  DBuffer buffer = stream->call("read", RealValue<DInt64>((DInt64)this->keyNameLength));
  this->name = DUnicodeString(std::string((char*)buffer.data(), this->keyNameLength)); //type UTF ?

  this->subkeys = Destruct::DStructs::instance().find("Subkeys")->newObject();
  if (this->subkeyCount != 0 && this->subkeyListOffset != 0xffffffff)
  {
    stream->call("seek", RealValue<DUInt64>(this->subkeyListOffset + 0x1000)); 
    deserializer->call("DObject", subkeys);
  }

  this->values = Destruct::DStructs::instance().find("RegistryValues")->newObject(this->valueCount);
  if (this->valueCount != 0 && this->valueListOffset != 0xffffffff)
  {
    stream->call("seek", RealValue<DUInt64>(this->valueListOffset + 0x1000)); 
    deserializer->call("DObject", values);
  }

  deserializer->destroy();
  stream->destroy();
  return (this);
}

