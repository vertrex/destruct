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

#include "dstructs.hpp"
#include "dexception.hpp"

#include <libudev.h>

#include "device.hpp"
#include "devicelist.hpp"

/**
 *  DeviceList 
 */
DeviceList::DeviceList(DStruct* dstruct, DValue const& args) : DCppObject<DeviceList>(dstruct, args)
{
  this->init();
}

DeviceList::~DeviceList()
{
}

DObject* DeviceList::list(void)
{
  udev*			udev;
  udev_enumerate*	enumerate;
  udev_list_entry*	devices;
  udev_list_entry*	dev_list_entry;
  udev_device*		dev;
  DObject* deviceList = Destruct::DStructs::instance().generate("DVectorObject");

  udev = udev_new();
  if (udev == NULL)
    throw DException("Can't access udev");
  	
  enumerate = udev_enumerate_new(udev);
  udev_enumerate_add_match_subsystem(enumerate, "block");
  udev_enumerate_scan_devices(enumerate);
  devices = udev_enumerate_get_list_entry(enumerate);

  udev_list_entry_foreach(dev_list_entry, devices)
  {
    const char* path = udev_list_entry_get_name(dev_list_entry);
    dev = udev_device_new_from_syspath(udev, path);
   
    if (std::string(udev_device_get_devtype(dev)) == std::string("disk"))
    {
      const char* blockDevice = udev_device_get_devnode(dev);
      const char* serialNumber = udev_device_get_property_value(dev, "ID_SERIAL");
      const char* model = udev_device_get_property_value(dev, "ID_MODEL");
      const char* size = udev_device_get_sysattr_value(dev, "size");
      const char* block_size = udev_device_get_sysattr_value(dev, "queue/physical_block_size");

      if (blockDevice != NULL && size != NULL)
      {	
        int bs = 512;
	uint64_t realSize = 0;

	if (block_size != NULL)
	  bs = atoi(block_size);
	realSize = atoll(size) * bs;
        if (model == NULL)
          model = "Unknown";
        if (serialNumber == NULL)
          serialNumber = "Unknown";
        if (realSize)
        {
          Device* device = static_cast<Device*>(Destruct::DStructs::instance().generate("Device"));
          device->blockDevice = blockDevice ;
          device->serialNumber = serialNumber;
          device->model = model;
          device->size = realSize;
          //XXX try stat or open here so we don't push device that can't be opened !

          deviceList->call("push", RealValue<DObject*>(device));
        }
      }
    }
  }
  udev_enumerate_unref(enumerate);
  udev_unref(udev);

  return (deviceList);
}
