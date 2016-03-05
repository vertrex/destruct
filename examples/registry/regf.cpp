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

#include "regf.hpp"
#include "registryopt.hpp"

#include "dstructs.hpp"

using namespace Destruct;

/**
 *  Regf
 */
Regf::Regf(DStruct* dstruct, DValue const& args) : DCppObject<Regf>(dstruct, args)
{
  this->init();
}

Regf::~Regf(void)
{
}

DUInt8 Regf::validate(void)
{
  if (((DUInt32)this->regf) == 0x66676572 && (this->sequence1 == this->sequence2))
    return (1);
  return (0);
}

DValue  Regf::name(void)
{
  return (((DObject*)this->regfName)->getValue("filename"));
}

DValue  Regf::version(void)
{
  std::stringstream stringStream;
  stringStream << minor << "." << major;

  return RealValue<DUnicodeString>(stringStream.str());
}

DObject* Regf::deserializeRaw(DValue const& args)
{
  DObject* deserializer = args;
  DObject* stream = deserializer->getValue("stream");

  this->regf = deserializer->call("DUInt32");
  this->sequence1 = deserializer->call("DUInt32");
  this->sequence2 = deserializer->call("DUInt32");
  this->timestamp = deserializer->call("DUInt64");
  this->major = deserializer->call("DUInt32");
  this->minor = deserializer->call("DUInt32");
  this->fileType = deserializer->call("DUInt32");
  this->unknown1 = deserializer->call("DUInt32"); 
  this->keyrecord = deserializer->call("DUInt32");
  this->lasthbin = deserializer->call("DUInt32");
  this->unknown2 = deserializer->call("DUInt32");

  this->regfName = new RegfName(Destruct::DStructs::instance().find("RegfName"), RealValue<DObject*>(DNone));
  deserializer->call("DObject", RealValue<DObject*>(this->regfName));

  if (this->validate() == 0)
    throw DException("Invalid sequence in registry");

  stream->call("seek", RealValue<DUInt64>(this->keyrecord + 0x1000));
  this->key = DStructs::instance().find("NamedKey")->newObject();
  deserializer->call("DObject", this->key);
 
  return (this);
}


/**
 *  RegfName
 */
RegfName::RegfName(DStruct* dstruct, DValue const& args) : DCppObject<RegfName>(dstruct, args)
{
  this->init();
}

RegfName::~RegfName(void)
{
}

DValue    RegfName::deserializeRaw(DValue const& arg)
{
  DObject* deserializer = arg;
  DObject* stream = deserializer->getValue("stream");

  DBuffer buffer = stream->call("read", RealValue<DInt64>(60));
  uint8_t* fileNameBuff = buffer.data();

  uint32_t i = 0;
  for (; i < 58; ++i)
  {
     if (!(i % 2))
       if (fileNameBuff[i] == 0 && fileNameBuff[i+1] == 0)
         break;
  }
  if (i < 58)
    this->fileName = DUnicodeString((char*)fileNameBuff, i, "UTF16-LE");

  return RealValue<DObject*>(this);
}
