#ifndef __STUBFUNCTIONOBJECT_HPP__
#define __STUBFUNCTIONOBJECT_HPP__

#include "dvalue.hpp"

#include "networkstream.hpp"
#include "objectmanager.hpp"
#include "serverfunctionobject.hpp"

class SerializeRPC;
class DeserializeRPC;

using namespace Destruct;

/**
 * Client FunctionObject proxy
 */
class StubFunctionObject : public DFunctionObject
{
public:
  StubFunctionObject(DObject* networkStream, DObject* serializer, DObject* deserializer,  uint64_t id, DType::Type_t argumentType, DType::Type_t returnType);

  DValue        call(void) const;
  DValue        call(DValue const& args) const;
protected:
  virtual ~StubFunctionObject();
private:
  uint64_t             __id;
  NetworkStream*       __networkStream;
  SerializeRPC*        __serializer;
  DeserializeRPC*      __deserializer;
  DType::Type_t        __argumentType;
  DType::Type_t        __returnType;
};

#endif
