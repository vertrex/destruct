#ifndef __namedkey_hh__
#define __namedkey_hh__

#include "registry_common.hpp"

using namespace Destruct;

class NamedKey: public DCppObject<NamedKey>
{
public:
  NamedKey(DStruct* dstruct, DValue const& args);
  ~NamedKey();

  RealValue<DUInt16> signature, keyType, keyNameLength, classNameLength;
  RealValue<DInt32>  size;
  RealValue<DUInt32> parentKeyOffset, subkeyCount, subkeyCountVolatile,  subkeyListOffset, subkeyListOffsetVolatile, valueCount, valueListOffset,
  securityDescriptorOffset, classNameOffset, unknown1, subkeyNameMaximumLength, subkeyClassNameMaximumLength, valueNameMaximumLength, valueDataMaximumSize, unknown2;
  RealValue<DObject*> keyName, subkeys, values, timestamp;

  RealValue<DUnicodeString>  fileName;


  attributeCount(NamedKey, 24)
  attributeList(
      attribute(DInt32, size) //hbin size
      attribute(DUInt16, signature)
      attribute(DUInt16, keyType)
      attribute(DObject, timestamp)
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
      attribute(DObject, keyName)
      attribute(DObject, subkeys)
      attribute(DObject, values)
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
      member(NamedKey, keyName)
      member(NamedKey, subkeys)
      member(NamedKey, values)
      //method(NamedKey, deserializeRaw)
      )
private:
  RealValue<DFunctionObject*>        _deserializeRaw;
};

class NameLength : public DCppObject<NameLength>
{
public:
          NameLength(DStruct* dstruct, DValue const& args);
          ~NameLength();
  DValue  deserializeRaw(DValue const& stream);

  RealValue<DObject*>        parent;
  RealValue<DUnicodeString>  keyName;
  RealValue<DUnicodeString>  attributeKeyName;

  attributeCount(NameLength, 3)
  attributeList(attribute(DUnicodeString, keyName)
                attribute(DUnicodeString, attributeKeyName)
                function(DUInt8, deserializeRaw, DObject)
                //attribute(DObject, parent) //recurse
               )
  memberList(NameLength, 
             member(NameLength, keyName)
             member(NameLength, attributeKeyName)
             method(NameLength, deserializeRaw)
            )
private:
  uint64_t                           __size;
  RealValue<DFunctionObject*>        _deserializeRaw;
};

#endif
