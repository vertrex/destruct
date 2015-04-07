#ifndef DESTRUCT_DSERIALIZETEXT_HPP_
#define DESTRUCT_DSERIALIZETEXT_HPP_

#include "dcppobject.hpp"

namespace Destruct
{

class SerializeText : public DCppObject<SerializeText> 
{
public:
  SerializeText(DStruct* dstruct, DValue const& args);
  SerializeText(SerializeText const& copy);

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
  ~SerializeText();

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

  static DPointer<SerializeText>* memberBegin()
  {
    static DPointer<SerializeText> memberPointer[] = 
    {
       DPointer<SerializeText>(&SerializeText::__stream),
        
       DPointer<SerializeText>(&SerializeText::_DInt8, &SerializeText::sDInt8),
       DPointer<SerializeText>(&SerializeText::_DInt16, &SerializeText::sDInt16),
       DPointer<SerializeText>(&SerializeText::_DInt32, &SerializeText::sDInt32),
       DPointer<SerializeText>(&SerializeText::_DInt64, &SerializeText::sDInt64),

       DPointer<SerializeText>(&SerializeText::_DUInt8, &SerializeText::sDUInt8),
       DPointer<SerializeText>(&SerializeText::_DUInt16, &SerializeText::sDUInt16),
       DPointer<SerializeText>(&SerializeText::_DUInt32, &SerializeText::sDUInt32),
       DPointer<SerializeText>(&SerializeText::_DUInt64, &SerializeText::sDUInt64),

       DPointer<SerializeText>(&SerializeText::_DUnicodeString, &SerializeText::sDUnicodeString),

       DPointer<SerializeText>(&SerializeText::_DObject, &SerializeText::sDObject),
       DPointer<SerializeText>(&SerializeText::_DMethod, &SerializeText::sDMethod),
       DPointer<SerializeText>(&SerializeText::_DNone, &SerializeText::sDNone),
       DPointer<SerializeText>(&SerializeText::_DStruct, &SerializeText::sDStruct),
       DPointer<SerializeText>(&SerializeText::_DBuffer, &SerializeText::sDBuffer),
    };
    return (memberPointer);
  }

  static DAttribute* ownAttributeEnd()
  {
    return (ownAttributeBegin() + ownAttributeCount());
  }

  static DPointer<SerializeText>*  memberEnd()
  {
    return (memberBegin() + ownAttributeCount());
  } 
};

}

#endif
