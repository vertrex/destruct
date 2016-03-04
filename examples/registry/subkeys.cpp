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
  this->parent = args;
  this->list = Destruct::DStructs::instance().generate("DVectorObject");
}

Subkeys::~Subkeys(void)
{
}

DValue    Subkeys::deserializeRaw(DValue const& arg)
{
  DObject* deserializer = arg;
  DObject* stream = deserializer->getValue("stream");

  //XXX IF SUBKEY COUNT IN PARENT ? //if nk only ? 
  DUInt32 parentSubkeyCount = ((DObject*)this->parent)->getValue("subkeyCount"); //passer en paremetre car si non c la loose pour les ri
  DUInt32 subkeyListOffset = ((DObject*)this->parent)->getValue("subkeyListOffset"); //check ds le parent
  if (parentSubkeyCount == 0 || subkeyListOffset == 0xffffffff)
  {
    deserializer->destroy();
    stream->destroy();
    return  RealValue<DObject*>(this);
  }

  stream->call("seek", RealValue<DUInt64>(subkeyListOffset + 0x1000)); 

  //XXX hbin cell  (multi form can't know yet)
  size = deserializer->call("DInt32");
  signature = deserializer->call("DUInt16");
  //e

                   //"lh"                 //"lf"                  "ri"                  "li"
  DStruct* namedKeyStruct = Destruct::DStructs::instance().find("NamedKey"); 
  if (signature == 0x686c || signature == 0x666c || signature == 0x6972 || signature == 0x696c)
  { 
    subkeyCount = deserializer->call("DUInt16");
    for (uint32_t index = 0; index < subkeyCount; ++index)
    {
      if (signature == 0x6972) //special case sublist of itself ...
      {
         DUnicodeString kname = ((DObject*)this->parent)->getValue("name");
         std::cout << " ri on " << kname << std::endl;
         RealValue<DUInt32> subKeyListOffset = deserializer->call("DUInt32");
         DUInt64 currentOffset = stream->call("tell");  //XXX parse all put in lsit then create after ? rather than seeking ?
         //next sublist 
       
         //DObject*  subkeys = Destruct().find("Subkeys").newObject(RealValue<DObject*>(this->parent)); //XXX nous ou le paren ?? ca devient compliquer :) mais bien gere ca doit se faire recup la list et push tous ds notre parent car elle doivent etre merge sur une clef !
         //this->deserializer("DObject", subkeys);
 
         stream->call("seek", RealValue<DUInt64>(currentOffset));
      }
      else 
      {
        RealValue<DUInt32> subkeyOffset, subkeyChecksum;
        subkeyOffset = deserializer->call("DUInt32");   

        if (signature == 0x686c || signature == 0x666c) //LH || LF
          subkeyChecksum = deserializer->call("DUInt32");
  
      
        DUInt64 currentOffset = stream->call("tell"); //parse all offset and then create from list rather than seeking we reed this metada structure and other use it to create the nk object 
        stream->call("seek", RealValue<DUInt64>(subkeyOffset + 0x1000));

        //DUInt32 subHbinSize = deserializer->call("DUInt32");
        deserializer->call("DUInt32");
        DUInt16 subKeySignature = deserializer->call("DUInt16");

        if (subKeySignature == 0x6b6e) //nk
        { 
          stream->call("seek", RealValue<DUInt64>(subkeyOffset + 0x1000));
          DObject* subkey = namedKeyStruct->newObject();
          deserializer->call("DObject", RealValue<DObject*>(subkey)); //throw if not nk ?
          ((DObject*)this->list)->call("push", RealValue<DObject*>(subkey)); 
        }
        //else 
        //std::cout << "found subkey signature " << subKeySignature << std::endl; 
        //strange sometime is lh but that's all !
        stream->call("seek", RealValue<DUInt64>(currentOffset));
      }
    }
  }
  else
  {
     std::ios::fmtflags flags(std::cout.flags()); 
     std::cout << "Key bad signature" << std::hex << signature << std::endl;
     std::cout.flags(flags);
  }

  stream->destroy();
  deserializer->destroy();

  return (RealValue<DObject*>(this));
}

