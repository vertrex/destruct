#ifndef __CLIENT_FUNCTION_OBJECT_HPP__
#define __CLIENT_FUNCTION_OBJECT_HPP__

#include "dvalue.hpp"
//#include "protocol/dserialize.hpp"

#include "networkstream.hpp"
#include "objectmanager.hpp"
#include "serverfunctionobject.hpp"

using namespace Destruct;


/**
 * Client FunctionObject proxy
 */
class ClientFunctionObject : public DFunctionObject
{
public:
  ClientFunctionObject(DObject* networkStream, DObject* serializer, DObject* deserializer,  uint64_t id, DType::Type_t argumentType, DType::Type_t returnType);
  ~ClientFunctionObject();

  DValue        call(void) const;
  DValue        call(DValue const& args) const;
private:
  uint64_t       __id;
  DObject*       __networkStream;
  DObject*       __serializer;
  DObject*       __deserializer;
  DType::Type_t  __argumentType;
  DType::Type_t  __returnType;
};

#endif
