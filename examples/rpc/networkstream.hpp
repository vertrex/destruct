#ifndef __NETWORKSTREAM__
#define __NETWORKSTREAM__

#include <iostream>
#include <stdint.h>

class NetworkStream
{
public:
  NetworkStream(int socket);
  ~NetworkStream();
  int32_t read(void* buff, int32_t size);
  int32_t read(std::string & readValue);
  int32_t write(void* buff, int32_t size) const ;
  int32_t write(std::string const& str) const;
private:
  void __close(void);
  int __socket;
};

#endif
