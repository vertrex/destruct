#ifdef WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <arpa/inet.h>
#endif
#include <iostream>
#include <sstream>
#include <string.h>
#include <signal.h>

#include "networkstream.hpp"

namespace Destruct 
{

NetworkStream::NetworkStream(DStruct* dstruct, DValue const& args) : DCppObject<NetworkStream>(dstruct, args)
{
  this->init();
  this->__socket = args.get<DInt32>();
#ifndef WIN32
  signal(SIGPIPE, SIG_IGN);
#endif
}

NetworkStream::NetworkStream(const NetworkStream& copy) : DCppObject<NetworkStream>(copy), __socket(copy.__socket)
{
  this->init();
}

NetworkStream::~NetworkStream()
{
}

DBuffer NetworkStream::read(DValue const& args) //passer un buffer comme ca c deja allouer et c a eux de le delete ?
{
  DInt64 size = args.get<DInt64>(); //DUInt32 ? 
  if (!this->__readStream.toRead()) 
  {
    char tmp[4096];
    int32_t readed;
    
    int64_t remaining = this->__readStream.toRead();
    if (remaining == 0 || size < remaining)
    {
      this->__readStream.reset(); // size < remaining ???
    }
    
    do 
    {
      readed = (::recv(this->__socket, tmp, 4096, 0));
      if (readed != -1)
        this->__readStream.write(tmp, readed);
      else
        throw DException("NetworkStream::recv error");
    } while (readed == 4096 || readed == -1);
  }

  DBuffer buffer(size);
  
  this->__readStream.read((char*)buffer.data(), (uint32_t)size);
  return (buffer);
}

DInt64  NetworkStream::write(DValue const& args)
{
  DBuffer buffer = args.get<DBuffer>();

  const char* data = (const char*)buffer.data();
  int64_t size = buffer.size();

  this->__writeStream.write(data, (uint32_t)size);
  return (size); //XXX check
}

void NetworkStream::flush(void)// const
{
  int32_t size = this->__writeStream.toRead();
  char* buff = new char[size];
  this->__writeStream.read(buff, size); //size shouldn't be too big 

  int32_t totalSent = 0;
  while (totalSent < size)
  {
    int32_t sent = ::send(this->__socket, (const char*)(buff + totalSent), size - totalSent, 0);
    if (sent != -1)
      totalSent += sent;
    else
      throw DException("NetworkStream::send error"); //EPIPE error is handled by signal so return -1, check if EPIPE or not ...
  }
  this->__writeStream.reset();
  delete[] buff;

  return ;
}

/**
 *  StreamBuffer
 */
StreamBuffer::StreamBuffer() : __buffer(new char[4096]), __currentRead(0), __currentWrite(0), __maxSize(4096)
{
}

StreamBuffer::StreamBuffer(const StreamBuffer& copy) : __buffer(new char[copy.__maxSize]), __currentRead(copy.__currentRead), __currentWrite(copy.__currentWrite), __maxSize(copy.__maxSize)
{
  memcpy(__buffer, copy.__buffer, copy.__maxSize);
}

StreamBuffer::~StreamBuffer()
{
  delete[] __buffer;
}

void    StreamBuffer::write(const char* inbuff, uint32_t size)
{
  if (this->__currentWrite + size > this->__maxSize) //XXX maxSize > realloc until some size ?
  {
    //std::cout << "Write overflow " << currentWrite + size << " > " << this->__maxSize << std::endl;
    throw DException("StreamBuffer::WriteOverflow");
  }
  memcpy(this->__buffer + this->__currentWrite, inbuff, size);
  this->__currentWrite += size;
}

void    StreamBuffer::read(char* inbuff, uint32_t size)
{
  if (this->__currentRead + size > this->__maxSize)
  {
    //std::cout << "Read overflow " << currentRead + size << " > " << this->__maxSize << std::endl;
    throw DException("StreamBuffer::ReadOverflow");
  }
  memcpy(inbuff, this->__buffer + this->__currentRead, size);
  this->__currentRead += size;
}

uint32_t        StreamBuffer::toRead(void)
{
  return (this->__currentWrite - this->__currentRead);
}

void            StreamBuffer::reset(void)
{
  this->__currentRead = 0;
  this->__currentWrite = 0;
}

}
