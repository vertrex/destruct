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

#include "device.hpp"
#include "devicelist.hpp"

#define _WIN32_DCOM
#include <windows.h>
#include <Shlwapi.h>
#include <comdef.h>
#include <Wbemidl.h>
# pragma comment(lib, "wbemuuid.lib")

using namespace Destruct;

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
  HRESULT hres;

  IWbemLocator	*pLoc = NULL; 
  IWbemServices	*pSvc = NULL;

  hres =  CoInitializeEx(NULL, COINIT_APARTMENTTHREADED); 
  if (FAILED(hres))
    return DNone;

  hres =  CoInitializeSecurity(NULL, -1, NULL, NULL, RPC_C_AUTHN_LEVEL_DEFAULT,
                               RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE, NULL );
       
  if (FAILED(hres) &&  !(hres == RPC_E_TOO_LATE))
    return DNone;//throw
 
  hres = CoCreateInstance(CLSID_WbemAdministrativeLocator,
			  NULL, CLSCTX_INPROC_SERVER, IID_IWbemLocator, (LPVOID *) &(pLoc));
  if (FAILED(hres))
    return DNone;//throw
  
  hres = pLoc->ConnectServer(_bstr_t(L"ROOT\\CIMV2"), NULL , NULL, 0,
                             NULL, 0, 0, &(pSvc));
  if (FAILED(hres))
    return DNone;//throw

  hres = CoSetProxyBlanket(pSvc, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE,
           	           NULL, RPC_C_AUTHN_LEVEL_CALL, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE);
  if (FAILED(hres))
    return DNone; //throw

  IEnumWbemClassObject* pEnumerator = NULL;
  hres = pSvc->ExecQuery(bstr_t("WQL"), bstr_t("SELECT * FROM Win32_DiskDrive"),
			       WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,  NULL,&pEnumerator);
  if (FAILED(hres))
	return DNone;//throw

  DObject* deviceList = Destruct::DStructs::instance().generate("DVectorObject");
  IWbemClassObject *pclsObj;
  ULONG uReturn = 0;
  while (pEnumerator)
  {
    HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1, &pclsObj, &uReturn);

    if (!uReturn)
      break;

    Device* device = static_cast<Device*>(Destruct::DStructs::instance().generate("Device"));
//    _variant_t vtProp;
	VARIANT vtProp;

    hr = pclsObj->Get(L"Name", 0, &vtProp, 0, 0);
    if (SUCCEEDED(hr))
      device->blockDevice = DUnicodeString((char*)vtProp.pbstrVal, (int32_t)wcslen((wchar_t*)vtProp.pbstrVal) * 2, "UTF-16LE"); //, wchar_t* length?, "UTF-16LE")
    else 
      continue;
	VariantClear(&vtProp);

	
	DUnicodeString type;
         hr = pclsObj->Get(L"InterfaceType", 0, &vtProp, 0, 0);
	if (SUCCEEDED(hr))
        type = DUnicodeString((char*)vtProp.pbstrVal, (int32_t)wcslen((wchar_t*)vtProp.pbstrVal) * 2, "UTF-16LE");
	VariantClear(&vtProp);

	if (type != "USB")
	{
          hr = pclsObj->Get(L"SerialNumber", 0, &vtProp, 0, 0);
	  if (SUCCEEDED(hr))
            device->serialNumber = DUnicodeString((char*)vtProp.pbstrVal, (int32_t)wcslen((wchar_t*)vtProp.pbstrVal) * 2, "UTF-16LE");
	  VariantClear(&vtProp);
	}
	else
	{
	  hr = pclsObj->Get(L"PnPDeviceID", 0, &vtProp, 0, 0);
	  if (SUCCEEDED(hr))
	  {
            DUnicodeString pnpDeviceId((char*)vtProp.pbstrVal, (int32_t)wcslen((wchar_t*)vtProp.pbstrVal) * 2, "UTF-16LE");
	    std::size_t pos = std::string(pnpDeviceId).rfind("\\");
	    if (pos != std::string::npos)
	    {	
	      std::string serialNumber = std::string(pnpDeviceId).substr(pos + 1);
	      pos = serialNumber.rfind("&");
	      if (pos != std::string::npos)
	        serialNumber = serialNumber.substr(0, pos);
	      device->serialNumber = DUnicodeString(serialNumber);
	    }
	  }
	  VariantClear(&vtProp);
	}


    hr = pclsObj->Get(L"Model", 0, &vtProp, 0, 0);
	if (SUCCEEDED(hr))
      device->model = DUnicodeString((char*)vtProp.pbstrVal, (int32_t)wcslen((wchar_t*)vtProp.pbstrVal) * 2, "UTF-16LE");
	VariantClear(&vtProp);
    /*
     * Get Size 
     */

    hr = pclsObj->Get(L"Name", 0, &vtProp, NULL, NULL);
    if (SUCCEEDED(hr))
    {
      wchar_t*	var = (wchar_t*)vtProp.pbstrVal;
	      
      HANDLE hnd = CreateFileW(var , GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
      if (hnd == INVALID_HANDLE_VALUE)
        continue;
      else
      {
        GET_LENGTH_INFORMATION diskSize;
        DWORD lpBytesReturned = 0;
        DeviceIoControl(hnd, FSCTL_ALLOW_EXTENDED_DASD_IO, NULL, 0, NULL, 0, &lpBytesReturned, NULL);
        if (DeviceIoControl(hnd, IOCTL_DISK_GET_LENGTH_INFO, NULL, 0, &diskSize, sizeof(diskSize), &lpBytesReturned,0) == 0)
        {
	     CloseHandle(hnd);
         continue; 
        } 
        device->size = ((uint64_t)diskSize.Length.QuadPart);
        CloseHandle(hnd);
      }
    }
	deviceList->call("push", RealValue<DObject*>(device));
  }
  pEnumerator->Release();

  if (pSvc)
    pSvc->Release();
  if (pLoc)
    pLoc->Release();

  //WinPmem support
  HANDLE handle = CreateFile("\\\\.\\pmem", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
  if (handle != INVALID_HANDLE_VALUE)
  {
	  DWORD ioctlSize;
	  uint64_t memorySize;
	  struct PmemMemoryInfo info;	  
	  ZeroMemory(&info, sizeof(info));

	  if (DeviceIoControl(handle, PMEM_INFO_IOCTRL, NULL, 0, (char *)&info, sizeof(info), &ioctlSize, NULL) == TRUE)
	  {
		  for (__int64 i = 0; i < info.NumberOfRuns.QuadPart; i++) 
			memorySize = info.Run[i].start + info.Run[i].length;

		  Device* ramDevice = static_cast<Device*>(Destruct::DStructs::instance().generate("Device"));
		  ramDevice->model = DUnicodeString("RAM");
		  ramDevice->blockDevice = DUnicodeString("\\\\.\\pmem");
		  ramDevice->serialNumber = DUnicodeString("Unknown");
		  ramDevice->size = RealValue<DUInt64>(memorySize);
		  deviceList->call("push", RealValue<DObject*>(ramDevice));
	  }
	  CloseHandle(handle);
  }

  return (deviceList);
}
