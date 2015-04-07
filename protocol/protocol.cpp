#include "dstructs.hpp"
#include "dstruct.hpp"
#include "dnullobject.hpp"

#include "protocol/dclassobject.hpp"
#include "protocol/dmutablestruct.hpp"
#include "protocol/dmutableobject.hpp"

#include "protocol/protocol.hpp"

#include "protocol/dserializebinary.hpp"
#include "protocol/dserializetext.hpp"
#include "protocol/dserializexml.hpp"
#include "protocol/dstream.hpp"
#include "protocol/traceobject.hpp"
#include "protocol/recursivetraceobject.hpp"

namespace Destruct
{
Protocol::Protocol(DStructs* destruct)
{
  this->__registerStruct(destruct);
}

void Protocol::__registerStruct(DStructs* destruct)
{
  registerVector<DInt8T>(destruct, "DVectorInt8");
  registerVector<DUInt8T>(destruct, "DVectorUInt8");
  registerVector<DInt16T>(destruct, "DVectorInt16");
  registerVector<DUInt16T>(destruct, "DVectorUInt16");
  registerVector<DInt32T>(destruct, "DVectorInt32");
  registerVector<DUInt32T>(destruct, "DVectorUInt32");
  registerVector<DInt64T>(destruct, "DVectorInt64");
  registerVector<DUInt64T>(destruct, "DVectorUInt64");
  registerVector<DUnicodeStringT>(destruct, "DVectorString");
  registerVector<DObjectT>(destruct, "DVectorObject");

  registerVector<DStructT>(destruct, "DVectorStruct");

  registerMap<DUnicodeStringT, DUnicodeStringT>(destruct, "DMapString");
  registerMap<DObjectT, DObjectT>(destruct, "DMapObject");
  registerMap<DUInt64T, DObjectT>(destruct, "DMapUInt64Object");

  registerCpp<DStream >(destruct, "DStream");

  registerCpp<SerializeBinary>(destruct, "SerializeBinary");
  registerCpp<DeserializeBinary>(destruct, "DeserializeBinary");
  registerCpp<SerializeText>(destruct, "SerializeText");
  registerCpp<SerializeXML>(destruct, "SerializeXML");

  //registerCpp<DStreamCout >(destruct, "DStreamCout");
  //registerCpp<DStreamString >(destruct, "DStreamString");
  //registerCpp<DSerializeXML>(destruct, "DSerializeXML");

  destruct->registerDStruct(new DStruct(NULL, "TraceObject",  TraceObject::newObject));
  destruct->registerDStruct(new DStruct(NULL, "RecursiveTraceObject",  RecursiveTraceObject::newObject));
  destruct->registerDStruct(static_cast<DStruct*>(new DMutableStruct(NULL, "DMutable", DMutableObject::newObject)));
}

}
