#ifndef __CLIENT_FUNCTION_OBJECT_HPP__
#define __CLIENT_FUNCTION_OBJECT_HPP__

#include "dvalue.hpp"

#include "networkstream.hpp"
#include "objectmanager.hpp"
#include "serverfunctionobject.hpp"

namespace Destruct
{
  class SerializeRPC;
  class DeserializeRPC;
}

using namespace Destruct;

/**
 * Client FunctionObject proxy
 */
class ClientFunctionObject : public DFunctionObject
{
public:
  ClientFunctionObject(DObject* networkStream, DObject* serializer, DObject* deserializer,  uint64_t id, DType::Type_t argumentType, DType::Type_t returnType);

  DValue        call(void) const;
  DValue        call(DValue const& args) const;
protected:
  virtual ~ClientFunctionObject();
private:
  uint64_t             __id;
  NetworkStream*       __networkStream;
  SerializeRPC*        __serializer;
  DeserializeRPC*      __deserializer;
  DType::Type_t        __argumentType;
  DType::Type_t        __returnType;
};

#endif
