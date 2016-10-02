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
class Client : public DCppObject<Client>
{
public:
  EXPORT Client(DUnicodeString const& addr, uint32_t port);
  EXPORT Client(DStruct* dstruct, DValue const& args);

  EXPORT static void                   declare(void);
  EXPORT virtual DObject*              start(void);
  EXPORT DObject*                      generate(DValue const& args);
  EXPORT DStruct*	               find(DValue const& args);
  EXPORT bool                          print(DObject* dobject) const;
  EXPORT bool                          print(DStruct* dstruct) const;

  EXPORT int32_t                       connectionSocket(void) const;
  EXPORT DObject*                      networkStream(void) const;
  EXPORT DObject*                      serializeRPC(void) const;
  EXPORT DObject*                      deserializeRPC(void) const;

private:
  void                          __connect(DUnicodeString const& addr, uint32_t port);
  void                          __close(void);
  int32_t                       __connectionSocket;
  DObject*                      __networkStream;
  DObject*                      __serialize;
  DObject*                      __deserialize;

protected:
  EXPORT ~Client();
public:
/**
 *  Destruct definition
 */
  RealValue<DFunctionObject*>   _find;
  RealValue<DFunctionObject*>   _generate;
 
  static size_t ownAttributeCount()
  {
    return (2);
  }

  static DAttribute* ownAttributeBegin()
  {
    static DAttribute attributes[] = 
    {
      DAttribute(DType::DStructType, "find", DType::DUnicodeStringType),
      DAttribute(DType::DObjectType, "generate", DType::DUnicodeStringType), 
    };

    return (attributes);
  }

  static DPointer<Client>* memberBegin()
  {
     static DPointer<Client> memberPointer[] =
     {
       DPointer<Client>(&Client::_find, &Client::find), //useless use creatRoot (registry ...)
       DPointer<Client>(&Client::_generate, &Client::generate),
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
