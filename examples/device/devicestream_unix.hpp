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

#ifndef __DEVICESTREAM_UNIX_HPP__
#define __DEVICESTREAM_UNIX_HPP__

#include "device_common.hpp"

using namespace Destruct;

class DeviceStream : public DCppObject<DeviceStream>
{
public:
  DeviceStream(DStruct* dstruct, DValue const& args);

  DBuffer       read(DValue const& args);
  DUInt64       size(void);
  void          seek(DValue const& args);
  DUInt64       tell(void);

  attributeCount(DeviceStream, 4)

  attributeList(
                function(DBuffer, read, DInt64)
                function(DUInt64, size, DNone)
                function(DNone, seek, DUInt64)
                function(DUInt64, tell, DNone)
               )

  memberList(DeviceStream, 
              method(DeviceStream, read)
              method(DeviceStream, size)
              method(DeviceStream, seek)
              method(DeviceStream, tell)
            )

protected:
  ~DeviceStream();
private:
  RealValue<DFunctionObject*>   _read, _size, _seek, _tell;
  DUnicodeString                __path;
  DUInt64                       __size;
  DInt32                        __fd;
};

#endif
