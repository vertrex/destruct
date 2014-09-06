#ifndef __DSERIALIZERPC__
#define __DSERIALIZERPC__

#include "protocol/dserialize.hpp"
#include "networkstream.hpp"
#include "server.hpp"

namespace Destruct
{

class DSerializeRPC : public DSerialize
{
public:
  DSerializeRPC(NetworkStream& networkStream, ObjectManager & objects);
  DSerializeRPC*        create(void);

  const std::string     name(void); 
  bool                  serialize(DStream& output, DStruct& dstruct); 
  bool                  serialize(DStream& output, DValue value, DType::Type_t type);
  bool                  serialize(DStream& output, DObject* dobject); 
  DStruct*              deserialize(DStream& input);
  DValue                deserialize(DStream& input, DType::Type_t type);
  bool                  deserialize(DStream& input, DObject* dobject);
protected:
  virtual               ~DSerializeRPC() {};
private:
  NetworkStream        __networkStream;
  ObjectManager&        __objects;
  DStreamString         __streamString;
};

}

#endif
