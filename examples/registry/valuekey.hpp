#ifndef __valuekey_hh__
#define __valuekey_hh__

#include "registry_common.hpp"

using namespace Destruct;

class ValueKey : public DCppObject<ValueKey>
{
public:
  ValueKey(DStruct* dstruct, DValue const& args);

  DObject*              deserializeRaw(DValue const& stream);
  DUnicodeString        valueTypeName(void);
  DObject*              data(void);
 
  DUInt64  realDataOffset;


  RealValue<DUInt16>           signature, nameLength, unknown1, valueType;//named, unkown2
  RealValue<DInt32>            size, realDataSize;
  RealValue<DUInt32>           dataOffset, dataLength, dataType;
  RealValue<DUnicodeString>    name;
  RealValue<DObject*>          _deserializer, dataOffsets;
  RealValue<DFunctionObject*>  _valueTypeName, _data;
  attributeCount(ValueKey, 14)
  attributeList(
                attribute(DInt32, size)
                attribute(DUInt16, signature)
                attribute(DUInt16, nameLength)
                attribute(DUInt32, dataLength)
                attribute(DUInt32, dataOffset)
                attribute(DUInt32, dataType)
                attribute(DUInt16, valueType)
                attribute(DUInt16, unknown1)
                attribute(DUnicodeString, name)
                function(DObject, data, DNone)
                function(DUnicodeString, valueTypeName, DNone)
                function(DObject, deserializeRaw, DObject)
                attribute(DObject, dataOffsets)
                attribute(DInt32, realDataSize)
               )

  memberList(ValueKey, 
             member(ValueKey, size)
             member(ValueKey, signature)
             member(ValueKey, nameLength)
             member(ValueKey, dataLength)
             member(ValueKey, dataOffset)
             member(ValueKey, dataType)
             member(ValueKey, valueType)
             member(ValueKey, unknown1)
             member(ValueKey, name)
             method(ValueKey, data) 
             method(ValueKey, valueTypeName)
             method(ValueKey, deserializeRaw)
             member(ValueKey, dataOffsets) 
             member(ValueKey, realDataSize)
            )

protected:
  ~ValueKey();
private:
  uint64_t                           __size;
  RealValue<DFunctionObject*>        _deserializeRaw;

  static std::string registryType(uint32_t dataType)
  {
    static std::string registryType[] = { 
                                          "REG_NONE",
                                          "REG_SZ",
                                          "REG_EXPAND_SZ",
                                          "REG_BINARY",
                                          "REG_DWORD",
                                          "REG_DWORD_BIG_ENDIAN",
                                          "REG_LINK",
                                          "REG_MULTI_SZ",
                                          "REG_RESOURCE_LIST",
                                          "REG_FULL_RESOURCE_DESCRIPTON",
                                          "REG_RESOURCE_REQUIEREMENTS_LIST",
                                          "REG_QWORD",
                                        };
    if (dataType < 12) 
      return registryType[dataType]; 
    return ("REG_BINARY");
  }

  static std::string registryTypeStructName(uint32_t dataType)
  {
    static std::string registryTypeStructName[] = { 
                                          "RegistryDataNone",
                                          "RegistryDataSZ",
                                          "RegistryDataSZ",
                                          "RegistryDataBinary",
                                          "RegistryDataDWord",
                                          "RegistryDataDwordBE",
                                          "RegistryDataSZ",
                                          "RegistryDataMultiSZ",
                                          "RegistryDataBinary",
                                          "RegistryDataBinary",
                                          "RegistryDataBinary",
                                          "RegistryQWord",
                                        };
    if (dataType < 12) 
      return registryTypeStructName[dataType]; 
    return ("RegistryDataBinary"); 
  }
};

class RegistryBigData : public DCppObject<RegistryBigData>
{
public:
  RegistryBigData(DStruct* dstruct, DValue const& args);

  DObject* deserializeRaw(DValue const& args);

  RealValue<DObject*>           offsets;
  RealValue<DFunctionObject*>     _deserializeRaw;

  attributeCount(RegistryBigData, 2)
  attributeList(
                attribute(DObject, offsets)
                function(DObject, deserializeRaw, DObject)
               )
  memberList(RegistryBigData,
                member(RegistryBigData, offsets)
                method(RegistryBigData, deserializeRaw)
            )
protected:
  ~RegistryBigData();
};

class RegistryData
{
public:
  RegistryData();
  DBuffer   read(DValue const& parent);
protected:
  ~RegistryData();
};

class RegistryDataNone : public DCppObject<RegistryDataNone>
{
public:
  RegistryDataNone(DStruct* dstruct, DValue const& args);

  RealValue<DFunctionObject*> _data;

  DObject* data(void);

  attributeCount(RegistryDataNone, 1)
  attributeList(
                function(DObject, data, DNone)
               )
  memberList(RegistryDataNone, 
               method(RegistryDataNone, data)
            )
protected:
  ~RegistryDataNone();
};

class RegistryDataSZ : public DCppObject<RegistryDataSZ>, public RegistryData
{
public:
  RegistryDataSZ(DStruct* dstruct, DValue const& args);

  RealValue<DObject*>         _parent;
  RealValue<DFunctionObject*> _data;

  DUnicodeString data(void);

  attributeCount(RegistryDataSZ, 1)
  attributeList(
                function(DUnicodeString, data, DNone)
               )
  memberList(RegistryDataSZ, 
               method(RegistryDataSZ, data)
            )
protected:
  ~RegistryDataSZ();
};

class RegistryDataMultiSZ : public DCppObject<RegistryDataMultiSZ>, public RegistryData
{
public:
  RegistryDataMultiSZ(DStruct* dstruct, DValue const& args);

  RealValue<DObject*>         _parent;
  RealValue<DFunctionObject*> _data;

  DObject*      data(void);

  attributeCount(RegistryDataMultiSZ, 1)
  attributeList(
                function(DObject, data, DNone)
               )
  memberList(RegistryDataMultiSZ, 
               method(RegistryDataMultiSZ, data)
            )
protected:
  ~RegistryDataMultiSZ();
};

class RegistryDataDWord : public DCppObject<RegistryDataDWord>
{
public:
  RegistryDataDWord(DStruct* dstruct, DValue const& args);

  RealValue<DObject*>         _parent;
  RealValue<DFunctionObject*> _data;

  DUInt32 data(void);

  attributeCount(RegistryDataDWord, 1)
  attributeList(
                function(DUInt32, data, DNone)
               )
  memberList(RegistryDataDWord, 
               method(RegistryDataDWord, data)
            )
protected:
  ~RegistryDataDWord();
};

class RegistryDataDWordBE : public DCppObject<RegistryDataDWordBE>
{
public:
  RegistryDataDWordBE(DStruct* dstruct, DValue const& args);

  RealValue<DObject*>         _parent;
  RealValue<DFunctionObject*> _data;

  DInt32 data(void); //joachim metz doc said 32bit signed little endian ...

  attributeCount(RegistryDataDWordBE, 1)
  attributeList(
                function(DInt32, data, DNone)
               )
  memberList(RegistryDataDWordBE, 
               method(RegistryDataDWordBE, data)
            )
protected:
  ~RegistryDataDWordBE();
};

class RegistryDataQWord : public DCppObject<RegistryDataQWord>
{
public:
  RegistryDataQWord(DStruct* dstruct, DValue const& args);

  RealValue<DObject*>         _parent;
  RealValue<DFunctionObject*> _data;

  DUInt64 data(void);

  attributeCount(RegistryDataQWord, 1)
  attributeList(
                function(DUInt64, data, DNone)
               )
  memberList(RegistryDataQWord, 
               method(RegistryDataQWord, data)
            )
protected:
  ~RegistryDataQWord();
};

class RegistryDataBinary : public DCppObject<RegistryDataBinary>, public RegistryData
{
public:
  RegistryDataBinary(DStruct* dstruct, DValue const& args);

  RealValue<DObject*>         _parent;
  RealValue<DFunctionObject*> _data;

  DBuffer data(void);//Return a dstream so it can be read dynamically ? 

  attributeCount(RegistryDataBinary, 1)
  attributeList(
                function(DBuffer, data, DNone)
               )
  memberList(RegistryDataBinary, 
               method(RegistryDataBinary, data)
            )
protected:
  ~RegistryDataBinary();
};
#endif
