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

#ifndef __MEMORYDEVICE_HPP__
#define __MEMORYDEVICE_HPP__

#include "memorydevice_common.hpp"

#include <windows.h>
#include <Shlwapi.h>
#include <initguid.h>

#define PMEM_CTRL_IOCTRL CTL_CODE(0x22, 0x101, 0, 3)
#define PMEM_WRITE_ENABLE CTL_CODE(0x22, 0x102, 0, 3)
#define PMEM_INFO_IOCTRL CTL_CODE(0x22, 0x103, 0, 3)


#pragma pack(push, 2)
typedef struct pmem_info_runs {
	__int64 start;
	__int64 length;
} PHYSICAL_MEMORY_RANGE;

struct PmemMemoryInfo {
	LARGE_INTEGER CR3;
	LARGE_INTEGER NtBuildNumber; // Version of this kernel.
	LARGE_INTEGER KernBase;  // The base of the kernel image.
	LARGE_INTEGER KDBG;  // The address of KDBG

	// Support up to 32 processors for KPCR.
	LARGE_INTEGER KPCR[32];

	LARGE_INTEGER PfnDataBase;
	LARGE_INTEGER PsLoadedModuleList;
	LARGE_INTEGER PsActiveProcessHead;

	// The address of the NtBuildNumber integer - this is used to find the kernel
	// base quickly.
	LARGE_INTEGER NtBuildNumberAddr;

	// As the driver is extended we can add fields here maintaining
	// driver alignment..
	LARGE_INTEGER Padding[0xfe];

	LARGE_INTEGER NumberOfRuns;

	// A Null terminated array of ranges.
	PHYSICAL_MEMORY_RANGE Run[100];
};

#pragma pack(pop)

#endif

using namespace Destruct;

class MemoryDeviceDriver : public DCppObject<MemoryDeviceDriver>
{
public:
  MemoryDeviceDriver(DStruct* dstruct, DValue const& args);

  void      load(DUnicodeString);
  void      unload(void);

  attributeCount(MemoryDeviceDriver, 2)

  attributeList(
                function(DNone, load, DNone)
                function(DNone, unload, DNone)
               )

  memberList(MemoryDeviceDriver, 
             member(MemoryDeviceDriver, load)
             member(MemoryDeviceDriver, unload)
            )

protected:
  ~MemoryDeviceDriver();
private:
  RealValue<DFunctionObject*>        _load, _unload;
};

#endif
