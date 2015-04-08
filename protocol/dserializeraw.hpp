#ifndef DESTRUCT_DSERIALIZERAW_HPP_
#define DESTRUCT_DSERIALIZERAW_HPP_

#include "protocol/dcppobject.hpp"

namespace Destruct
{
/*
 *  This desrialize and deserialize as binary 'raw' 'flat' type without markers. 
 *  That can be used to read file produce by other tools.
 */
class SerializeRaw : public DCppObject<SerializeRaw> 
{
public:
  SerializeRaw(DStruct* dstruct, DValue const& args);
  SerializeRaw(SerializeRaw const& copy);

  DObject*   stream(void);

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

protected:
  ~SerializeRaw();

public:
  RealValue<DFunctionObject* > _DObject, _DStruct, _DUnicodeString, _DNone, _DMethod, _DBuffer, 
                               _DInt8, _DInt16, _DInt32, _DInt64,
                               _DUInt8, _DUInt16, _DUInt32, _DUInt64;

  RealValue<DObject*>         __stream;
  DUInt32                     __depth;

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

  static DPointer<SerializeRaw>* memberBegin()
  {
    static DPointer<SerializeRaw> memberPointer[] = 
    {
       DPointer<SerializeRaw>(&SerializeRaw::__stream),
        
       DPointer<SerializeRaw>(&SerializeRaw::_DInt8, &SerializeRaw::sDInt8),
       DPointer<SerializeRaw>(&SerializeRaw::_DInt16, &SerializeRaw::sDInt16),
       DPointer<SerializeRaw>(&SerializeRaw::_DInt32, &SerializeRaw::sDInt32),
       DPointer<SerializeRaw>(&SerializeRaw::_DInt64, &SerializeRaw::sDInt64),

       DPointer<SerializeRaw>(&SerializeRaw::_DUInt8, &SerializeRaw::sDUInt8),
       DPointer<SerializeRaw>(&SerializeRaw::_DUInt16, &SerializeRaw::sDUInt16),
       DPointer<SerializeRaw>(&SerializeRaw::_DUInt32, &SerializeRaw::sDUInt32),
       DPointer<SerializeRaw>(&SerializeRaw::_DUInt64, &SerializeRaw::sDUInt64),

       DPointer<SerializeRaw>(&SerializeRaw::_DUnicodeString, &SerializeRaw::sDUnicodeString),

       DPointer<SerializeRaw>(&SerializeRaw::_DObject, &SerializeRaw::sDObject),
       DPointer<SerializeRaw>(&SerializeRaw::_DMethod, &SerializeRaw::sDMethod),
       DPointer<SerializeRaw>(&SerializeRaw::_DNone, &SerializeRaw::sDNone),
       DPointer<SerializeRaw>(&SerializeRaw::_DStruct, &SerializeRaw::sDStruct),
       DPointer<SerializeRaw>(&SerializeRaw::_DBuffer, &SerializeRaw::sDBuffer),
    };
    return (memberPointer);
  }

  static DAttribute* ownAttributeEnd()
  {
    return (ownAttributeBegin() + ownAttributeCount());
  }

  static DPointer<SerializeRaw>*  memberEnd()
  {
    return (memberBegin() + ownAttributeCount());
  } 
};

class DeserializeRaw : public DCppObject<DeserializeRaw> 
{
public:
  DeserializeRaw(DStruct* dstruct, DValue const& args);
  DeserializeRaw(DeserializeRaw const& copy);

  DObject*           stream(void);

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
  ~DeserializeRaw();

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

  static DPointer<DeserializeRaw>* memberBegin()
  {
    static DPointer<DeserializeRaw> memberPointer[] = 
    {
       DPointer<DeserializeRaw>(&DeserializeRaw::__stream),
        
       DPointer<DeserializeRaw>(&DeserializeRaw::_DInt8, &DeserializeRaw::dDInt8),
       DPointer<DeserializeRaw>(&DeserializeRaw::_DInt16, &DeserializeRaw::dDInt16),
       DPointer<DeserializeRaw>(&DeserializeRaw::_DInt32, &DeserializeRaw::dDInt32),
       DPointer<DeserializeRaw>(&DeserializeRaw::_DInt64, &DeserializeRaw::dDInt64),

       DPointer<DeserializeRaw>(&DeserializeRaw::_DUInt8, &DeserializeRaw::dDUInt8),
       DPointer<DeserializeRaw>(&DeserializeRaw::_DUInt16, &DeserializeRaw::dDUInt16),
       DPointer<DeserializeRaw>(&DeserializeRaw::_DUInt32, &DeserializeRaw::dDUInt32),
       DPointer<DeserializeRaw>(&DeserializeRaw::_DUInt64, &DeserializeRaw::dDUInt64),

       DPointer<DeserializeRaw>(&DeserializeRaw::_DUnicodeString, &DeserializeRaw::dDUnicodeString),

       DPointer<DeserializeRaw>(&DeserializeRaw::_DObject, &DeserializeRaw::dDObject),
       DPointer<DeserializeRaw>(&DeserializeRaw::_DMethod, &DeserializeRaw::dDMethod),
       DPointer<DeserializeRaw>(&DeserializeRaw::_DNone, &DeserializeRaw::dDNone),
       DPointer<DeserializeRaw>(&DeserializeRaw::_DStruct, &DeserializeRaw::dDStruct),
       DPointer<DeserializeRaw>(&DeserializeRaw::_DBuffer, &DeserializeRaw::dDBuffer),
    };
    return (memberPointer);
  }

  static DAttribute* ownAttributeEnd()
  {
    return (ownAttributeBegin() + ownAttributeCount());
  }

  static DPointer<DeserializeRaw>*  memberEnd()
  {
    return (memberBegin() + ownAttributeCount());
  } 
};

}

#endif
