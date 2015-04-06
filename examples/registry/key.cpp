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
#include "regf.hpp"

#include "streamvfile.hpp"

using namespace Destruct;

/**
 *  RegistryKey
 */
RegistryKey::RegistryKey(DStruct* dstruct, DValue const& args) : DCppObject<RegistryKey>(dstruct, args)
{
  this->init();
  
  this->timestamp = new RegfTime64(Destruct::DStructs::instance().find("RegfTime64"), RealValue<DObject*>(DNone));
  this->keyName = new NameLength(Destruct::DStructs::instance().find("NameLength"), RealValue<DObject*>(this));
  ((DObject*)this->keyName)->setValue("attributeKeyName", RealValue<DUnicodeString>("keyNameLength"));

  this->subkeys = new Subkeys(Destruct::DStructs::instance().find("Subkeys"), RealValue<DObject*>(this));
  this->values = new RegistryValues(Destruct::DStructs::instance().find("RegistryValues"), RealValue<DObject*>(this));
  ((DObject*)this->timestamp)->addRef();
  ((DObject*)this->keyName)->addRef();
  ((DObject*)this->subkeys)->addRef();
  ((DObject*)this->values)->addRef();
}

RegistryKey::~RegistryKey(void)
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
  DStream* stream = static_cast<DStream*>(arg.get<DObject*>());

  DUInt16 size = ((DObject*)this->parent)->getValue(this->attributeKeyName).get<DUInt16>();

  char keyNameBuff[size];
  stream->read(keyNameBuff, size);
  stream->destroy();

  this->keyName = DUnicodeString(std::string(keyNameBuff, size));

  return (RealValue<DUInt8>(1));
}


/**
 * Subkeys
 */
Subkeys::Subkeys(DStruct* dstruct, DValue const& args) : DCppObject<Subkeys>(dstruct, args), __size(0)
{
  this->init();
  this->parent = args.get<DObject* >();
  ((DObject*)this->parent)->addRef();
  this->list = Destruct::DStructs::instance().generate("DVectorObject");
  ((DObject*)list)->addRef();
}

Subkeys::~Subkeys(void)
{
}

DValue    Subkeys::deserializeRaw(DValue const& arg)
{
  StreamFile* stream = static_cast<StreamFile*>(arg.get<DObject*>());
  DStruct* keyStruct = Destruct::DStructs::instance().find("RegistryKey"); 

  //XXX IF SUBKEY COUNT IN PARENT ?
  DUInt32 parentSubkeyCount = ((DObject*)this->parent)->getValue("subkeyCount").get<DUInt32>();
  DUInt32 subkeyListOffset = ((DObject*)this->parent)->getValue("subkeyListOffset").get<DUInt32>();
  if (parentSubkeyCount == 0 || subkeyListOffset == 0xffffffff)
  {
    stream->destroy();    
    return (RealValue<DUInt8>(1));
  }

  stream->seek(subkeyListOffset + 0x1000); 
  
  //size.unserialize(*stream);
  //signature.unserialize(*stream);
  //subkeyCount.unserialize(*stream);
  // 
  //new serialization XXX
  stream->read(size);
  stream->read(signature);
  stream->read(subkeyCount); 

  Destruct::DSerialize* serializer = Destruct::DSerializers::to("Raw");

  if (signature == 0x686c || signature == 0x666c || signature == 0x6972 || signature == 0x696c)
  {
    for (uint32_t index = 0; index < subkeyCount; ++index)
    {
      RealValue<DUInt32> subkeyOffset, subkeyChecksum;
      //subkeyOffset.unserialize(*stream); //new serializaiton XXX
      stream->read(subkeyOffset);
      if (signature == 0x686c || signature == 0x666c) //LH || LI 
      {
        //subkeyChecksum.unserialize(*stream);//new serialization XXX
        stream->read(subkeyChecksum);
      }
   
      DObject* subkey = keyStruct->newObject();
      int64_t currentOffset = stream->tell();
      stream->seek(subkeyOffset + 0x1000);
      serializer->deserialize(*stream, subkey);
      stream->seek(currentOffset);

      ((DObject*)this->list)->call("push", RealValue<DObject*>(subkey)); 
    }

  }
  else
  {
     std::ios::fmtflags flags(std::cout.flags()); 
     std::cout << "Key bad signature" << std::hex << signature << std::endl;
     std::cout.flags(flags);
  }

  stream->destroy();
  delete serializer;

  return (RealValue<DUInt8>(1));
}

