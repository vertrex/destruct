#ifndef DESTRUCT_DPROTOCOL_HPP
#define DESTRUCT_DPROTOCOL_HPP

#include "dcppobject.hpp"
#include "protocol/dcontainer.hpp"
#include "protocol/diterator.hpp"
#include "protocol/dvector.hpp"
#include "protocol/dmap.hpp"
#include "protocol/dmapiterator.hpp"

//put in DType
#define DInt8T    DInt8,    DType::DInt8Type 
#define DUInt8T   DUInt8,   DType::DUInt8Type 
#define DInt16T   DInt16,   DType::DInt16Type
#define DUInt16T  DUInt16,  DType::DUInt16Type
#define DInt32T   DInt32,   DType::DInt32Type
#define DUInt32T  DUInt32,  DType::DUInt32Type
#define DInt64T   DInt64,   DType::DInt64Type
#define DUInt64T  DUInt64,  DType::DUInt64Type
#define DObjectT  DObject*, DType::DObjectType
#define DUnicodeStringT DUnicodeString, DType::DUnicodeStringType
#define DStructT  DStruct*, DType::DStructType

namespace Destruct
{
class DSstructs;

class Protocol
{
public:
   Protocol(DStructs* destruct);
private:
   void __registerStruct(DStructs* destruct);
};

template <typename CppClass>
inline 
void  registerCpp(DStructs* destruct, DUnicodeString const& name)
{
  destruct->registerDStruct(new DStruct(NULL, name, DCppObject<CppClass>::newObject, CppClass::ownAttributeBegin(), CppClass::ownAttributeEnd()));
}

template <typename VectorType, DType::Type_t VectorTypeId>
inline 
void  registerVector(DStructs* destruct, DUnicodeString const& name)
{
  registerCpp<DVector<VectorType, VectorTypeId> >(destruct, name);

  DUnicodeString iteratorName = "DIterator" + DType(VectorTypeId).name();
  registerCpp<DIterator<VectorType, VectorTypeId> >(destruct, iteratorName);
}

template <typename KeyType, DType::Type_t  KeyTypeId,
          typename ValueType, DType::Type_t  ValueTypeId>
inline 
void  registerMap(DStructs* destruct, DUnicodeString const& name)
{
  registerCpp<DMap<KeyType, KeyTypeId, ValueType, ValueTypeId > >(destruct, name);

  DUnicodeString iteratorName = "DMapIterator" + DType(KeyTypeId).name() + DType(ValueTypeId).name();
  registerCpp<DMapIterator<KeyType, KeyTypeId, ValueType, ValueTypeId > >(destruct, iteratorName);

  DUnicodeString itemName = "DMapItem" + DType(KeyTypeId).name() + DType(ValueTypeId).name();
  registerCpp<DMapItem<KeyType, KeyTypeId, ValueType, ValueTypeId > >(destruct, itemName);
}

}
#endif
