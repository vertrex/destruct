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
  EXPORT DValue                        findObject(void); ///XXX:
  EXPORT DValue                        createRoot(DUnicodeString objectName); ///XXX:

  EXPORT DStruct*	               remoteFind(const DUnicodeString name);
  EXPORT bool                          print(DObject* dobject) const;
  EXPORT bool                          print(DStruct* dstruct) const;

  //EXPORT int32_t                       connectionSocket(void) const;
  EXPORT DObject*                      networkStream(void) const;
  EXPORT DObject*                      serializeRPC(void) const;
  EXPORT DObject*                      deserializeRPC(void) const;

private:
  void                          __connect(DUnicodeString const& addr, uint32_t port);
  void                          __close(void);

  void*                         __context;
  void*                         __socket;

  DObject*                      __networkStream;
  DObject*                      __serialize;
  DObject*                      __deserialize;

protected:
  EXPORT ~Client();
public:
/**
 *  Destruct definition
 */
  RealValue<DFunctionObject*>   _findObject;
  RealValue<DFunctionObject*>   _createRoot;
 
  static size_t ownAttributeCount()
  {
    return (2);
  }

  static DAttribute* ownAttributeBegin()
  {
    static DAttribute attributes[] = 
    {
      DAttribute(DType::DObjectType, "findObject", DType::DNoneType), //useless
      DAttribute(DType::DObjectType, "createRoot", DType::DUnicodeStringType), 
    };

    return (attributes);
  }

  static DPointer<Client>* memberBegin()
  {
     static DPointer<Client> memberPointer[] =
     {
       DPointer<Client>(&Client::_findObject, &Client::findObject), //useless use creatRoot (registry ...)
       DPointer<Client>(&Client::_createRoot, &Client::createRoot),
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
