#ifndef __SERVERFUNCTIONOBJECT_HPP__
#define __SERVERFUNCTIONOBJECT_HPP__

#include "dobject.hpp"
#include "dfunction.hpp"
#include "protocol/dcppobject.hpp"

using namespace Destruct;

//in fact it's a dattribute but dmethod can't resolve it own arg
class ServerFunctionObject : public DCppObject<ServerFunctionObject>
{
public:
  ServerFunctionObject(DStruct* dstruct, DValue const& args);
  ServerFunctionObject(ServerFunctionObject const& copy);

  RealValue<DUInt64>            argumentType;
  RealValue<DUInt64>            returnType;
  RealValue<DFunctionObject*>   functionObject;
protected:
  ~ServerFunctionObject();
public:
  static size_t ownAttributeCount()
  {
    return (3);
  }

  static DAttribute* ownAttributeBegin()
  {
    static DAttribute attributes[] = 
    {
      DAttribute(DType::DUInt64Type, "argumentType"), 
      DAttribute(DType::DUInt64Type, "returnType"), 
      DAttribute(DType::DMethodType, "functionObject"), 
    };

    return (attributes);
  }

  static DPointer<ServerFunctionObject>* memberBegin()
  {
     static DPointer<ServerFunctionObject> memberPointer[] =
     {
       DPointer<ServerFunctionObject>(&ServerFunctionObject::argumentType),
       DPointer<ServerFunctionObject>(&ServerFunctionObject::returnType),
       DPointer<ServerFunctionObject>(&ServerFunctionObject::functionObject),
     };

    return (memberPointer);
  } 

  static DAttribute*  ownAttributeEnd()
  {
    return (ownAttributeBegin() + ownAttributeCount());
  }

  static DPointer<ServerFunctionObject>* memberEnd()
  {
    return (memberBegin() + ownAttributeCount());
  } 
};

#endif
