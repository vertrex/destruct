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

#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "dstructs.hpp"
#include "devicestream.hpp"

using namespace Destruct;

/**
 *  DeviceStream 
 */
DeviceStream::DeviceStream(DStruct* dstruct, DValue const& args) : DCppObject<DeviceStream>(dstruct, args)
{
  struct stat 	stbuff;

  this->init();
  this->__size = ((DObject*)args)->getValue("size");
  this->__path = ((DObject*)args)->getValue("path").get<DUnicodeString>();
  this->__fd = open(this->__path.c_str(), O_RDONLY | O_LARGEFILE);
  if (this->__fd == -1)
    throw DException("Can't open device");
  if (stat(this->__path.c_str(), &stbuff) == -1)
    throw DException("Cant stat");
}

DeviceStream::~DeviceStream()
{
  ::close(this->__fd);
}

DBuffer DeviceStream::read(DValue const& args)
{
  DInt64 sizeToRead = args;
  if (sizeToRead == 0)
    return DBuffer(NULL, 0);

  DBuffer buffer((int32_t)sizeToRead);
  int n = ::read(this->__fd, (char*)buffer.data(), sizeToRead);
  if (n < 0)
  {
    if (errno == EIO)
      throw DException("DeviceStream::vread EIO error");
    else
      throw DException("DeveiceStream::vread error read = -1");
  }
  //if n != size throw ?
  return (buffer);
}

DUInt64 DeviceStream::size(void)
{
  return (this->__size);
}

void    DeviceStream::seek(DValue const& args)
{
 uint64_t n = lseek64(this->__fd, args.get<DUInt64>(), SEEK_SET);
 if (n == ((uint64_t)-1))
    throw DException("devices::vseek can't seek error " + std::string(strerror(errno)));

 return ;// (n);
}

DUInt64 DeviceStream::tell(void)
{
  return lseek64(this->__fd, 0, SEEK_CUR);
}

void    DeviceStream::close(void)
{
  ::close(this->__fd);
}
