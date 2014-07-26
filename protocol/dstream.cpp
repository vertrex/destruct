#include "dstream.hpp"

namespace Destruct
{
DStream::DStream(DStruct* dstruct) : DCppObject<DStream>(dstruct, RealValue<DObject*>(DNone))
{
}

DStream::DStream(DStruct* dstruct, DValue const& args) : DCppObject<DStream>(dstruct, args)
{
  this->init();
  DObject* dargs = args.get<DObject*>();

  DInt8 _mode = dargs->getValue("input").get<DInt8>();
  DUnicodeString filePath = dargs->getValue("filePath").get<DUnicodeString>();

  std::cout << "opening stream " << std::endl;
  _mode = 0;
  filePath = "output";
  if (_mode == 0)
    this->__fstream.open(filePath.c_str(), std::iostream::out | std::iostream::binary | std::iostream::trunc);
  else
    this->__fstream.open(filePath.c_str(), std::iostream::in | std::iostream::binary);
}

DStream::DStream(const DStream& copy) : DCppObject<DStream>(copy) 
{
}

DStream::~DStream()
{
  this->__fstream.close(); 
}

/* write */
DStream& DStream::operator<<(std::string val) 
{
  std::cout << val;
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
  std::cout << std::endl;
  func(this->__fstream);
  return (*this);
}

DStream& DStream::write(const char* buff, uint32_t size) 
{
  this->__fstream.write((const char*)buff, size);
  return (*this);
} 

DInt64 DStream::write(DValue const& args)
{
 //this->__fstream >> args.getValue(buff) args.getValue(size) 
  DUnicodeString val = args.get<DUnicodeString>();
  this->__fstream << val;

  return (0);
}

DStream& DStream::operator<<(DStream& input)
{
        //this->__fstream << input;
  return (*this);
}

/* read */
DStream& DStream::operator>>(DStream& output)
{
        //this->__fstream >> output;
  return (*this);
}

DStream& DStream::operator>>(std::string& val) 
{
  this->__fstream >> val;
  return (*this);
}

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

/*
 * DStreamCout
*/

DStreamCout::DStreamCout(DStruct* dstruct, DValue const& args): DStream(dstruct)
{
}

DStreamCout::DStreamCout(const DStreamCout& copy) : DStream(copy)
{
}


DStreamCout::~DStreamCout()
{
}

DStream& DStreamCout::operator>>(std::string& val) 
{
  std::cin >> val;
  return (*this);
}

DStream& DStreamCout::operator<<(std::string val) 
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


/**
 *  DStream string
 */

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

}
