#include "streamvfile.hpp"

#include <unistd.h>

#include <fcntl.h>

using namespace Destruct;

StreamFile::StreamFile(DStruct* dstruct, DValue const& args): DStream(dstruct), __fd(-1)
{
  this->init();
  DUnicodeString filePath = args.get<DUnicodeString>();
  this->__fd = open(filePath.c_str(), O_RDONLY);
}

StreamFile::StreamFile(const StreamFile& copy) : DStream(copy), __fd(-1)
{
  this->init();
}

StreamFile::~StreamFile()
{
  close(this->__fd);
}

DStream& StreamFile::read(char*  buff, uint32_t size)
{
  if (::read(this->__fd, buff, size) != size)
    throw DException("StreamFile::read Can't read enough bytes");
  return (*this);
}

DStream& StreamFile::write(const char* buff, uint32_t size) 
{
  throw DException("Can't write on StreamFile");
}

DStream& StreamFile::seek(int64_t pos)
{
  if (lseek64(this->__fd, pos, SEEK_SET) != pos)
    throw DException("StreamFile::seek Can't seek");
  return (*this);
} 

int64_t  StreamFile::tell(void)
{
  return (lseek64(this->__fd, 0, SEEK_CUR));
}
