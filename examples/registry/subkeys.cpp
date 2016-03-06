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

#include "subkeys.hpp"
#include "regf.hpp"

using namespace Destruct;


/**
 * Subkeys
 */
Subkeys::Subkeys(DStruct* dstruct, DValue const& args) : DCppObject<Subkeys>(dstruct, args), __size(0)
{
  this->init();
  this->list = Destruct::DStructs::instance().generate("DVectorObject");
}

Subkeys::~Subkeys(void)
{
  //std::cout << "~Subkeys" << std::endl;
}

DObject*        Subkeys::deserializeRaw(DValue const& arg)
{
  DObject* deserializer = arg;
  DObject* stream = deserializer->getValue("stream");

  size = deserializer->call("DInt32");
  signature = deserializer->call("DUInt16");

  DStruct* namedKeyStruct = Destruct::DStructs::instance().find("NamedKey"); 
                   //"lh"                 //"lf"                  "ri"                  "li"
  if (signature == 0x686c || signature == 0x666c || signature == 0x6972 || signature == 0x696c)
  { 
    subkeyCount = deserializer->call("DUInt16");
    for (uint32_t index = 0; index < subkeyCount; ++index)
    {
      if (signature == 0x6972) //special case sublist of itself ...
      {
         RealValue<DUInt32> subKeyListOffset = deserializer->call("DUInt32");
         DUInt64 currentOffset = stream->call("tell"); 
         if (subKeyListOffset != 0xffffffff)
         {
           DObject* subkeys = Destruct::DStructs::instance().find("Subkeys")->newObject();
           stream->call("seek", RealValue<DUInt64>(subKeyListOffset + 0x1000)); 
           deserializer->call("DObject", RealValue<DObject*>(subkeys));
           DObject* sublist = subkeys->getValue("list"); 
           DUInt64 count = sublist->call("size");
           for (DUInt64 index = 0; index < count; index++)
           {
            DValue subkey = sublist->call("get", RealValue<DUInt64>(index));
            ((DObject*)this->list)->call("push", subkey);
           }
           sublist->destroy();
           ((DObject*)subkeys)->destroy();
         }
         stream->call("seek", RealValue<DUInt64>(currentOffset));
      }
      else 
      {
        RealValue<DUInt32> subkeyOffset, subkeyChecksum;
        subkeyOffset = deserializer->call("DUInt32");   

        if (signature == 0x686c || signature == 0x666c) //LH || LF
          subkeyChecksum = deserializer->call("DUInt32");
  
      
        DUInt64 currentOffset = stream->call("tell");
        stream->call("seek", RealValue<DUInt64>(subkeyOffset + 0x1000));

        deserializer->call("DUInt32"); //subhbinsize
        DUInt16 subKeySignature = deserializer->call("DUInt16");

        if (subKeySignature == 0x6b6e) //nk
        { 
          stream->call("seek", RealValue<DUInt64>(subkeyOffset + 0x1000));
          DObject* subkey = namedKeyStruct->newObject();
          deserializer->call("DObject", RealValue<DObject*>(subkey)); //throw if not nk ?
          ((DObject*)this->list)->call("push", RealValue<DObject*>(subkey));
          subkey->destroy();
        }
        stream->call("seek", RealValue<DUInt64>(currentOffset));
      }
    }
  }

  stream->destroy();
  deserializer->destroy();

  return (this);
}

