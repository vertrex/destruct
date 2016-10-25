#ifndef __SERVER_HPP__
#define __SERVER_HPP__

#include "destruct.hpp"

#include "serializerpc.hpp"
#include "serverfunctionobject.hpp"
#include "objectmanager.hpp"
#include "networkstream.hpp"

class Server : public DCppObject<Server>
{
public:
  EXPORT Server(DValue const& args);
  EXPORT Server(DStruct* dstruct, DValue const& args);
  EXPORT ~Server();

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
    return (2);
  }

  static DAttribute* ownAttributeBegin()
  {
    static DAttribute attributes[] = 
    {
      DAttribute(DType::DNoneType, "serve", DType::DNoneType), 
      DAttribute(DType::DNoneType, "daemonize", DType::DNoneType), 
    };

    return (attributes);
  }

  static DPointer<Server>* memberBegin()
  {
     static DPointer<Server> memberPointer[] =
     {
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

private:
  void*                                 __socket;
  void*                                 __context;

  void                                  __bind(DObject* args);
  void                                  __setAuth(DObject* rpcAuth);
  DObject*                              __objectManager;
};

#endif
