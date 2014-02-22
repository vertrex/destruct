#include "destruct.hpp"
#include "dstruct.hpp"

#include "protocol/dclassobject.hpp"
#include "protocol/dmutablestruct.hpp"
#include "protocol/dmutableobject.hpp"

#include "protocol/protocol.hpp"
#include "protocol/diterator.hpp"
#include "protocol/dvector.hpp"
#include "protocol/dmap.hpp"
#include "protocol/dmapiterator.hpp"

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
  destruct->registerDStruct(static_cast<DStruct*>(new DMutableStruct(NULL,"DMutable", DMutableObject::newObject)));
  destruct->registerDStruct(makeNewDMutable<DIterator >("DIterator"));

  destruct->registerDStruct(makeNewDCpp<DVectorInt32 >("DVector<Int32>")); //DInt32 please  //CppStruct ?
  destruct->registerDStruct(makeNewDCpp<DVectorInt64 >("DVector<DInt64>")); //virer le <>
  destruct->registerDStruct(makeNewDCpp<DVectorString >("DVector<String>"));
  destruct->registerDStruct(makeNewDCpp<DVectorObject >("DVector<DObject*>")); //virer le <> et * ! 

  destruct->registerDStruct(makeNewDCpp<DMap<DUnicodeString, DType::DUnicodeStringType,  DUnicodeString, DType::DUnicodeStringType > >("DMapString"));
  //destruct->registerDStruct(makeNewDMutable<DMapIterator >("DMapIterator"));
}

}
