#include "destruct.hpp"
#include "dstruct.hpp"

#include "protocol/dclassobject.hpp"
#include "protocol/dmutablestruct.hpp"

#include "protocol/protocol.hpp"
#include "protocol/diterator.hpp"
#include "protocol/dcontainer.hpp"

namespace Destruct
{

typedef DVector<DUnicodeString, DType::DUnicodeStringType> DVectorString;
typedef DVector<DInt32, DType::DInt32Type > DVectorInt32;
typedef DVector<DInt64, DType::DInt64Type > DVectorInt64;
typedef DVector<DInt32, DType::DInt32Type > DVectorObject;


Protocol::Protocol(Destruct* destruct)
{
  this->__registerStruct(destruct);
}

void Protocol::__registerStruct(Destruct* destruct)
{
  destruct->registerDStruct((DStruct*)new DMutableStruct(NULL, "DIterator",(DObject* (*)(DMutableStruct*)) DIterator::newObject, DIterator::ownAttributeBegin(), DIterator::ownAttributeEnd()));
 

  destruct->registerDStruct(makeNewDClass< DVectorString >(NULL, "DVector<String>"));
  destruct->registerDStruct(makeNewDClass< DVectorInt32  >(NULL, "DVector<Int32>"));
  destruct->registerDStruct(makeNewDClass< DVectorInt64  >(NULL, "DVector<DInt64>"));
  destruct->registerDStruct(makeNewDClass< DVectorObject  >(NULL, "DVector<DObject*>"));
}

}
