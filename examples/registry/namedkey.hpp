#ifndef __namedkey_hh__
#define __namedkey_hh__

#include "registry_common.hpp"

using namespace Destruct;

class NamedKey: public DCppObject<NamedKey>
{
public:
  NamedKey(DStruct* dstruct, DValue const& args);
protected:
  ~NamedKey();
public:
  RealValue<DUInt16> signature, keyType, keyNameLength, classNameLength;
  RealValue<DInt32>  size;
  RealValue<DUInt32> parentKeyOffset, subkeyCount, subkeyCountVolatile,  subkeyListOffset, subkeyListOffsetVolatile, valueCount, valueListOffset, securityDescriptorOffset, classNameOffset, unknown1, subkeyNameMaximumLength, subkeyClassNameMaximumLength, valueNameMaximumLength, valueDataMaximumSize, unknown2, minor;
  RealValue<DUInt64> timestamp;
  RealValue<DUnicodeString>  fileName, name;

  RealValue<DObject*> subkeys, values;

  DObject*      deserializeRaw(DValue const& stream);

  attributeCount(NamedKey, 26)
  attributeList(
      attribute(DInt32, size) //hbin size
      attribute(DUInt16, signature)
      attribute(DUInt16, keyType)
      attribute(DUInt64, timestamp)
      attribute(DUInt32, unknown1)
      attribute(DUInt32, parentKeyOffset)
      attribute(DUInt32, subkeyCount)
      attribute(DUInt32, subkeyCountVolatile)
      attribute(DUInt32, subkeyListOffset)
      attribute(DUInt32, subkeyListOffsetVolatile)
      attribute(DUInt32, valueCount)
      attribute(DUInt32, valueListOffset)
      attribute(DUInt32, securityDescriptorOffset)
      attribute(DUInt32, classNameOffset)
      attribute(DUInt32, subkeyNameMaximumLength)
      attribute(DUInt32, subkeyClassNameMaximumLength)
      attribute(DUInt32, valueNameMaximumLength)
      attribute(DUInt32, valueDataMaximumSize)
      attribute(DUInt32, unknown2) //security cell index ??
      attribute(DUInt16, keyNameLength)
      attribute(DUInt16, classNameLength)
      attribute(DUnicodeString, name) //deserialized directly ?
      attribute(DObject, subkeys)
      attribute(DObject, values)
      function(DObject, deserializeRaw, DObject)
      attribute(DUInt32, minor)
      )

  memberList(NamedKey, 
      member(NamedKey, size) //??
      member(NamedKey, signature)
      member(NamedKey, keyType)
      member(NamedKey, timestamp)
      member(NamedKey, unknown1)
      member(NamedKey, parentKeyOffset)
      member(NamedKey, subkeyCount)
      member(NamedKey, subkeyCountVolatile)
      member(NamedKey, subkeyListOffset)
      member(NamedKey, subkeyListOffsetVolatile)
      member(NamedKey, valueCount)
      member(NamedKey, valueListOffset)
      member(NamedKey, securityDescriptorOffset)
      member(NamedKey, classNameOffset)
      member(NamedKey, subkeyNameMaximumLength)
      member(NamedKey, subkeyClassNameMaximumLength)
      member(NamedKey, valueNameMaximumLength)
      member(NamedKey, valueDataMaximumSize)
      member(NamedKey, unknown2)
      member(NamedKey, keyNameLength)
      member(NamedKey, classNameLength)
      member(NamedKey, name)
      member(NamedKey, subkeys)
      member(NamedKey, values)
      method(NamedKey, deserializeRaw)
      member(NamedKey, minor)
      )
private:
  RealValue<DFunctionObject*>        _deserializeRaw;
};

#endif
