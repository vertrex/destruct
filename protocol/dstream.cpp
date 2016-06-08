#include "dstream.hpp"

namespace Destruct
{

DStream::DStream(DStruct* dstruct, DValue const& args) : DCppObject<DStream>(dstruct, args)
{
  this->init();
  DObject* dargs = args;

  DInt8 mode = dargs->getValue("input");
  DUnicodeString filePath = dargs->getValue("filePath");

  if (mode == 0)
    this->__stream.open(filePath.c_str(), std::iostream::out | std::iostream::binary | std::iostream::trunc);
  else
    this->__stream.open(filePath.c_str(), std::iostream::in | std::iostream::binary);
  if (this->__stream.fail())
    throw DException("DStream can't open file " + filePath);
}

DStream::DStream(const DStream& rhs) : DCppObject<DStream>(rhs)
{
  this->copy(this, rhs);
}

/**
 *  Take care if not destroyed it will not be flushed !
 */
DStream::~DStream()
{
  this->__stream.close();
}

DBuffer DStream::read(DValue const& args)
{
  DInt64 size = args;
  if (size == 0)
    return DBuffer(NULL, 0);

 
  DBuffer buffer((int32_t)size);
  this->__stream.read((char*)buffer.data(), size);

  if (this->__stream.gcount() != size)
    throw DException("Can't read all size");
  if (this->__stream.fail())
    throw DException("DStream::read " + args.asUnicodeString() + " fail");
  return (buffer);
}

DInt64  DStream::write(DValue const& args)
{
  DBuffer buffer = args;

  const char* data = (const char*)buffer.data();
  int64_t size = buffer.size();

  if (size)
  {
    this->__stream.write(data, size);
    if (this->__stream.fail())
    {
      std::stringstream exception;
      exception << "DStream::write error can't write data of size " << size;
      throw DException(exception.str());
    }
  }
  return (size); //XXX check
}

void    DStream::flush(void)
{
  this->__stream.flush();
}

void    DStream::seek(DValue const& args)
{
  DUInt64 pos = args;
//if input
  this->__stream.seekg(pos);
}

DUInt64 DStream::size(void)
{
//if input
  return (this->__stream.tellg());
}

DUInt64 DStream::tell(void)
{
//if input
  return (this->__stream.tellg());
}

}
