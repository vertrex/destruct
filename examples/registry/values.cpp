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

#include "registry.hpp"
#include "values.hpp"


using namespace Destruct;

/**
 * RegistryValues
 */
RegistryValues::RegistryValues(DStruct* dstruct, DValue const& args) : DCppObject<RegistryValues>(dstruct, args), __size(0)
{
  this->init();
  this->valueCount = args;
  this->list = Destruct::DStructs::instance().generate("DVectorObject");
  this->minor = 5;
}

RegistryValues::~RegistryValues()
{
}

DObject* RegistryValues::deserializeRaw(DValue const& arg)
{
  DObject* deserializer = arg;
  DObject* stream = deserializer->getValue("stream");
  DStruct* valueStruct = Destruct::DStructs::instance().find("ValueKey"); 

  size = deserializer->call("DInt32");
  for (uint32_t index = 0; index < this->valueCount ; ++index)
  {
    RealValue<DUInt32> subvalueOffset = deserializer->call("DUInt32");
    DUInt64 currentOffset = stream->call("tell");

    stream->call("seek", RealValue<DUInt64>(subvalueOffset + 0x1000));
    DObject* subvalue = valueStruct->newObject();
    subvalue->setValue("minor", this->minor);
    deserializer->call("DObject", RealValue<DObject*>(subvalue));
    ((DObject*)this->list)->call("push", RealValue<DObject*>(subvalue)); 
    subvalue->destroy();

    stream->call("seek", RealValue<DUInt64>((DUInt64)currentOffset));
  }

  stream->destroy();
  deserializer->destroy();

  return (this);
}

