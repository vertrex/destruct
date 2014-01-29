#include "../destruct.hpp"
#include "../dstruct.hpp"
#include "../dcppobject.hpp"

#include "protocol.hpp"
#include "diterator.hpp"
#include "dcontainer.hpp"

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
  DStruct* s =  makeNewDClass< DIterator<DUnicodeString, DType::DUnicodeStringType> >(NULL, "DIteratorString"); //DIterator<String>
  destruct->registerDStruct(s);
  s = makeNewDClass< DIterator<DInt32, DType::DInt32Type> >(NULL, "DIteratorInt32"); //DIterator<String>
  destruct->registerDStruct(s);
  s = makeNewDClass< DIterator<DInt64, DType::DInt64Type> >(NULL, "DIteratorInt64"); //DIterator<String>
  destruct->registerDStruct(s);
  DStruct* dvectorString = makeNewDClass< DVectorString  >(NULL, "DVector<String>");
  destruct->registerDStruct(dvectorString);

  DStruct* dvectorInt32 = makeNewDClass< DVectorInt32 >(NULL, "DVector<Int32>");
  destruct->registerDStruct(dvectorInt32);

  DStruct* dvectorInt64 = makeNewDClass< DVectorInt64 >(NULL, "DVector<DInt64>");
  destruct->registerDStruct(dvectorInt64);


  DStruct* dvectorObject = makeNewDClass< DVectorObject  >(NULL, "DVector<DObject*>");
  destruct->registerDStruct(dvectorObject);
}

}
