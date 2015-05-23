#include "dstream.hpp"

namespace Destruct
{

DStream::DStream(DStruct* dstruct, DValue const& args) : DCppObject<DStream>(dstruct, args)
{
  this->init();
  DObject* dargs = args.get<DObject*>();

  DInt8 mode = dargs->getValue("input").get<DInt8>();
  DUnicodeString filePath = dargs->getValue("filePath").get<DUnicodeString>();
  dargs->destroy();

  if (mode == 0)
    this->__stream.open(filePath.c_str(), std::iostream::out | std::iostream::binary | std::iostream::trunc);
  else
    this->__stream.open(filePath.c_str(), std::iostream::in | std::iostream::binary);
}

DStream::DStream(const DStream& copy) : DCppObject<DStream>(copy)
{
  this->init();
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
  DInt64 size = args.get<DInt64>();
  if (size == 0)
    return DBuffer(NULL, 0);

  uint8_t* data = new uint8_t[size + 1];
  this->__stream.read((char*)data, size);
  data[size] = 0;

  if (this->__stream.gcount() != size)
    throw DException("Can't read all size");
  if (this->__stream.fail())
    throw DException("DStream::read " + args.asUnicodeString() + " fail");
  DBuffer buffer(data, (int32_t)size);
  return (buffer); //XXX new ! 
}

DInt64  DStream::write(DValue const& args)
{
  DBuffer buffer = args.get<DBuffer>();

  const char* data = (const char*)buffer.data();
  int64_t size = buffer.size();

  if (size)
  {
    this->__stream.write(data, size);
    if (this->__stream.fail())
      throw DException("DStream::write Can't write data of size " + size);
  }
  return (size); //XXX check
}

void    DStream::flush(void)
{
  this->__stream.flush();
}

void    DStream::seek(DValue const& args)
{
  DUInt64 pos = args.get<DUInt64>();
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

/**
 *  DStreamCout
 */
DStreamCout::DStreamCout(DStruct* dstruct, DValue const& args) : DCppObject<DStreamCout>(dstruct, args)
{
  this->init();
}

DStreamCout::DStreamCout(const DStreamCout& copy) : DCppObject<DStreamCout>(copy)
{
  this->init();
}

DStreamCout::~DStreamCout()
{
}

DBuffer DStreamCout::read(DValue const& args)
{
  //XXX std::cin >>
  //DInt64 size = args.get<DInt64>();
  //if (size == 0)
    //return DBuffer(NULL, 0);

  //uint8_t* data = new uint8_t[size + 1];
  //this->__stream.read((char*)data, size);
  //data[size] = 0;

  //if (this->__stream.gcount() != size)
    //throw DException("Can't read all size");
  //if (this->__stream.fail())
    //throw DException("DStreamCout::read " + args.asUnicodeString() + " fail");
  //DBuffer buffer(data, size);
  //return (buffer); //XXX new ! 
  return DBuffer();
}

DInt64  DStreamCout::write(DValue const& args)
{
  DBuffer buffer = args.get<DBuffer>();

  const char* data = (const char*)buffer.data();
  int64_t size = buffer.size();

  if (size)
  {
    std::cout.write(data, size); // << std::string(buff, size);
    if (std::cout.fail())
      throw DException("DStreamCout::write Can't write data of size " + size);
  }
  return (size); //XXX check
}

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
  DInt64 size = args.get<DInt64>();
  if (size == 0)
    return DBuffer(NULL, 0);

  uint8_t* data = new uint8_t[size + 1]; //XXX ?
  this->__stream.read((char*)data, size);
  data[size] = 0; //XXX ?

  if (this->__stream.gcount() != size)
    throw DException("Can't read all size");
  if (this->__stream.fail())
    throw DException("DStreamString::read " + args.asUnicodeString() + " fail");
  std::cout << std::string((const char*)data, size) << std::endl;
  DBuffer buffer(data, (int32_t)size);
  return (buffer); //XXX new ! 
}

DInt64  DStreamString::write(DValue const& args)
{
  DBuffer buffer = args.get<DBuffer>();

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
