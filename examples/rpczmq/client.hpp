#ifndef __CLIENT_HPP__
#define __CLIENT_HPP__

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
  EXPORT Client(DStruct* dstruct, DValue const& args);
  EXPORT ~Client();

  EXPORT static void            declare(void);
  
  EXPORT DStruct*               find(DValue const& args);
  EXPORT DObject*               generate(DValue const& args);
  //EXPORT DObject*             generate(DUnicodeString const& name, DValue const& args);

  EXPORT DObject*               networkStream(void) const;
  EXPORT DObject*               serializeRPC(void) const;
  EXPORT DObject*               deserializeRPC(void) const;
private:
  void                          __connect(DObject* args);
  void                          __setAuth(DObject* rpcAuth);
  void                          __close(void);

  void*                         __context;
  void*                         __socket;

  NetworkStream*                __networkStream;
  SerializeRPC*                 __serialize;
  DeserializeRPC*               __deserialize;

  //protected:
  //EXPORT ~Client();
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
      DAttribute(DType::DStructType, "find", DType::DUnicodeStringType), //return a local struct ...
      DAttribute(DType::DObjectType, "generate", DType::DUnicodeStringType), //can't pass argument ... 
    };

    return (attributes);
  }

  static DPointer<Client>* memberBegin()
  {
     static DPointer<Client> memberPointer[] =
     {
       DPointer<Client>(&Client::_find, &Client::find),
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
