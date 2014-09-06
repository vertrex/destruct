#ifndef __NETWORKSTREAM__
#define __NETWORKSTREAM__

#include "protocol/dstream.hpp"
#include <sstream>
#include <iostream>
#include <string.h> //memcpy 

namespace Destruct
{

class DValue;


class StringBuff
{
public:
  StringBuff(const std::string& recv) //: __data(recv)
  {
    std::string tmp;
  
    uint64_t size = recv.size();
    tmp+= std::string((char*)&size, sizeof(uint64_t));
    tmp+= recv;
    __data = tmp; 
  }

  uint8_t* data(void)
  {
    return ((uint8_t*)__data.c_str());
  }

  size_t size(void)
  {
    return __data.size();
  }

private:
  std::string __data;
};

class Buffer
{
public:
  Buffer() : __data(new char[4096]), __dataSize(0), __readed(0), __written(0)
  {
    std::cout << "Buffer() " << std::endl;
  }

  Buffer(Buffer const& copy) : __data(new char[4096]), __dataSize(copy.__dataSize), __readed(copy.__readed), __written(copy.__written)
  {
    std::cout << "Buffer(const& copy)" << std::endl;
    memcpy(__data, copy.__data, copy.__dataSize);
     
  }

  ~Buffer()
  {
    std::cout << "~Buffer delete" << std::endl;
    delete __data;
  }

  void        add(char* buff, uint64_t size)
  { 
          //std::cout << "Buff::add(buff, " << size << ")" <<  std::endl;
    memcpy((char*)__data + __written, buff, size);
    __dataSize += size;
    __written += size;
    //add at end pos if not used 
  }

  std::string   get(void)
  {
          //std::cout << "Buffer::get() at pos " << __readed << std::endl;
     uint64_t size= *((uint64_t*)(this->__data + __readed));
     std::string res((char*)this->__data + __readed + sizeof(uint64_t), size);

   //  memcpy(__dataRead + size, __data, __dataSize - size); //copy sur lui meme 1
     this-> __readed += size + sizeof(uint64_t);
     //std::cout << "Buffer::get() get data of size " << size + sizeof(uint64_t) << std::endl;
     //std::cout << "THIS READED " << this->__readed << std::endl;
     return (res);
    //get from current pos ;
  }

  bool  toRead(void)
  {
          //std::cout << "toRead " << this->__readed << " " << this->__dataSize << std::endl;
    if (this->__readed < this->__dataSize)
      return true;
    return false;
  }

  char* data(void)
  {
    return (__data);
  }
private:
  char*          __data;
  uint64_t       __dataSize;
  uint64_t       __readed;
  uint64_t       __written;
};


class NetworkStream : public DStream
{
public:
  NetworkStream(DStruct* dstruct, DValue const& args);
  NetworkStream(NetworkStream const& copy);
  ~NetworkStream();
  DStream& operator>>(DStreamString& output);
  int32_t read(void* buff, int32_t size);
  int32_t read(std::string & readValue);
  int32_t read(uint64_t*  id);

  DStream& operator<<(DStreamString& input);
  int32_t write(void* buff, int32_t size) const;
  int32_t write(std::string const& str) const; 
  //DStream& operator<<(std::string val); 
  int32_t write(uint64_t id) const;
  //DStream& operator<<(char val);
  //DStream& operator<<(DValue val);

// Read network data and populate buffer act as cache to avoid multiple-read write 
//void    send(void) const;
//void    recv(void) const;

private:
  void    __close(void);
  int32_t __socket;

  uint8_t* __readBuffer;
  Buffer __buff;
  //uint8_t* __offset;
//  uint8_t* __
//  int8_t* __writeBuffer
};

}

#endif
