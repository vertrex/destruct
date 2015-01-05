#ifndef __SERVER__
#define __SERVER__

#include "objectmanager.hpp"
#include "serverfunctionobject.hpp"
#include "networkstream.hpp"

#include "serializerpc.hpp"
#include "protocol/dserialize.hpp"

class Server : public DCppObject<Server>
{
public:
  Server(uint32_t port);

  Server(DStruct* dstruct, DValue const& args) : DCppObject<Server>(dstruct, args)
  {
    this->init();
    this->__bind(args.get<DUInt32>());
  }

  ~Server();

  virtual void                          initRoot(void) //setRoot(DValue object);
  {
    //throw DException("Not implemented");
    std::cout << "Server::initRoot not implemented" << std::endl;
  }

  virtual void                          addRoot(RealValue<DObject*> root)
  {
    this->objectManager().registerObject(root);
  }

  virtual void                          serve();
  void                                  daemonize(void);


  ObjectManager<ServerFunctionObject*>& functionObjectManager(void);
  ObjectManager<Destruct::DObject*>&    objectManager(void);

  RealValue<DFunctionObject*>   _addRoot, _serve, _daemonize;

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
      DAttribute(DType::DNoneType, "addRoot", DType::DObjectType), 
      DAttribute(DType::DNoneType, "serve", DType::DNoneType), 
      DAttribute(DType::DNoneType, "daemonize", DType::DNoneType), 
    };

    return (attributes);
  }

  static DPointer<Server>* memberBegin()
  {
     static DPointer<Server> memberPointer[] =
     {
       DPointer<Server>(&Server::_addRoot, &Server::addRoot),
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
  void                                  findDStruct(void);
  void                                  unknown(const std::string& cmd);
  void                                  showRoot(void);

  int                                   __listenSocket;
  int32_t                               __connectionSocket;
  void                                  __bind(int32_t port);
  void                                  __listen(void);
  NetworkStream*                        __networkStream;
  DSerialize*                           __serializer;
  ObjectManager<Destruct::DObject*>     __objectManager;
  ObjectManager<ServerFunctionObject*>  __functionObjectManager;
};

#endif
