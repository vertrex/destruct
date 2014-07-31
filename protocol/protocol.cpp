#include "destruct.hpp"
#include "dstruct.hpp"
#include "dnullobject.hpp"

#include "protocol/dclassobject.hpp"
#include "protocol/dmutablestruct.hpp"
#include "protocol/dmutableobject.hpp"

#include "protocol/protocol.hpp"
#include "protocol/diterator.hpp"
#include "protocol/dvector.hpp"
#include "protocol/dmap.hpp"
#include "protocol/dmapiterator.hpp"
#include "protocol/dstream.hpp"

namespace Destruct
{

typedef DVector<DUnicodeString, DType::DUnicodeStringType> DVectorString;
typedef DVector<DInt32,   DType::DInt32Type >                DVectorInt32;
typedef DVector<DInt64,   DType::DInt64Type >                DVectorInt64;
typedef DVector<DObject*, DType::DObjectType>                DVectorObject;

Protocol::Protocol(Destruct* destruct)
{
  this->__registerStruct(destruct);
}

void Protocol::__registerStruct(Destruct* destruct)
{
  destruct->registerDStruct(static_cast<DStruct*>(new DMutableStruct(NULL, "DMutable", DMutableObject::newObject)));
  destruct->registerDStruct(makeNewDMutable<DIterator >("DIterator"));

  destruct->registerDStruct(makeNewDCpp<DVectorInt32 >("DVectorInt32"));
  destruct->registerDStruct(makeNewDCpp<DVectorInt64 >("DVectorInt64"));
  destruct->registerDStruct(makeNewDCpp<DVectorString >("DVectorString"));
  destruct->registerDStruct(makeNewDCpp<DVectorObject >("DVectorObject"));

  destruct->registerDStruct(makeNewDCpp<DMap<DUnicodeString, DType::DUnicodeStringType,  DUnicodeString, DType::DUnicodeStringType > >("DMapString"));
  destruct->registerDStruct(makeNewDCpp<DMap<DObject* , DType::DObjectType,  DObject* , DType::DObjectType> >("DMapObject"));
  //destruct->registerDStruct(makeNewDMutable<DMapIterator >("DMapIterator"));
  
  destruct->registerDStruct(makeNewDCpp<DStream >("DStream"));
  destruct->registerDStruct(makeNewDCpp<DStreamCout >("DStreamCout"));
  destruct->registerDStruct(makeNewDCpp<DStreamString >("DStreamString"));

  //destruct->registerDStruct(makeNewDCpp<DSerializeXML>("DSerializeBinary"));
}

}
