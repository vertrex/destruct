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
          ~RegistryValues();
  DValue  deserializeRaw(DValue const& stream);


  RealValue<DInt32>          size;
  RealValue<DObject*>        parent, list;

  attributeCount(RegistryValues, 3)
  attributeList(attribute(DInt32, size)
                attribute(DObject, list)
                function(DObject, deserializeRaw, DObject)
               )
  memberList(RegistryValues, 
             member(RegistryValues, size)
             member(RegistryValues, list)
             method(RegistryValues, deserializeRaw)
            )
private:
  uint64_t                           __size;
  RealValue<DFunctionObject*>        _deserializeRaw;
};

#endif
