#ifndef __SERVER__
#define __SERVER__

#include "objectmanager.hpp"
#include "serverfunctionobject.hpp"

class Server
{
public:
                        Server();
  virtual void          initRoot(void);
  void                  serve(void);
  void                  showRoot(void);
  ObjectManager<Destruct::DObject*>&        objectManager(void);
  ObjectManager<ServerFunctionObject*>&        functionObjectManager(void);
private:
  ObjectManager<Destruct::DObject*>         __objectManager;
  ObjectManager<ServerFunctionObject*> __functionObjectManager;
  int                   __listenSocket;
  int32_t               __connectionSocket;
  void                  __bind(void);
  void                  __listen(void);
};

#endif
