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

//bind other function & use inheritance in other examples & python
class EXPORT Client : public DCppObject<Client>
{
public:
  Client(DUnicodeString const& addr, uint32_t port);
  Client(DStruct* dstruct, DValue const& args);
  ~Client();

  static void                           declare(void);
  virtual DObject*                      start(void);
  DValue                                findObject(void); ///XXX:

  Destruct::DStruct*                    remoteFind(const DUnicodeString name);
  bool                                  print(DObject* dobject) const;
  bool                                  print(DStruct* dstruct) const;

  int32_t                               connectionSocket(void) const;
  DObject*                              networkStream(void) const;
  DObject*                              serializeRPC(void) const;
  DObject*                              deserializeRPC(void) const;

private:
  void                                  __connect(DUnicodeString const& addr, uint32_t port);
  void                                  __close(void);
  int32_t                               __connectionSocket;
  //NetworkStream*                      __networkStream;
  //DSerialize*                         __serializeRPC; 
  DObject*                              __networkStream;
  DObject*                              __serialize;
  DObject*                              __deserialize;

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
