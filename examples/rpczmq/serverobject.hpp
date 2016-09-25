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

#define CMD_FIND                0
#define CMD_GENERATE            1
#define CMD_SETVALUE            2
#define CMD_GETVALUE            3
#define CMD_CALL                4
#define CMD_CALL0               5
#define CMD_FUNCTIONCALL        6
#define CMD_FUNCTIONCALL0       7

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
  void                                  dispatch(void);
private:
  NetworkStream*                        __networkStream;
  SerializeRPC*                         __serializer;
  DeserializeRPC*                       __deserializer;
  ObjectManager*                        __objectManager;
};

#endif
