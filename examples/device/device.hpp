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

#ifndef __DEVICE_UNIX_HPP__
#define __DEVICE_UNIX_HPP__

#include "device_common.hpp"

using namespace Destruct;

class Device : public DCppObject<Device>
{
public:
  Device(DStruct* dstruct, DValue const& args);

  DObject*      list(void);
  DObject*      open(void);

  RealValue<DUnicodeString> blockDevice, serialNumber, model;
  RealValue<DUInt64> size;

  attributeCount(Device, 5)

  attributeList(
                attribute(DUnicodeString, blockDevice)
                attribute(DUnicodeString, serialNumber)
                attribute(DUnicodeString, model) 
                attribute(DUInt64, size)
                function(DObject, open, DNone)
               )

  memberList(Device, 
             member(Device, blockDevice)
             member(Device, serialNumber)
             member(Device, model)
             member(Device, size)
             method(Device, open)
            )

protected:
  ~Device();
private:
  RealValue<DFunctionObject*>        _list, _open;
};

#endif
