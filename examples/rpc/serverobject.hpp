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
        //ServerObject(Destruct::NetworkStream& networkStream, DObject* serializer, ObjectManager<DObject*>& objectManager, ObjectManager<ServerFunctionObject*>& functionObjectManager);
  ServerObject(DObject* networkStream, DObject* serializer, ObjectManager<DObject*>& objectManager, ObjectManager<ServerFunctionObject*>& functionObjectManager);

  void                                  getValue(void);
  void                                  setValue(void);
  void                                  call(void);
  void                                  call0(void);
  void                                  functionCall(void);
  void                                  functionCall0(void);
//  Destruct::NetworkStream&              networkStream(void);
  DObject*                              networkStream;
private:
//  Destruct::NetworkStream&              __networkStream;
  DObject*                              __networkStream;
  DObject*                              __serializer;
  ObjectManager<DObject*>&              __objectManager;
  ObjectManager<ServerFunctionObject*>& __functionObjectManager;
  DObject*                              __object;
  uint64_t                              __id;
};

#endif
