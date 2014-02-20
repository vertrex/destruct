#include "destruct.hpp"
#include "dstruct.hpp"

#include "protocol/dclassobject.hpp"
#include "protocol/dmutablestruct.hpp"

#include "protocol/protocol.hpp"
#include "protocol/diterator.hpp"
#include "protocol/dvector.hpp"

namespace Destruct
{

typedef DVector<DUnicodeString, DType::DUnicodeStringType> DVectorString;
typedef DVector<DInt32, DType::DInt32Type >                DVectorInt32;
typedef DVector<DInt64, DType::DInt64Type >                DVectorInt64;
typedef DVector<DInt32, DType::DInt32Type >                DVectorObject;

Protocol::Protocol(Destruct* destruct)
{
  this->__registerStruct(destruct);
}

void Protocol::__registerStruct(Destruct* destruct)
{
  destruct->registerDStruct(makeNewDMutable<DIterator >("DIterator"));
  destruct->registerDStruct(makeNewDClass<DVectorInt32 >("DVector<Int32>"));
  destruct->registerDStruct(makeNewDClass<DVectorInt64 >("DVector<DInt64>"));
  destruct->registerDStruct(makeNewDClass<DVectorString >("DVector<String>"));
  destruct->registerDStruct(makeNewDClass<DVectorObject >("DVector<DObject*>"));
}

}
