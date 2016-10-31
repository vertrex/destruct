#ifndef __DSERIALIZERPC_HPP__
#define __DSERIALIZERPC_HPP__

#include "czmq.h"

#include "protocol/dcppobject.hpp"

#include "networkstream.hpp"
#include "objectmanager.hpp"
#include "serverfunctionobject.hpp"

using namespace Destruct;

class SerializeRPC : public DCppObject<SerializeRPC> 
{
public:
  SerializeRPC(DStruct* dstruct, DValue const& args);
  SerializeRPC(SerializeRPC const& copy);

  DOpaque       sDInt8(DValue const& args);
  DOpaque       sDInt16(DValue const& args);
  DOpaque       sDInt32(DValue const& args);
  DOpaque       sDInt64(DValue const& args);

  DOpaque       sDUInt8(DValue const& args);
  DOpaque       sDUInt16(DValue const& args);
  DOpaque       sDUInt32(DValue const& args);
  DOpaque       sDUInt64(DValue const& args);

  DOpaque       sDUnicodeString(DValue const& args);

  DOpaque       sDObject(DValue const& args);
  DOpaque       sDMethod(DValue const& args);
  DOpaque       sDNone(void);
  DOpaque       sDStruct(DValue const& args);
  DOpaque       sDBuffer(DValue const& args);

  DObject*   __objectManager; //RV
protected:
  ~SerializeRPC();

public:
  RealValue<DFunctionObject* > _DObject, _DStruct, _DUnicodeString, _DNone, _DMethod, _DBuffer, 
                               _DInt8, _DInt16, _DInt32, _DInt64,
                               _DUInt8, _DUInt16, _DUInt32, _DUInt64;

  RealValue<DObject*>          __stream;
  NetworkStream*               __networkStream;

  static size_t ownAttributeCount()
  {
    return (15);
  }

  static DAttribute* ownAttributeBegin()
  {
    static DAttribute  attributes[] = 
    {
       DAttribute(DType::DObjectType, "stream"),

       DAttribute(DType::DOpaqueType,"DInt8", DType::DInt8Type), 
       DAttribute(DType::DOpaqueType,"DInt16", DType::DInt16Type), 
       DAttribute(DType::DOpaqueType,"DInt32", DType::DInt32Type), 
       DAttribute(DType::DOpaqueType,"DInt64", DType::DInt64Type), 

       DAttribute(DType::DOpaqueType,"DUInt8", DType::DUInt8Type), 
       DAttribute(DType::DOpaqueType,"DUInt16", DType::DUInt16Type), 
       DAttribute(DType::DOpaqueType,"DUInt32", DType::DUInt32Type), 
       DAttribute(DType::DOpaqueType,"DUInt64", DType::DUInt64Type), 
       
       DAttribute(DType::DOpaqueType, "DUnicodeString", DType::DUnicodeStringType),

       DAttribute(DType::DOpaqueType, "DObject", DType::DObjectType), 
       DAttribute(DType::DOpaqueType, "DMethod", DType::DMethodType),
       DAttribute(DType::DOpaqueType, "DNone", DType::DNoneType),
       DAttribute(DType::DOpaqueType, "DStruct", DType::DStructType), 
       DAttribute(DType::DOpaqueType, "DBuffer", DType::DBufferType), 

    };
    return (attributes);
  }

  static DPointer<SerializeRPC>* memberBegin()
  {
    static DPointer<SerializeRPC> memberPointer[] = 
    {
       DPointer<SerializeRPC>(&SerializeRPC::__stream),
        
       DPointer<SerializeRPC>(&SerializeRPC::_DInt8, &SerializeRPC::sDInt8),
       DPointer<SerializeRPC>(&SerializeRPC::_DInt16, &SerializeRPC::sDInt16),
       DPointer<SerializeRPC>(&SerializeRPC::_DInt32, &SerializeRPC::sDInt32),
       DPointer<SerializeRPC>(&SerializeRPC::_DInt64, &SerializeRPC::sDInt64),

       DPointer<SerializeRPC>(&SerializeRPC::_DUInt8, &SerializeRPC::sDUInt8),
       DPointer<SerializeRPC>(&SerializeRPC::_DUInt16, &SerializeRPC::sDUInt16),
       DPointer<SerializeRPC>(&SerializeRPC::_DUInt32, &SerializeRPC::sDUInt32),
       DPointer<SerializeRPC>(&SerializeRPC::_DUInt64, &SerializeRPC::sDUInt64),

       DPointer<SerializeRPC>(&SerializeRPC::_DUnicodeString, &SerializeRPC::sDUnicodeString),

       DPointer<SerializeRPC>(&SerializeRPC::_DObject, &SerializeRPC::sDObject),
       DPointer<SerializeRPC>(&SerializeRPC::_DMethod, &SerializeRPC::sDMethod),
       DPointer<SerializeRPC>(&SerializeRPC::_DNone, &SerializeRPC::sDNone),
       DPointer<SerializeRPC>(&SerializeRPC::_DStruct, &SerializeRPC::sDStruct),
       DPointer<SerializeRPC>(&SerializeRPC::_DBuffer, &SerializeRPC::sDBuffer),
    };
    return (memberPointer);
  }

  static DAttribute* ownAttributeEnd()
  {
    return (ownAttributeBegin() + ownAttributeCount());
  }

  static DPointer<SerializeRPC>*  memberEnd()
  {
    return (memberBegin() + ownAttributeCount());
  } 
};

class DeserializeRPC : public DCppObject<DeserializeRPC> 
{
public:
  DeserializeRPC(DStruct* dstruct, DValue const& args);
  DeserializeRPC(DeserializeRPC const& copy);

  DInt8              dDInt8(DOpaque msg);
  DInt16             dDInt16(DOpaque msg);
  DInt32             dDInt32(DOpaque msg);
  DInt64             dDInt64(DOpaque msg);

  DUInt8             dDUInt8(DOpaque msg);
  DUInt16            dDUInt16(DOpaque msg);
  DUInt32            dDUInt32(DOpaque msg);
  DUInt64            dDUInt64(DOpaque msg);

  DUnicodeString     dDUnicodeString(DOpaque msg);

  DObject*           dDObject(DOpaque msg);
  DFunctionObject*   dDMethod(DOpaque msg);
  DObject*           dDNone(DOpaque msg);
  DStruct*           dDStruct(DOpaque msg);
  DBuffer            dDBuffer(DOpaque msg);

protected:
  ~DeserializeRPC();

public:
  RealValue<DFunctionObject* > _DObject, _DStruct, _DUnicodeString, _DNone, _DMethod, _DBuffer, 
                               _DInt8, _DInt16, _DInt32, _DInt64,
                               _DUInt8, _DUInt16, _DUInt32, _DUInt64;

  RealValue<DObject*>          __stream;
  NetworkStream*               __networkStream;

  static size_t ownAttributeCount()
  {
    return (15);
  }

  static DAttribute* ownAttributeBegin()
  {
    static DAttribute  attributes[] = 
    {
       DAttribute(DType::DObjectType, "stream"),

       DAttribute(DType::DInt8Type, "DInt8", DType::DOpaqueType), 
       DAttribute(DType::DInt16Type,"DInt16", DType::DOpaqueType), 
       DAttribute(DType::DInt32Type,"DInt32", DType::DOpaqueType), 
       DAttribute(DType::DInt64Type,"DInt64", DType::DOpaqueType), 

       DAttribute(DType::DUInt8Type, "DUInt8", DType::DOpaqueType), 
       DAttribute(DType::DUInt16Type,"DUInt16", DType::DOpaqueType), 
       DAttribute(DType::DUInt32Type,"DUInt32", DType::DOpaqueType), 
       DAttribute(DType::DUInt64Type,"DUInt64", DType::DOpaqueType), 
       
       DAttribute(DType::DUnicodeStringType, "DUnicodeString", DType::DOpaqueType),

       DAttribute(DType::DObjectType, "DObject", DType::DOpaqueType), 
       DAttribute(DType::DMethodType, "DMethod", DType::DOpaqueType),
       DAttribute(DType::DNoneType,   "DNone", DType::DOpaqueType),
       DAttribute(DType::DStructType, "DStruct", DType::DOpaqueType), 
       DAttribute(DType::DBufferType, "DBuffer", DType::DOpaqueType), 
    };
    return (attributes);
  }

  static DPointer<DeserializeRPC>* memberBegin()
  {
    static DPointer<DeserializeRPC> memberPointer[] = 
    {
       DPointer<DeserializeRPC>(&DeserializeRPC::__stream),
        
       DPointer<DeserializeRPC>(&DeserializeRPC::_DInt8, &DeserializeRPC::dDInt8),
       DPointer<DeserializeRPC>(&DeserializeRPC::_DInt16, &DeserializeRPC::dDInt16),
       DPointer<DeserializeRPC>(&DeserializeRPC::_DInt32, &DeserializeRPC::dDInt32),
       DPointer<DeserializeRPC>(&DeserializeRPC::_DInt64, &DeserializeRPC::dDInt64),

       DPointer<DeserializeRPC>(&DeserializeRPC::_DUInt8, &DeserializeRPC::dDUInt8),
       DPointer<DeserializeRPC>(&DeserializeRPC::_DUInt16, &DeserializeRPC::dDUInt16),
       DPointer<DeserializeRPC>(&DeserializeRPC::_DUInt32, &DeserializeRPC::dDUInt32),
       DPointer<DeserializeRPC>(&DeserializeRPC::_DUInt64, &DeserializeRPC::dDUInt64),

       DPointer<DeserializeRPC>(&DeserializeRPC::_DUnicodeString, &DeserializeRPC::dDUnicodeString),

       DPointer<DeserializeRPC>(&DeserializeRPC::_DObject, &DeserializeRPC::dDObject),
       DPointer<DeserializeRPC>(&DeserializeRPC::_DMethod, &DeserializeRPC::dDMethod),
       DPointer<DeserializeRPC>(&DeserializeRPC::_DNone, &DeserializeRPC::dDNone),
       DPointer<DeserializeRPC>(&DeserializeRPC::_DStruct, &DeserializeRPC::dDStruct),
       DPointer<DeserializeRPC>(&DeserializeRPC::_DBuffer, &DeserializeRPC::dDBuffer),
    };
    return (memberPointer);
  }

  static DAttribute* ownAttributeEnd()
  {
    return (ownAttributeBegin() + ownAttributeCount());
  }

  static DPointer<DeserializeRPC>*  memberEnd()
  {
    return (memberBegin() + ownAttributeCount());
  } 
};

#endif
