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

#ifndef __regf_hh__
#define __regf_hh__

#include "registry_common.hpp"

using namespace Destruct;

class Regf : public DCppObject<Regf> //header REGF
{
public:
  Regf(DStruct* dstruct, DValue const& args);
  DValue              time(void);
  DValue              version(void);
  DUInt8              validate(void);
  DObject*            deserializeRaw(DValue const& args);

  RealValue<DUInt32>  regf, sequence1, sequence2, major, minor, fileType, unknown1,
                      keyrecord, lasthbin, unknown2;
  RealValue<DUInt64>  timestamp;
  RealValue<DObject*> key;
  RealValue<DUnicodeString> name;

  attributeCount(Regf, 15)

  attributeList(attribute(DUInt32, regf)
                attribute(DUInt32, sequence1)
                attribute(DUInt32, sequence2) //sequence1 == sequence2 if sync properly
                attribute(DUInt64, timestamp) //FILETIME UTC
                attribute(DUInt32, major) 
                attribute(DUInt32, minor)
                attribute(DUInt32, fileType) //0x0 normal 0x1 transaction log
                attribute(DUInt32, unknown1)
                attribute(DUInt32, keyrecord) //root key offset
                attribute(DUInt32, lasthbin)  //hive bins data size
                attribute(DUInt32, unknown2)
                //attribute(Dbuffer, padding) 396 bytes
                //attribute(DUInt32, checksum) xor-32 of previous 508 bytes
                attribute(DUnicodeString, name)
                function(DUnicodeString, version, DNone)
                attribute(DObject, key)
                function(DObject, deserializeRaw, DObject)
               )

  memberList(Regf, 
             member(Regf, regf)
             member(Regf, sequence1)
             member(Regf, sequence2)
             member(Regf, timestamp)
             member(Regf, major)
             member(Regf, minor)
             member(Regf, fileType)
             member(Regf, unknown1)
             member(Regf, keyrecord)
             member(Regf, lasthbin)
             member(Regf, unknown2)
             member(Regf, name)
             method(Regf, version)
             member(Regf, key)
             method(Regf, deserializeRaw)
            )

protected:
  ~Regf();
private:
  RealValue<DFunctionObject*>        _version, _deserializeRaw;
};

#endif
