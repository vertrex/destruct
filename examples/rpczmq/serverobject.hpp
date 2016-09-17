#ifndef __SERVER_OBJECT__
#define __SERVER_OBJECT__

#include "networkstream.hpp"
#include "objectmanager.hpp"

class ServerFunctionObject;

namespace Destruct
{
class SerializeRPC;
class DeserializeRPC;
}

using namespace Destruct;

class ServerObject
{
public:
  ServerObject(void* context, void* socket);
  ~ServerObject();

  void                                  find(void);
  void                                  generate(void);

  void                                  getValue(void);
  void                                  setValue(void);
  void                                  call(void);
  void                                  call0(void);
  void                                  functionCall(void);
  void                                  functionCall0(void);
  void                                  unknown(const DUnicodeString& cmd);
  void                                  dispatch(void);
private:
  NetworkStream*                        __networkStream;
  SerializeRPC*                         __serializer;
  DeserializeRPC*                       __deserializer;
  DObject*                              __objectManager;
};

#endif
