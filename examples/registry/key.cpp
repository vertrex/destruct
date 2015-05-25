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
  DObject* deserializer = arg.get<DObject*>();

  DUInt16 size = ((DObject*)this->parent)->getValue(this->attributeKeyName).get<DUInt16>();

  DBuffer buffer = deserializer->getValue("stream").get<DObject*>()->call("read", RealValue<DInt64>((DInt64)size)).get<DBuffer>();
  
  this->keyName = DUnicodeString(std::string((char*)buffer.data(), size));

  return RealValue<DObject*>(this);
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
  DObject* deserializer = arg.get<DObject*>();
  DObject* stream = deserializer->getValue("stream").get<DObject*>();

  DStruct* keyStruct = Destruct::DStructs::instance().find("RegistryKey"); 

  //XXX IF SUBKEY COUNT IN PARENT ?
  DUInt32 parentSubkeyCount = ((DObject*)this->parent)->getValue("subkeyCount").get<DUInt32>();
  DUInt32 subkeyListOffset = ((DObject*)this->parent)->getValue("subkeyListOffset").get<DUInt32>();
  if (parentSubkeyCount == 0 || subkeyListOffset == 0xffffffff)
  {
    deserializer->destroy();
    stream->destroy();
    return  RealValue<DObject*>(this);
  }

  stream->call("seek", RealValue<DUInt64>(subkeyListOffset + 0x1000)); 
  
  size = deserializer->call("DInt32");
  signature = deserializer->call("DUInt16");

                   //"lh"                 //"lf"                  "ri"                  "li"
  if (signature == 0x686c || signature == 0x666c || signature == 0x6972 || signature == 0x696c)
  { 

    subkeyCount = deserializer->call("DUInt16");
    for (uint32_t index = 0; index < subkeyCount; ++index)
    {
      RealValue<DUInt32> subkeyOffset, subkeyChecksum;
      subkeyOffset = deserializer->call("DUInt32");   

      if (signature == 0x686c || signature == 0x666c) //LH || LI 
        subkeyChecksum = deserializer->call("DUInt32");
   
      DUInt64 currentOffset = stream->call("tell").get<DUInt64>();

      DObject* subkey = keyStruct->newObject();
      stream->call("seek", RealValue<DUInt64>(subkeyOffset + 0x1000));
      deserializer->call("DObject", RealValue<DObject*>(subkey));
      ((DObject*)this->list)->call("push", RealValue<DObject*>(subkey)); 

      stream->call("seek", RealValue<DUInt64>(currentOffset));
    }

  }
  else if (signature == 0x6b6e) // "nk" named key
  {
    //TODO
    std::cout << "signature nk " << std::endl;
    if (size < 1)
      size = size * -1;
    DUInt64 pos = stream->call("tell").get<DUInt64>() + size - 8;
    std::cout << "seek to " << pos << std::endl;
    stream->call("seek", RealValue<DUInt64>(pos)); //substract size and signature from size
  }
  //else if (signature == 0x6b73) //"sk" security key
  //{
    //TODO
    // 
    //}
    //else if (signature == 0x6264) //"db" data block key
    //{
    ////TODO
    //}
  else
  {
     std::cout << "Bad signature n key " << subkeyCount << " size " << size << std::endl;
     std::ios::fmtflags flags(std::cout.flags()); 
     std::cout << "Key bad signature" << std::hex << signature << std::endl;
     std::cout.flags(flags);
  }

  stream->destroy();
  deserializer->destroy();

  return (RealValue<DObject*>(this));
}

