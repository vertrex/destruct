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
#include <iostream>

#include "key.hpp"
#include "value.hpp"
//#include "regf.hpp"

#include "streamvfile.hpp"
#include "registry.hpp"

using namespace Destruct;

/**
 * RegistryValues
 */
RegistryValues::RegistryValues(DStruct* dstruct, DValue const& args) : DCppObject<RegistryValues>(dstruct, args), __size(0)
{
  this->init();
  this->parent = args.get<DObject* >();
  this->list = Destruct::DStructs::instance().generate("DVectorObject");
  ((DObject*)list)->addRef();
}

RegistryValues::~RegistryValues(void)
{
}

DValue    RegistryValues::deserializeRaw(DValue const& arg)
{
  DObject* deserializer = arg.get<DObject*>();

  DStruct* valueStruct = Destruct::DStructs::instance().find("RegistryValue"); 

  DUInt32 valueCount = ((DObject*)this->parent)->getValue("valueCount").get<DUInt32>();
  DUInt32 valueListOffset = ((DObject*)this->parent)->getValue("valueListOffset").get<DUInt32>();
  if (valueCount == 0 || valueListOffset == 0xffffffff)
  {
    deserializer->destroy();
    return (RealValue<DObject*>(this)); 
  }

  DObject* stream = deserializer->getValue("stream").get<DObject*>();
  stream->call("seek", RealValue<DUInt64>(valueListOffset + 0x1000)); 

  size = deserializer->call("DInt32");

  for (uint32_t index = 0; index < valueCount ; ++index)
  {
    RealValue<DUInt32> subvalueOffset = deserializer->call("DUInt32");
        
    DObject* subvalue = valueStruct->newObject();

    DUInt64 currentOffset = stream->call("tell").get<DUInt64>();
    stream->call("seek", RealValue<DUInt64>(subvalueOffset + 0x1000));

    deserializer->call("DObject", RealValue<DObject*>(subvalue));
    stream->call("seek", RealValue<DUInt64>((DUInt64)currentOffset));
    ((DObject*)this->list)->call("push", RealValue<DObject*>(subvalue)); 
  }

  stream->destroy();
  deserializer->destroy();

  return (RealValue<DObject*>(this));
}

/**
 * RegistryValues
 */
RegistryValue::RegistryValue(DStruct* dstruct, DValue const& args) : DCppObject<RegistryValue>(dstruct, args), __size(0)
{
  this->init();
  this->name = new NameLength(Destruct::DStructs::instance().find("NameLength"), RealValue<DObject*>(this));
  ((DObject*)this->name)->setValue("attributeKeyName", RealValue<DUnicodeString>("nameLength"));
  ((DObject*)this->name)->addRef();
  
  this->data = new RegistryValueData(Destruct::DStructs::instance().find("RegistryValueData"), RealValue<DObject*>(this));
  ((DObject*)this->data)->addRef();
}


RegistryValue::~RegistryValue(void)
{
}

DValue    RegistryValue::deserializeRaw(DValue const& arg)
{
  //Destruct::DStructs& destruct = Destruct::DStructs::instance();

  //destruct.generate(this->dataTypeName[this->dataType]);
  return (RealValue<DObject*>(this));
}

DValue  RegistryValue::valueTypeName(void)
{
  return (RealValue<DUnicodeString>(RegistryValue::registryType(this->dataType)));
}
/**
 * RegistryValueData
 */
RegistryValueData::RegistryValueData(DStruct* dstruct, DValue const& args) : DCppObject<RegistryValueData>(dstruct, args)
{

  this->init();
}

RegistryValueData::~RegistryValueData(void)
{
}


DValue    RegistryValueData::deserializeRaw(DValue const& arg)
{
  return (RealValue<DObject*>(this));
}
