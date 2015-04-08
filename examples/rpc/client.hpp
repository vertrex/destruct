#ifndef __CLIENT__
#define __CLIENT__

#include <iostream>
#include <stdint.h>

#include "destruct.hpp"
#include "networkstream.hpp" 
#include "objectmanager.hpp"
#include "serializerpc.hpp"
using namespace Destruct;

class ServerFunctionObject;

class Client : public DCppObject<Client>
{
public:
  EXPORT Client(DUnicodeString const& addr, uint32_t port);
  EXPORT Client(DStruct* dstruct, DValue const& args);
  EXPORT ~Client();

  EXPORT virtual DObject*                      start(void);
  EXPORT DValue                                findObject(void); ///XXX:

  EXPORT Destruct::DStruct*                    remoteFind(const DUnicodeString name);
  EXPORT bool                                  print(DObject* dobject) const;
  EXPORT bool                                  print(DStruct* dstruct) const;

  EXPORT int32_t                               connectionSocket(void) const;
  EXPORT NetworkStream*                        networkStream(void) const;
  EXPORT DSerialize*                           serializeRPC(void) const;
  EXPORT ObjectManager<DObject*>&              objectManager(void);
  EXPORT ObjectManager<ServerFunctionObject*>& functionObjectManager(void);

private:
  EXPORT void                                  __connect(DUnicodeString const& addr, uint32_t port);
  EXPORT void                                  __close(void);
  int32_t                                      __connectionSocket;
  NetworkStream*                               __networkStream;
  DSerialize*                                  __serializeRPC; 
  ObjectManager<DObject* >                     __objectManager;
  ObjectManager<ServerFunctionObject*>         __functionObjectManager;

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
