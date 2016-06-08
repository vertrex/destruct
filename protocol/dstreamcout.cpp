#include "dstreamcout.hpp"

namespace Destruct
{
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
  DBuffer buffer = args;

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

}
