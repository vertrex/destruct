#ifndef __DSERIALIZERPC__
#define __DSERIALIZERPC__

#include "networkstream.hpp"
#include "objectmanager.hpp"
#include "serverfunctionobject.hpp"

#include "protocol/dcppobject.hpp"

namespace Destruct
{

class SerializeRPC : public DCppObject<SerializeRPC> 
{

public:
  SerializeRPC(DStruct* dstruct, DValue const& args);
  SerializeRPC(SerializeRPC const& copy);

  void       sDInt8(DValue const& args);
  void       sDInt16(DValue const& args);
  void       sDInt32(DValue const& args);
  void       sDInt64(DValue const& args);

  void       sDUInt8(DValue const& args);
  void       sDUInt16(DValue const& args);
  void       sDUInt32(DValue const& args);
  void       sDUInt64(DValue const& args);

  void       sDUnicodeString(DValue const& args);

  void       sDObject(DValue const& args);
  void       sDMethod(DValue const& args);
  void       sDNone(void);
  void       sDStruct(DValue const& args);
  void       sDBuffer(DValue const& args);

  DObject*   __objectManager; //RV
protected:
  ~SerializeRPC();

public:
  RealValue<DFunctionObject* > _DObject, _DStruct, _DUnicodeString, _DNone, _DMethod, _DBuffer, 
                               _DInt8, _DInt16, _DInt32, _DInt64,
                               _DUInt8, _DUInt16, _DUInt32, _DUInt64;

  RealValue<DObject*>          __stream;

  static size_t ownAttributeCount()
  {
    return (15);
  }

  static DAttribute* ownAttributeBegin()
  {
    static DAttribute  attributes[] = 
    {
       DAttribute(DType::DObjectType, "stream"),

       DAttribute(DType::DNoneType,"DInt8", DType::DInt8Type), 
       DAttribute(DType::DNoneType,"DInt16", DType::DInt16Type), 
       DAttribute(DType::DNoneType,"DInt32", DType::DInt32Type), 
       DAttribute(DType::DNoneType,"DInt64", DType::DInt64Type), 

       DAttribute(DType::DNoneType,"DUInt8", DType::DUInt8Type), 
       DAttribute(DType::DNoneType,"DUInt16", DType::DUInt16Type), 
       DAttribute(DType::DNoneType,"DUInt32", DType::DUInt32Type), 
       DAttribute(DType::DNoneType,"DUInt64", DType::DUInt64Type), 
       
       DAttribute(DType::DNoneType, "DUnicodeString", DType::DUnicodeStringType),

       DAttribute(DType::DNoneType, "DObject", DType::DObjectType), 
       DAttribute(DType::DNoneType, "DMethod", DType::DMethodType),
       DAttribute(DType::DNoneType, "DNone", DType::DNoneType),
       DAttribute(DType::DNoneType, "DStruct", DType::DStructType), 
       DAttribute(DType::DNoneType, "DBuffer", DType::DBufferType), 

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

  DInt8              dDInt8(void);
  DInt16             dDInt16(void);
  DInt32             dDInt32(void);
  DInt64             dDInt64(void);

  DUInt8             dDUInt8(void);
  DUInt16            dDUInt16(void);
  DUInt32            dDUInt32(void);
  DUInt64            dDUInt64(void);

  DUnicodeString     dDUnicodeString(void);

  DObject*           dDObject(DValue const& args);
  DFunctionObject*   dDMethod(void);
  DObject*           dDNone(void);
  DStruct*           dDStruct(void);
  DBuffer            dDBuffer(void);

protected:
  ~DeserializeRPC();

public:
  RealValue<DFunctionObject* > _DObject, _DStruct, _DUnicodeString, _DNone, _DMethod, _DBuffer, 
                               _DInt8, _DInt16, _DInt32, _DInt64,
                               _DUInt8, _DUInt16, _DUInt32, _DUInt64;

  RealValue<DObject*>          __stream;

  static size_t ownAttributeCount()
  {
    return (15);
  }

  static DAttribute* ownAttributeBegin()
  {
    static DAttribute  attributes[] = 
    {
       DAttribute(DType::DObjectType, "stream"),

       DAttribute(DType::DInt8Type, "DInt8", DType::DNoneType), 
       DAttribute(DType::DInt16Type,"DInt16", DType::DNoneType), 
       DAttribute(DType::DInt32Type,"DInt32", DType::DNoneType), 
       DAttribute(DType::DInt64Type,"DInt64", DType::DNoneType), 

       DAttribute(DType::DUInt8Type, "DUInt8", DType::DNoneType), 
       DAttribute(DType::DUInt16Type,"DUInt16", DType::DNoneType), 
       DAttribute(DType::DUInt32Type,"DUInt32", DType::DNoneType), 
       DAttribute(DType::DUInt64Type,"DUInt64", DType::DNoneType), 
       
       DAttribute(DType::DUnicodeStringType, "DUnicodeString", DType::DNoneType),

       DAttribute(DType::DObjectType, "DObject", DType::DObjectType), 
       DAttribute(DType::DMethodType, "DMethod", DType::DNoneType),
       DAttribute(DType::DNoneType,   "DNone", DType::DNoneType),
       DAttribute(DType::DStructType, "DStruct", DType::DNoneType), 
       DAttribute(DType::DBufferType, "DBuffer", DType::DNoneType), 
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

}

#endif
