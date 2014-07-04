#include<sys/socket.h>    //socket
#include<arpa/inet.h> //inet_addr

#include "networkstream.hpp"

namespace Destruct 
{

NetworkStream::NetworkStream(DStruct* dstruct, DValue const& args) : DStream(dstruct)
{
  this->__socket = args.get<DInt32>();
}

NetworkStream::~NetworkStream()
{
  this->__close();
}

int32_t NetworkStream::write(std::string const& str) const
{
  uint64_t size = str.size();
  uint32_t readed = this->write((void*)&size, sizeof(size));
  if (readed != sizeof(size))
    throw std::string("Can't write string size");
 
  readed = this->write((void*)str.c_str(), size);
  if (readed != size)
    throw std::string("Can't write string");
  return str.size();
}

int32_t NetworkStream::write(uint64_t id) const
{
  return (this->write((void*)&id, sizeof(id)));
}

int32_t NetworkStream::write(void* buff, int32_t size) const 
{
  return (send(this->__socket, buff, size, 0));
}

int32_t NetworkStream::read(std::string & readValue)
{
  uint64_t size = 0;
  if (this->read(&size, sizeof(size)) != sizeof(size))
    throw std::string("NetworkStream::read can't get size");
  uint8_t*  value = new uint8_t[size + 1];
  this->read(value, size); //test return value
  value[size] = 0;
  readValue = std::string((char*)value, size);
  delete value;

  return readValue.size();
}

int32_t NetworkStream::read(uint64_t* id)
{
  return (this->read((void*)id, sizeof(id)));
}

int32_t NetworkStream::read(void* buff, int32_t size)
{
  return(recv(this->__socket, buff, size, 0));
}


void NetworkStream::__close(void)
{
}

}
