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
  
  this->timestamp = Destruct::DStructs::instance().find("RegfTime64")->newObject();
  this->keyName = Destruct::DStructs::instance().find("NameLength")->newObject(RealValue<DObject*>(this));
  ((DObject*)this->keyName)->setValue("attributeKeyName", RealValue<DUnicodeString>("keyNameLength"));

  this->subkeys = Destruct::DStructs::instance().find("Subkeys")->newObject(RealValue<DObject*>(this));
  this->values = Destruct::DStructs::instance().find("RegistryValues")->newObject(RealValue<DObject*>(this));
}

NamedKey::~NamedKey(void)
{
}

/**
 * NameLength 
 */
NameLength::NameLength(DStruct* dstruct, DValue const& args) : DCppObject<NameLength>(dstruct, args), __size(0)
{
  this->init();
  this->parent = args.get<DObject* >();
}

NameLength::~NameLength(void)
{
}

DValue    NameLength::deserializeRaw(DValue const& arg)
{
  DObject* deserializer = arg.get<DObject*>();

  DUInt16 size = ((DObject*)this->parent)->getValue(this->attributeKeyName).get<DUInt16>();

  DBuffer buffer = deserializer->getValue("stream").get<DObject*>()->call("read", RealValue<DInt64>((DInt64)size)).get<DBuffer>();
  
  this->keyName = DUnicodeString(std::string((char*)buffer.data(), size));

  return RealValue<DObject*>(this);
}
