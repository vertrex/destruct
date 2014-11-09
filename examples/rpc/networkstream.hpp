#ifndef __NETWORKSTREAM__
#define __NETWORKSTREAM__

#include <sstream>
#include <iostream>
#include <string.h>

#include "protocol/dstream.hpp"

namespace Destruct
{

class DValue;

class NetworkStream : public DStream
{
public:
  NetworkStream(DStruct* dstruct, DValue const& args);
  NetworkStream(NetworkStream const& copy);
  ~NetworkStream();

  DStream&          operator>>(DStreamString& output);
  int32_t           read(void* buff, int32_t size);
  int32_t           read(std::string & readValue);
  int32_t           read(uint64_t*  id);

  DStream&          operator<<(DStreamString& input);
  int32_t           write(std::string const& str); 
  int32_t           write(uint64_t id) ;

  int32_t           flush(void) ; 
private:
  int32_t           __send(void* buff, int32_t size); 
  int32_t           __recv(void* buff, int32_t size);

  int32_t           __socket;
  std::stringstream __readStream;
  std::stringstream __writeStream;
};

}

#endif
