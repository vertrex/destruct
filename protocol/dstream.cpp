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

DStream::~DStream()
{
  std::cout << "~DStream closing stream" << std::endl; 
// IF IT"S NOT CLOSE IT WILL NOT BE FLUSHED TAKE CAREEEE
//and it can't be read because it's not flushed too :) 
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


/*
DStream::DStream(DStruct* dstruct) : DCppObject<DStream>(dstruct, RealValue<DObject*>(DNone))
{
  this->init();
}

DStream::DStream(DStruct* dstruct, DValue const& args) : DCppObject<DStream>(dstruct, args)
{
  this->init();
  DObject* dargs = args.get<DObject*>();

  DInt8 _mode = dargs->getValue("input").get<DInt8>();
  DUnicodeString filePath = dargs->getValue("filePath").get<DUnicodeString>();

  if (_mode == 0)
    this->__fstream.open(filePath.c_str(), std::iostream::out | std::iostream::binary | std::iostream::trunc);
  else
    this->__fstream.open(filePath.c_str(), std::iostream::in | std::iostream::binary);
}

DStream::DStream(const DStream& copy) : DCppObject<DStream>(copy) 
{
  this->init();
}

DStream::~DStream()
{
  this->__fstream.close(); 
}

// write 
DStream& DStream::operator<<(DUnicodeString val) 
{
  this->__fstream << val;
  return (*this);
}
  
DStream& DStream::operator<<(char val)
{
  this->__fstream << val;
  return (*this);
}

DStream& DStream::operator<<(StandardEndLine func)
{
  func(this->__fstream);
  return (*this);
}

DStream& DStream::write(const char* buff, uint32_t size) 
{
  this->__fstream.write((const char*)buff, size);
  return (*this);
} 

DInt64 DStream::write(DValue const& args) //XXX
{
 //this->__fstream >> args.getValue(buff) args.getValue(size) 
  DUnicodeString val = args.get<DUnicodeString>(); //XXX get DBuffer
  this->__fstream << val;  //this->__fstream.write(buffer.data(), buffer.size())

  return (0);
}

DStream& DStream::operator<<(DStream& input)
{
  //this->__fstream << input;
  return (*this);
}

// read 
DStream& DStream::operator>>(DStream& output)
{
  //this->__fstream >> output;
  return (*this);
}

//DStream& DStream::operator>>(DUnicodeString& val) 
//{
//this->__fstream >> val;

  //std::cout << "res " << this->__fstream ? << std::endl;
  //return (*this);
  //}

DStream& DStream::read(char*  buff, uint32_t size)
{
  this->__fstream.read((char*)buff, size);

  return (*this);
}

DInt64 DStream::read(DValue const& args)
{
//XXX get args.buff, args.size  ...
  return (0);
}

bool DStream::fail(void)
{
  return (this->__fstream.fail()); 
}

DStreamCout::DStreamCout(DStruct* dstruct, DValue const& args): DStream(dstruct)
{
}

DStreamCout::DStreamCout(const DStreamCout& copy) : DStream(copy)
{
}


DStreamCout::~DStreamCout()
{
}

DStream& DStreamCout::operator>>(DUnicodeString& val) 
{
  std::cin >> val;
  return (*this);
}

DStream& DStreamCout::operator<<(DUnicodeString val) 
{
  std::cout << val;
  return (*this);
}
  

DStream& DStreamCout::operator<<(char val)
{
  std::cout << val;
  return (*this);
}

typedef std::basic_ostream<char, std::char_traits<char> > CoutType;
typedef CoutType& (*StandardEndLine)(CoutType&);
  
DStream& DStreamCout::operator<<(StandardEndLine func)
{
  func(std::cout);
  return (*this);
}

DStream& DStreamCout::read(char*  buff, uint32_t size)
{
  //XXX std::cin >>
  return (*this);
}

DStream& DStreamCout::write(const char* buff, uint32_t size) 
{
  std::cout << std::string(buff, size);
  return (*this);
} 

bool DStreamCout::fail(void)
{
  return (std::cout.fail());
}


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
