#ifndef __NETWORKSTREAM__
#define __NETWORKSTREAM__

#include "protocol/dstream.hpp"

namespace Destruct
{

class DValue;

class NetworkStream : public DStream
{
public:
  NetworkStream(DStruct* dstruct, DValue const& args);
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
private:
  void    __close(void);
  int32_t __socket;
};

}

#endif
