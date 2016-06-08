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
#include "protocol/dserializeraw.hpp"
#include "protocol/dstream.hpp"
#include "protocol/dstreamcout.hpp"
#include "protocol/dstreamstring.hpp"
#include "protocol/traceobject.hpp"
#include "protocol/recursivetraceobject.hpp"

#include "protocol/import.hpp"

namespace Destruct
{
Protocol::Protocol(DStructs* destruct)
{
  this->__registerStruct(destruct);
}

void Protocol::__registerStruct(DStructs* destruct)
{
  registerVector<DInt8T>(destruct, "DVectorInt8"); //Use namespace Vector.Int8
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

  registerMap<DUnicodeStringT, DUnicodeStringT>(destruct, "DMapString"); //U&se namesape Map.String
  registerMap<DObjectT, DObjectT>(destruct, "DMapObject");
  registerMap<DUInt64T, DObjectT>(destruct, "DMapUInt64Object");

  registerCpp<DStream>(destruct, "DStream");  //Use namespace Stream.File
  registerCpp<DStreamCout>(destruct, "DStreamCout");  //Use namespace Stream.Cout
  registerCpp<DStreamString>(destruct, "DStreamString"); //Use namespace Stream.String / Buffer

  registerCpp<SerializeBinary>(destruct, "SerializeBinary"); //Use namespace Serialize.Binary
  registerCpp<DeserializeBinary>(destruct, "DeserializeBinary"); //Use namespace Deserialize.Binary
  registerCpp<SerializeText>(destruct, "SerializeText");
  registerCpp<SerializeXML>(destruct, "SerializeXML");
  registerCpp<SerializeRaw>(destruct, "SerializeRaw");
  registerCpp<DeserializeRaw>(destruct, "DeserializeRaw");

  destruct->registerDStruct(new DStruct(NULL, "TraceObject",  TraceObject::newObject)); //use name space Debug.Trace
  destruct->registerDStruct(new DStruct(NULL, "RecursiveTraceObject",  RecursiveTraceObject::newObject));
  destruct->registerDStruct(static_cast<DStruct*>(new DMutableStruct(NULL, "DMutable", DMutableObject::newObject)));

  Destruct::DStruct* import = new DStruct(NULL, "Import", Import::newObject, Import::ownAttributeBegin(), Import::ownAttributeEnd());//Import is a singleton
  destruct->registerDStruct(import);
}

}
