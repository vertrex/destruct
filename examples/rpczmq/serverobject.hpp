#ifndef __SERVEROBJECT_HPP__
#define __SERVEROBJECT_HPP__

#include "networkstream.hpp"
#include "objectmanager.hpp"
#include "czmq.h"

class ServerFunctionObject;
class SerializeRPC;
class DeserializeRPC;

#define CMD_FIND                0
#define CMD_GENERATE            1
#define CMD_SETVALUE            2
#define CMD_GETVALUE            3
#define CMD_CALL                4
#define CMD_CALL0               5
#define CMD_FUNCTIONCALL        6
#define CMD_FUNCTIONCALL0       7
#define CMD_GENERATE_ARG        8

using namespace Destruct;

class ServerObject
{
public:
  ServerObject(void* context, void* socket);
  ~ServerObject();

  void                                  find(zmsg_t* msg);
  void                                  generate(zmsg_t* msg);
  void                                  generateArg(zmsg_t* msg);

  void                                  getValue(zmsg_t* msg);
  void                                  setValue(zmsg_t* msg);
  void                                  call(zmsg_t* msg);
  void                                  call0(zmsg_t* msg);
  void                                  functionCall(zmsg_t* msg);
  void                                  functionCall0(zmsg_t* msg);
  void                                  dispatch();
private:
  NetworkStream*                        __networkStream;
  SerializeRPC*                         __serializer;
  DeserializeRPC*                       __deserializer;
  ObjectManager*                        __objectManager;
};

#endif
