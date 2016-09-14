#ifndef __SERVER_OBJECT__
#define __SERVER_OBJECT__

#include "networkstream.hpp"
#include "objectmanager.hpp"
//#include "protocol/dserialize.hpp"

using namespace Destruct;

class ServerFunctionObject;

class ServerObject
{
public:
  ServerObject(void* context, void* socket);
  ~ServerObject();

  void                                  getValue(void);
  void                                  setValue(void);
  void                                  call(void);
  void                                  call0(void);
  void                                  functionCall(void);
  void                                  functionCall0(void);
  void                                  findDStruct(void);
  void                                  unknown(const DUnicodeString& cmd);
  DUnicodeString                        cmd(void);
private:
  DObject*                              __networkStream;
  DObject*                              __serializer;
  DObject*                              __deserializer;
  DObject*                              __objectManager;
};

#endif
