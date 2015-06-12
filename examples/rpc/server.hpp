#ifndef __SERVER__
#define __SERVER__

#include "objectmanager.hpp"
#include "serverfunctionobject.hpp"
#include "networkstream.hpp"

#include "destruct.hpp"
#include "serializerpc.hpp"

#ifdef WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#endif

class Server : public DCppObject<Server>
{
public:
  EXPORT Server(uint32_t port);
  EXPORT Server(DStruct* dstruct, DValue const& args);

  EXPORT void                           setRoot(RealValue<DObject*> root);
  EXPORT void                           serve();
  EXPORT void                           daemonize(void);

  EXPORT DObject*                       objectManager(void);
  EXPORT void                           initObject(void);

  RealValue<DFunctionObject*>   _setRoot, _serve, _daemonize;

/**
 *  Destruct definition
 */
  static size_t ownAttributeCount()
  {
    return (3);
  }

  static DAttribute* ownAttributeBegin()
  {
    static DAttribute attributes[] = 
    {
      DAttribute(DType::DNoneType, "setRoot", DType::DObjectType), 
      DAttribute(DType::DNoneType, "serve", DType::DNoneType), 
      DAttribute(DType::DNoneType, "daemonize", DType::DNoneType), 
    };

    return (attributes);
  }

  static DPointer<Server>* memberBegin()
  {
     static DPointer<Server> memberPointer[] =
     {
       DPointer<Server>(&Server::_setRoot, &Server::setRoot),
       DPointer<Server>(&Server::_serve,    &Server::serve),
       DPointer<Server>(&Server::_daemonize, &Server::daemonize),
     };

    return (memberPointer);
  } 

  static DAttribute*  ownAttributeEnd()
  {
    return (ownAttributeBegin() + ownAttributeCount());
  }

  static DPointer<Server>* memberEnd()
  {
    return (memberBegin() + ownAttributeCount());
  }

protected:
  EXPORT                                virtual ~Server();
private:
   void                                 showRoot(void);
#ifdef WIN32
  SOCKET			        __listenSocket;
  SOCKET			        __connectionSocket;
#else
  int                                   __listenSocket;
  int32_t                               __connectionSocket;
#endif
  void                                  __bind(int32_t port);
  void                                  __listen(void);
  DObject*                              __objectManager;
};

#endif
