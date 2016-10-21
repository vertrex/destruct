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

#include "memdevicedriver.hpp"
#include "windows.h"

extern "C"
{
  EXPORT void DestructExport()
  {
    Destruct::DStructs& destruct = Destruct::DStructs::instance();

    registerDCpp(MemoryDeviceDriver)
    registerDCpp(MemoryDeviceStream)
  }
}

using namespace Destruct;

/**
 *  MemoryDeviceDriver 
 */
MemoryMemoryDeviceDriverDriver::MemoryMemoryDeviceDriverDriver(DStruct* dstruct, DValue const& args) : DCppObject<MemoryMemoryDeviceDriverDriver>(dstruct, args)
{
  this->init();
}

MemoryDeviceDriver::~MemoryDeviceDriver()
{
}

void MemoryDeviceDriver::load(void)
{
  std::string driver_path;

  this->unload();// First ensure the driver is not already installed.

  SC_HANDLE scm = OpenSCManager(NULL, NULL, SC_MANAGER_CREATE_SERVICE);
  if (!scm)
    throw Destruct::DException("Can not open SCM. Are you administrator?");

  // First try to create the service.
  SC_HANDLE service = CreateService(
      scm,
      service_name.c_str(),
      service_name.c_str(),
      SERVICE_ALL_ACCESS,
      SERVICE_KERNEL_DRIVER,
      SERVICE_DEMAND_START,
      SERVICE_ERROR_NORMAL,
      driver_path.c_str(),
      NULL,
      NULL,
      NULL,
      NULL,
      NULL);

  // Maybe the service is already there - try to open it instead.
  if (GetLastError() == ERROR_SERVICE_EXISTS)
  {
    service = OpenService(scm, service_name.c_str(), SERVICE_ALL_ACCESS);
  }

  if (!service) 
  {
    CloseServiceHandle(scm);
    throw Destruct::DException("Service not found");
  }

  if (!StartService(service, 0, NULL)) 
  {
    if (GetLastError() != ERROR_SERVICE_ALREADY_RUNNING) 
    {
      CloseServiceHandle(service);
      CloseServiceHandle(scm);
      throw Destruct::DException("Error: StartService(), Cannot start the driver:");
    }
  }
/*
  // Remember this so we can safely unload it.
  driver_installed_ = true;

  LOG(INFO) << "Loaded Driver " << driver_path;
  device_urn = URN::NewURNFromFilename("\\\\.\\" + device_name);

  // We need write mode for issuing IO controls. Note the driver will refuse
  // write unless it is also switched to write mode.
  resolver.Set(device_urn, AFF4_STREAM_WRITE_MODE, new XSDString("append"));

  AFF4ScopedPtr<FileBackedObject> device_stream = resolver.AFF4FactoryOpen
      <FileBackedObject>(device_urn);

  if (!device_stream) {
    LOG(ERROR) << "Unable to open device: " << GetLastErrorMessage();
    CloseServiceHandle(service);
    CloseServiceHandle(scm);
    return IO_ERROR;
  }
*/
  CloseServiceHandle(service);
  CloseServiceHandle(scm);
}

void MemoryDeviceDriver::unload(void)
{
  SC_HANDLE scm, service;
  SERVICE_STATUS ServiceStatus;

  scm = OpenSCManager(NULL, NULL, SC_MANAGER_CREATE_SERVICE);

  if (!scm)
    throw Destruct::DException("Can't load driver");

  service = OpenService(scm, service_name.c_str(), SERVICE_ALL_ACCESS);

  if (service) 
  {
    ControlService(service, SERVICE_CONTROL_STOP, &ServiceStatus);
  }

  DeleteService(service);
  CloseServiceHandle(service);
}
