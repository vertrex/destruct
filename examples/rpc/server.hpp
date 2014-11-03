#ifndef __SERVER__
#define __SERVER__

#include <stdint.h>
#include <map>

#include "networkstream.hpp"
#include "ddynamicobject.hpp"
#include "protocol/dcppobject.hpp"
#include "fsobject.hpp"
#include "protocol/dserialize.hpp"
#include "rpcserver.hpp"

#include "dobject.hpp"
class ServerFunctionObject
{
public:
  ServerFunctionObject(DFunctionObject* functionObject, DType::Type_t argumentType, DType::Type_t returnType);
  DType::Type_t argumentType(void) const;
  DType::Type_t returnType(void) const;
  DFunctionObject* functionObject(void) const;
private:
  DFunctionObject* __functionObject;
  DType::Type_t __argumentType;
  DType::Type_t __returnType;
};

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
