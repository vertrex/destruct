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

#ifndef __value_hh__
#define __value_hh__

#include "registry_common.hpp"

using namespace Destruct;

class RegistryValues : public DCppObject<RegistryValues>
{
public:
  RegistryValues(DStruct* dstruct, DValue const& args);

  DObject*      deserializeRaw(DValue const& stream);


  RealValue<DInt32>          size;// == (valueCount / 4) * -1
  RealValue<DUInt32>         valueCount, minor;
  RealValue<DObject*>        list;

  attributeCount(RegistryValues, 4)
  attributeList(attribute(DInt32, size)
                attribute(DObject, list)
                function(DObject, deserializeRaw, DObject)
                attribute(DUInt32, minor)
               )
  memberList(RegistryValues, 
             member(RegistryValues, size)
             member(RegistryValues, list)
             method(RegistryValues, deserializeRaw)
             member(RegistryValues, minor)
            )
protected:
  ~RegistryValues();
private:
  uint64_t                           __size;
  RealValue<DFunctionObject*>        _deserializeRaw;
};

#endif
