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

#ifndef __DEVICELIST_UNIX_HPP__
#define __DEVICELIST_UNIX_HPP__

#include "device_common.hpp"

using namespace Destruct;


class DeviceList : public DCppObject<DeviceList>
{
public:
  DeviceList(DStruct* dstruct, DValue const& args);

  DObject*      list(void);

  attributeCount(DeviceList, 1)

  attributeList(
                function(DObject, list, DNone)
               )


  memberList(DeviceList, 
               method(DeviceList, list)
            )
protected:
  ~DeviceList();
private:
  RealValue<DFunctionObject*>        _list;
};

#endif
