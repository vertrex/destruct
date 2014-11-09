#include <arpa/inet.h>
#include <iostream>
#include <sstream>
#include <string.h>

#include "networkstream.hpp"

namespace Destruct 
{

NetworkStream::NetworkStream(DStruct* dstruct, DValue const& args) : DStream(dstruct)
{
  this->__socket = args.get<DInt32>();
}

NetworkStream::NetworkStream(NetworkStream const& copy) : DStream(copy), __socket(copy.__socket)//, __readStream(copy.__readStream)
{
  //NetworkStream copy XXX shouldn't happen because can't copy string stream
}

NetworkStream::~NetworkStream()
{
}

int32_t NetworkStream::write(std::string const& str) 
{
  uint64_t size = str.size();
  uint32_t readed = this->__send((void*)&size, sizeof(size));
  if (readed != sizeof(size))
    throw std::string("Can't write string size ");

  readed = this->__send((void*)str.c_str(), size);
  if (readed != size)
    throw std::string("Can't write string");
  
  return (readed);
}

int32_t NetworkStream::write(uint64_t id) 
{
  this->__send((void*)&id, sizeof(id));
  return (sizeof(id));
}

int32_t NetworkStream::read(std::string& readValue)
{    
  uint64_t size = 0;
  if (this->__recv(&size, sizeof(size)) != sizeof(size))
    throw std::string("NetworkStream::read can't get size");

  uint8_t*  value = new uint8_t[size + 1];
  this->__recv(value, size); //test return value
  value[size] = 0;
  readValue = std::string((char*)value, size);
  delete value;

  return (readValue.size());
}

int32_t NetworkStream::read(uint64_t* id)
{
  return (this->__recv((void*)id, sizeof(id)));
}

int32_t NetworkStream::read(void* buff, int32_t size)
{
  return(this->__recv(buff, size ));
}

DStream& NetworkStream::operator<<(DStreamString& input)
{
  this->write(input.str());
  input.clear();

  return (*this);
}

DStream& NetworkStream::operator>>(DStreamString& output)
{
  std::string tmp;
  this->read(tmp);
  output.write(tmp.c_str(), tmp.size());

  return (*this);
}

int32_t NetworkStream::__send(void* buff, int32_t size) 
{
  this->__writeStream.write((char*)buff, size);
  return (size);
}

int32_t NetworkStream::flush(void)// const
{
  int32_t size = this->__writeStream.tellp() - this->__writeStream.tellg();
  char* buff = new char[size];
  this->__writeStream.read(buff, size); //size shouldn't be too big 

  int32_t totalSent = 0;
  while (totalSent < size)
  {
    int32_t sent = ::send(this->__socket, (void*)(buff + totalSent), size - totalSent, 0);
    if (sent != -1)
      totalSent += sent;
  }
  this->__writeStream.seekp(0);
  this->__writeStream.seekg(0);
  delete buff;

  return (size);
}

int32_t NetworkStream::__recv(void* buff, int32_t size)
{
  if ((this->__readStream.tellg() == this->__readStream.tellp())) 
  {
    char tmp[4096];
    int32_t readed;
    int64_t remaining = this->__readStream.tellp() - this->__readStream.tellg();

    if (remaining == 0 || size < remaining)
    {
      this->__readStream.seekp(0);
      this->__readStream.seekg(0);
    }
    
    do 
    {
      readed = (::recv(this->__socket, tmp, 4096, 0));
      if (readed != -1)
        this->__readStream.write(tmp, readed);
    } while (readed == 4096 || readed == -1);
  }

  this->__readStream.read((char*)buff, size);
  return (size);
}


}
