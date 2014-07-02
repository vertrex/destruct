#ifndef __NETWORKSTREAM__
#define __NETWORKSTREAM__

#include <iostream>
#include <stdint.h>

#include "dvalue.hpp"
#include "protocol/dstream.hpp"
#include "protocol/dserialize.hpp"

namespace Destruct
{

class DValue;

class NetworkStream : public DStream
{
public:
//  NetworkStream(int socket);
  NetworkStream(DStruct* dstruct, DValue const& args);
  ~NetworkStream();
  int32_t read(void* buff, int32_t size);
  int32_t read(std::string & readValue);
  int32_t write(void* buff, int32_t size) const ;
  int32_t write(std::string const& str) const; 
//write DValue
//Read dvalue
private:
  void __close(void);
  int32_t __socket; //XXX int64_t ?? for fd
//buffer ? for serialization ?  
};

class DSerializeRPC : public DSerialize
{
public:
  DSerializeRPC()
  {
  }

  const std::string name(void);
  DSerializeRPC*   create(void);

  bool serialize(DStream& output, DObject& dobject); 
  bool deserialize(DStream& input, DObject& dobject);

  bool serialize(DStream& output, DStruct& dstruct); 
  DStruct* deserialize(DStream& input);

// pascal string deserialization
  bool serialize(DStream& output, const std::string& str);
  bool deserialize(DStream& input, std::string& str);

  bool serialize(DStream& output, DValue value, DType type);
  DValue deserialize(DStream& input, DType type); 
//serialization buffer ? non c la stream mais la meme que la stream de comm ?
};

}

#endif
