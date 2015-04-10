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
      else
        throw DException("NetworkStream::recv error");
    } while (readed == 4096 || readed == -1);
  }

  uint8_t* buff = new uint8_t[size];
  

  this->__readStream.read((char*)buff, size);

  return DBuffer(buff, size);
}

//int32_t NetworkStream::__send(void* buff, int32_t size) 
//{
//this->__writeStream.write((char*)buff, size);
//return (size);
////}

DInt64  NetworkStream::write(DValue const& args)
{
  DBuffer buffer = args.get<DBuffer>();

  const char* data = (const char*)buffer.data();
  int64_t size = buffer.size();

  //if (size)
  //{
  //this->socket
  //
  ////std::cout.write(data, size); // << std::string(buff, size);
  ////if (std::cout.fail())
  ////throw DException("NetworkStream::write Can't write data of size " + size);
  //}
  this->__writeStream.write(data, size);
  return (size); //XXX check
}

void NetworkStream::flush(void)// const
{
  int32_t size = this->__writeStream.tellp() - this->__writeStream.tellg();
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
  this->__writeStream.seekp(0);
  this->__writeStream.seekg(0);
  delete[] buff;

  return ;
  //return (size);
}

/*
int32_t NetworkStream::write(DUnicodeString const& str) //serialize DUNICDE STRINGT 
{
  uint64_t size = str.size();
  uint32_t readed = this->__send((void*)&size, sizeof(size));
  if (readed != sizeof(size))
    throw DException("NetworkStream::write Can't write string size ");

  readed = this->__send((void*)str.c_str(), size);
  if (readed != size)
    throw DException("NetworkStream::write Can't write string");
  
  return (readed);
}
*/

//int32_t NetworkStream::write(uint64_t id)  //Serialize UINT64 
//{
//this->__send((void*)&id, sizeof(id));
//return (sizeof(id));
//}

//int32_t NetworkStream::read(DUnicodeString& readValue) //?SERIALIZE DUNICODE STRINGT
//{    
//uint64_t size = 0;
//if (this->__recv(&size, sizeof(size)) != sizeof(size))
//throw DException("NetworkStream::read can't get size");
//
//uint8_t*  value = new uint8_t[size + 1];
//this->__recv(value, size); //test return value
//value[size] = 0;
//readValue = DUnicodeString(std::string((char*)value, size));
//delete[] value;
//
//return (readValue.size());
//}

//int32_t NetworkStream::read(uint64_t* id) //deserialize int64
//{
//return (this->__recv((void*)id, sizeof(*id)));
//}

//int32_t NetworkStream::read(void* buff, int32_t size)
//{
//return(this->__recv(buff, size ));
//}

//DStream& NetworkStream::operator<<(DStreamString& input) //write stream ?? use dunicode string ?
//{
//this->write(input.str());
//input.clear();

//return (*this);
//}

//DStream& NetworkStream::operator>>(DStreamString& output) // ?? use dunicode string ?
//{
//DUnicodeString tmp;
//this->read(tmp);
//output.write(tmp.c_str(), tmp.size());
//
//return (*this);
//}

//int32_t NetworkStream::__send(void* buff, int32_t size) 
//{
//this->__writeStream.write((char*)buff, size);
//return (size);
//}




}
