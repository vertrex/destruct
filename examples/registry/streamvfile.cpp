#include "streamvfile.hpp"

#include <unistd.h>

#include <fcntl.h>

using namespace Destruct;

StreamFile::StreamFile(DStruct* dstruct, DValue const& args): DStream(dstruct)
{
  this->init();
  std::string filePath = args.get<DUnicodeString>();
  this->__fd = open(filePath.c_str(), O_RDONLY);
}

StreamFile::StreamFile(const StreamFile& copy) : DStream(copy)
{
  this->init();
}

StreamFile::~StreamFile()
{
  close(this->__fd);
}

DStream& StreamFile::read(char*  buff, uint32_t size)
{
  int readed = ::read(this->__fd, buff, size);
  return (*this);
}

DStream& StreamFile::write(const char* buff, uint32_t size) 
{
  throw DException("Can't write on StreamFile");
}

DStream& StreamFile::seek(int64_t pos)
{
  lseek64(this->__fd, pos, SEEK_SET);
  return (*this);
} 

int64_t  StreamFile::tell(void)
{
  return (lseek64(this->__fd, 0, SEEK_CUR));
}
