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
typedef DVector<DInt32, DType::DInt32Type > DVectorObject;


Protocol::Protocol(Destruct* destruct)
{
  this->__registerStruct(destruct);
}

void Protocol::__registerStruct(Destruct* destruct)
{
  DStruct* diterator = makeNewDClass< DIterator >(NULL, "DIterator");
  destruct->registerDStruct(diterator);

  DStruct* dvectorString = makeNewDClass< DVectorString  >(NULL, "DVector<String>");
  destruct->registerDStruct(dvectorString);

  DStruct* dvectorInt32 = makeNewDClass< DVectorInt32 >(NULL, "DVector<Int32>");
  destruct->registerDStruct(dvectorInt32);

  DStruct* dvectorObject = makeNewDClass< DVectorObject  >(NULL, "DVector<DObject*>");
  destruct->registerDStruct(dvectorObject);
}

}
