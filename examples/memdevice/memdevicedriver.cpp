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

#include "memdevicedriver.hpp"
#include "memdevicestream.hpp"
#include "dstructs.hpp"

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
MemoryDeviceDriver::MemoryDeviceDriver(DStruct* dstruct, DValue const& args) : DCppObject<MemoryDeviceDriver>(dstruct, args)
{
  this->init();
}

MemoryDeviceDriver::~MemoryDeviceDriver()
{
	this->unload();
}

void MemoryDeviceDriver::load(void)
{
 HMODULE hModule = GetModuleHandleW(NULL);
 CHAR cpath[MAX_PATH];
 GetModuleFileNameA(hModule, cpath, MAX_PATH);

 std::string driver_path = std::string(cpath).substr(0, std::string(cpath).rfind("\\") + 1);
 driver_path += "winpmem_x64.sys";
  
  SC_HANDLE scm = OpenSCManager(NULL, NULL, SC_MANAGER_CREATE_SERVICE);
  if (!scm)
    throw Destruct::DException("Can't open SCM. Are you administrator?");

  SC_HANDLE service = CreateService(
      scm, "pmem", "pmem",
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
    service = OpenService(scm, "pmem", SERVICE_ALL_ACCESS);
	//ERROR_SERVICE_MARKED_FOR_DELETE
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
      throw Destruct::DException("MemoryDeviceDriver cannot start the service");
    }
  }

  CloseServiceHandle(service);
  CloseServiceHandle(scm);
}

void MemoryDeviceDriver::unload(void)
{
  SC_HANDLE scm, service;
  SERVICE_STATUS ServiceStatus;

  scm = OpenSCManager(NULL, NULL, SC_MANAGER_CREATE_SERVICE);

  if (!scm)
    throw Destruct::DException("Can't open SCM");

  service = OpenService(scm, "pmem", SERVICE_ALL_ACCESS);
  if (service) 
    ControlService(service, SERVICE_CONTROL_STOP, &ServiceStatus);

  DeleteService(service);
  CloseServiceHandle(service);
}
