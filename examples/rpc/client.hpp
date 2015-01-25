#ifndef __CLIENT__
#define __CLIENT__

#include <iostream>
#include <stdint.h>

#include "networkstream.hpp" 
#include "objectmanager.hpp"
#include "serializerpc.hpp"
using namespace Destruct;

class ServerFunctionObject;

class Client : public DCppObject<Client>
{
public:
  Client(DUnicodeString const& addr, uint32_t port);
  Client(DStruct* dstruct, DValue const& args);
  ~Client();

  virtual DObject*                      start(void);
  DValue                                findObject(void); ///XXX:

  Destruct::DStruct*                    remoteFind(const DUnicodeString name);
  bool                                  print(DObject* dobject) const;
  bool                                  print(DStruct* dstruct) const;

  int32_t                               connectionSocket(void) const;
  NetworkStream*                        networkStream(void) const;
  DSerialize*                           serializeRPC(void) const;
  ObjectManager<DObject*>&              objectManager(void);
  ObjectManager<ServerFunctionObject*>& functionObjectManager(void);
private:
  void                                  __connect(DUnicodeString const& addr, uint32_t port);
  void                                  __close(void);
  int32_t                               __connectionSocket;
  NetworkStream*                        __networkStream;
  DSerialize*                           __serializeRPC; 
  ObjectManager<DObject* >              __objectManager;
  ObjectManager<ServerFunctionObject*>  __functionObjectManager;

public:
/**
 *  Destruct definition
 */
  RealValue<DFunctionObject*>         _findObject;
 
  static size_t ownAttributeCount()
  {
    return (1);
  }

  static DAttribute* ownAttributeBegin()
  {
    static DAttribute attributes[] = 
    {
      DAttribute(DType::DObjectType, "findObject", DType::DNoneType), 
    };

    return (attributes);
  }

  static DPointer<Client>* memberBegin()
  {
     static DPointer<Client> memberPointer[] =
     {
       DPointer<Client>(&Client::_findObject, &Client::findObject),
     };

    return (memberPointer);
  } 

  static DAttribute*  ownAttributeEnd()
  {
    return (ownAttributeBegin() + ownAttributeCount());
  }

  static DPointer<Client>* memberEnd()
  {
    return (memberBegin() + ownAttributeCount());
  }
};

#endif
