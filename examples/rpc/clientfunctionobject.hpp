#ifndef __CLIENT_FUNCTION_OBJECT_HPP__
#define __CLIENT_FUNCTION_OBJECT_HPP__

#include "dvalue.hpp"
#include "protocol/dserialize.hpp"

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
  ClientFunctionObject(NetworkStream& networkStream, uint64_t id, ObjectManager<DObject*>& objects, ObjectManager<ServerFunctionObject*>& functionObjects, DType::Type_t argumentType, DType::Type_t returnType);
  ~ClientFunctionObject();

  DValue call(void) const;
  DValue call(DValue const& args) const;
private:
  uint64_t       __id;
  NetworkStream&  __networkStream;
  DSerialize*    __serializer;
  DType::Type_t         __argumentType;
  DType::Type_t         __returnType;
};

#endif
