#ifndef __DSERIALIZERPC__
#define __DSERIALIZERPC__

#include "protocol/dserialize.hpp"
#include "networkstream.hpp"
#include "server.hpp"

namespace Destruct
{
//class DeserialzieRPC //implem separer ? 

//class SerializeRPC // imeplem sperare ? finnalement plus logique ? 
class DSerializeRPC : public DSerialize
{
public:
  DSerializeRPC(NetworkStream networkStream, ObjectManager & objects);//OBjectManager 
  const std::string name(void); 
  DSerializeRPC*   create(void);

  bool serialize(DStream& output, DObject& dobject); 
  bool deserialize(DStream& input, DObject& dobject);

  bool serialize(DStream& output, DStruct& dstruct); 
  DStruct* deserialize(DStream& input);

// pascal string deserialization
  bool serialize(DStream& output, const std::string& str);
  bool deserialize(DStream& input, std::string& str);

  bool serialize(DStream& output, DValue value, DType::Type_t type);
  DValue deserialize(DStream& input, DType::Type_t type);
private:
  NetworkStream __networkStream;
  ObjectManager & __objects;
//objectManager get ...  
 
//serialization buffer ? non c la stream mais la meme que la stream de comm ?
};

}

#endif
