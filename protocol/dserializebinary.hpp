#ifndef DESTRUCT_DSERIALIZEBINARY_HPP_
#define DESTRUCT_DSERIALIZEBINARY_HPP_

#include <fstream>
#include <iostream> 
#include <vector>

#include "destruct.hpp"
#include "dstream.hpp"
#include "protocol/dcppobject.hpp"

namespace Destruct
{

class SerializeBinary : public DCppObject<SerializeBinary> 
{
public:
  SerializeBinary(DStruct* dstruct, DValue const& args);
  SerializeBinary(SerializeBinary const& copy);

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
  ~SerializeBinary();

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

  static DPointer<SerializeBinary>* memberBegin()
  {
    static DPointer<SerializeBinary> memberPointer[] = 
    {
       DPointer<SerializeBinary>(&SerializeBinary::__stream),
        
       DPointer<SerializeBinary>(&SerializeBinary::_DInt8, &SerializeBinary::sDInt8),
       DPointer<SerializeBinary>(&SerializeBinary::_DInt16, &SerializeBinary::sDInt16),
       DPointer<SerializeBinary>(&SerializeBinary::_DInt32, &SerializeBinary::sDInt32),
       DPointer<SerializeBinary>(&SerializeBinary::_DInt64, &SerializeBinary::sDInt64),

       DPointer<SerializeBinary>(&SerializeBinary::_DUInt8, &SerializeBinary::sDUInt8),
       DPointer<SerializeBinary>(&SerializeBinary::_DUInt16, &SerializeBinary::sDUInt16),
       DPointer<SerializeBinary>(&SerializeBinary::_DUInt32, &SerializeBinary::sDUInt32),
       DPointer<SerializeBinary>(&SerializeBinary::_DUInt64, &SerializeBinary::sDUInt64),

       DPointer<SerializeBinary>(&SerializeBinary::_DUnicodeString, &SerializeBinary::sDUnicodeString),

       DPointer<SerializeBinary>(&SerializeBinary::_DObject, &SerializeBinary::sDObject),
       DPointer<SerializeBinary>(&SerializeBinary::_DMethod, &SerializeBinary::sDMethod),
       DPointer<SerializeBinary>(&SerializeBinary::_DNone, &SerializeBinary::sDNone),
       DPointer<SerializeBinary>(&SerializeBinary::_DStruct, &SerializeBinary::sDStruct),
       DPointer<SerializeBinary>(&SerializeBinary::_DBuffer, &SerializeBinary::sDBuffer),
    };
    return (memberPointer);
  }

  static DAttribute* ownAttributeEnd()
  {
    return (ownAttributeBegin() + ownAttributeCount());
  }

  static DPointer<SerializeBinary>*  memberEnd()
  {
    return (memberBegin() + ownAttributeCount());
  } 
};

class DeserializeBinary : public DCppObject<DeserializeBinary> 
{
public:
  DeserializeBinary(DStruct* dstruct, DValue const& args);
  DeserializeBinary(DeserializeBinary const& copy);

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
  ~DeserializeBinary();

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

  static DPointer<DeserializeBinary>* memberBegin()
  {
    static DPointer<DeserializeBinary> memberPointer[] = 
    {
       DPointer<DeserializeBinary>(&DeserializeBinary::__stream),
        
       DPointer<DeserializeBinary>(&DeserializeBinary::_DInt8, &DeserializeBinary::dDInt8),
       DPointer<DeserializeBinary>(&DeserializeBinary::_DInt16, &DeserializeBinary::dDInt16),
       DPointer<DeserializeBinary>(&DeserializeBinary::_DInt32, &DeserializeBinary::dDInt32),
       DPointer<DeserializeBinary>(&DeserializeBinary::_DInt64, &DeserializeBinary::dDInt64),

       DPointer<DeserializeBinary>(&DeserializeBinary::_DUInt8, &DeserializeBinary::dDUInt8),
       DPointer<DeserializeBinary>(&DeserializeBinary::_DUInt16, &DeserializeBinary::dDUInt16),
       DPointer<DeserializeBinary>(&DeserializeBinary::_DUInt32, &DeserializeBinary::dDUInt32),
       DPointer<DeserializeBinary>(&DeserializeBinary::_DUInt64, &DeserializeBinary::dDUInt64),

       DPointer<DeserializeBinary>(&DeserializeBinary::_DUnicodeString, &DeserializeBinary::dDUnicodeString),

       DPointer<DeserializeBinary>(&DeserializeBinary::_DObject, &DeserializeBinary::dDObject),
       DPointer<DeserializeBinary>(&DeserializeBinary::_DMethod, &DeserializeBinary::dDMethod),
       DPointer<DeserializeBinary>(&DeserializeBinary::_DNone, &DeserializeBinary::dDNone),
       DPointer<DeserializeBinary>(&DeserializeBinary::_DStruct, &DeserializeBinary::dDStruct),
       DPointer<DeserializeBinary>(&DeserializeBinary::_DBuffer, &DeserializeBinary::dDBuffer),
    };
    return (memberPointer);
  }

  static DAttribute* ownAttributeEnd()
  {
    return (ownAttributeBegin() + ownAttributeCount());
  }

  static DPointer<DeserializeBinary>*  memberEnd()
  {
    return (memberBegin() + ownAttributeCount());
  } 
};

}

#endif
