#ifndef __NETWORKSTREAM__
#define __NETWORKSTREAM__

#include <iostream>
#include <stdint.h>

#include "dvalue.hpp"
#include "protocol/dstream.hpp"

namespace Destruct
{

class DValue;

class NetworkStream : public DStream
{
public:
  NetworkStream(DStruct* dstruct, DValue const& args);
  ~NetworkStream();
  int32_t read(void* buff, int32_t size);
  int32_t read(std::string & readValue);
  int32_t read(uint64_t*  id);
  int32_t write(void* buff, int32_t size) const ;
  int32_t write(std::string const& str) const; 
  int32_t write(uint64_t id) const;
  //DStream& operator<<(std::string val); 
  //DStream& operator<<(char val);
  //DStream& operator<<(DValue val);
  DStream& operator<<(DStreamString& input);
  DStream& operator>>(DStreamString& output);
private:
  void __close(void);
  int32_t __socket; //XXX int64_t ?? for fd
//buffer ? for serialization ?  
};



}

#endif
