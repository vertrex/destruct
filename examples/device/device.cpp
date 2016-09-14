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

#include "device.hpp"
#include "dstructs.hpp"
#include "devicelist.hpp"

#ifdef WIN32
#include "devicestream_windows.hpp"
#else
#include "devicestream_unix.hpp"
#endif
#include "protocol/dmutableobject.hpp"

extern "C"
{
  EXPORT void DestructExport()
  {
    Destruct::DStructs& destruct = Destruct::DStructs::instance();

    registerDCpp(DeviceList)
    registerDCpp(Device)
    registerDCpp(DeviceStream)
  }
}

using namespace Destruct;

/**
 *  Device 
 */
Device::Device(DStruct* dstruct, DValue const& args) : DCppObject<Device>(dstruct, args)
{
  this->init();
}

Device::~Device()
{
}

DObject* Device::open(void)
{
  DMutableObject* args = static_cast<DMutableObject*>(Destruct::DStructs::instance().generate("DMutable"));
  args->setValueAttribute(DType::DUInt64Type, "size", this->size); 
  args->setValueAttribute(DType::DUnicodeStringType, "path",  this->blockDevice);

  return (Destruct::DStructs::instance().generate("DeviceStream", RealValue<DObject*>(args)));
}
