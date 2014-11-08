#ifndef __DSERIALIZERPC__
#define __DSERIALIZERPC__

#include "protocol/dserialize.hpp"
#include "networkstream.hpp"
#include "objectmanager.hpp"
#include "serverfunctionobject.hpp"

namespace Destruct
{

class DSerializeRPC : public DSerialize
{
public:
  DSerializeRPC(NetworkStream& networkStream, ObjectManager<DObject*>& objects, ObjectManager<ServerFunctionObject*>& functionObjects);
  DSerializeRPC*        create(void);

  const std::string     name(void); 
  bool                  serialize(DStream& output, DStruct& dstruct); 
  bool                  serialize(DStream& output, DValue value, DType::Type_t type);
  bool                  serialize(DStream& output, DValue value, DType::Type_t argumentType, DType::Type_t returnType);
  bool                  serialize(DStream& output, DObject* dobject); 
  bool                  serialize(DStream& output, DFunctionObject* dfunctionObject, DType::Type_t argumentType, DType::Type_t returnType);
  
  DStruct*              deserialize(DStream& input);
  DValue                deserialize(DStream& input, DType::Type_t type);
  DValue                deserialize(DStream& input, DType::Type_t argumentType, DType::Type_t returnType);
  bool                  deserialize(DStream& input, DObject* dobject);
  //bool                  deserialize(DStream& input, DFunctionObject* dfunctionObject);
protected:
  virtual               ~DSerializeRPC() {};
private:
  NetworkStream              __networkStream;
  ObjectManager<DObject*>&   __objects;
  ObjectManager<ServerFunctionObject*>&   __functionObjects;
  DStreamString              __streamString;
};

}

#endif
