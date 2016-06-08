#include "dstreamstring.hpp"

namespace Destruct
{

/**
 *  DStreamString
 */
DStreamString::DStreamString(DStruct* dstruct, DValue const& args) : DCppObject<DStreamString>(dstruct, args)
{
  this->init();
}

DStreamString::DStreamString(const DStreamString& copy) : DCppObject<DStreamString>(copy)
{
  this->init();
}

DStreamString::~DStreamString()
{
}

DBuffer DStreamString::read(DValue const& args)
{
  DInt64 size = args;
  if (size == 0)
    return DBuffer(NULL, 0);

  DBuffer buffer((int32_t)size);
  this->__stream.read((char*)buffer.data(), size);
 
  if (this->__stream.gcount() != size)
    throw DException("Can't read all size");
  if (this->__stream.fail())
    throw DException("DStreamString::read " + args.asUnicodeString() + " fail");
  //std::cout << std::string((const char*)buffer.data(), size) << std::endl;
  return (buffer); //XXX new ! 
}

DInt64  DStreamString::write(DValue const& args)
{
  DBuffer buffer = args;

  const char* data = (const char*)buffer.data();
  int64_t size = buffer.size();

  if (size)
  {
    this->__stream.write(data, size);
    if (this->__stream.fail())
      throw DException("DStreamString::write Can't write data of size " + size);
  }
  return (size); //XXX check
}

DUnicodeString  DStreamString::str(void)//const 
{
  return (DUnicodeString(this->__stream.str()));
}

void    DStreamString::clear(void) //const
{
  this->__stream.str("");;
}

}
