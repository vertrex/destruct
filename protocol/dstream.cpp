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
  DBuffer buffer(data, size);
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
    this->__stream.write(data, size);
    std::cout.write(data, size); // << std::string(buff, size);
    if (std::cout.fail())
      throw DException("DStreamCout::write Can't write data of size " + size);
  }
  return (size); //XXX check
}

/*
DStreamString::DStreamString(DStruct* dstruct, DValue const& args): DStream(dstruct)
{
  this->init();
  //DObject* dargs = args.get<DObject*>();
  //DInt8 _mode = dargs->getValue("input").get<DInt8>();

  //std::cout << "opening stream " << std::endl;
  //_mode = 0;
  //filePath = "output";
  //if (_mode == 0)
    //this->__fstream.open(filePath.c_str(), std::iostream::out | std::iostream::binary | std::iostream::trunc);
  //else
    //this->__fstream.open(filePath.c_str(), std::iostream::in | std::iostream::binary);
}

DStreamString::DStreamString(const DStreamString& copy) : DStream(copy)
{
}

DStreamString::~DStreamString()
{
}

DStream& DStreamString::read(char*  buff, uint32_t size)
{
  this->__stream.read(buff, size);
  return (*this);
}

DStream& DStreamString::write(const char* buff, uint32_t size) 
{
  this->__stream.write(buff, size);
  return (*this);
} 

const std::string DStreamString::str(void) const //seek etc ?
{
  return (this->__stream.str());
}

void DStreamString::clear(void) 
{
  this->__stream.str("");;
}
*/
}
