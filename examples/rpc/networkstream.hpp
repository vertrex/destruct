#ifndef __NETWORKSTREAM__
#define __NETWORKSTREAM__

#include <sstream>
#include <iostream>
#include <string.h>

#include "destruct.hpp"
#include "protocol/dstream.hpp"

namespace Destruct
{

class DValue;

class NetworkStream : public DStream
{
public:
  EXPORT NetworkStream(DStruct* dstruct, DValue const& args);
  EXPORT NetworkStream(NetworkStream const& copy);
  EXPORT ~NetworkStream();

  EXPORT DStream&          operator>>(DStreamString& output);
  EXPORT int32_t           read(void* buff, int32_t size);
  EXPORT int32_t           read(DUnicodeString& readValue);
  EXPORT int32_t           read(uint64_t*  id);

  EXPORT DStream&          operator<<(DStreamString& input);
  EXPORT int32_t           write(DUnicodeString const& str); 
  EXPORT int32_t           write(uint64_t id) ;

  EXPORT int32_t           flush(void) ; 
private:
  int32_t           __send(void* buff, int32_t size); 
  int32_t           __recv(void* buff, int32_t size);

  int32_t           __socket;
  std::stringstream __readStream;
  std::stringstream __writeStream;
};

}

#endif
