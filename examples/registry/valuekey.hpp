#ifndef __valuekey_hh__
#define __valuekey_hh__

#include "registry_common.hpp"

using namespace Destruct;

class ValueKey : public DCppObject<ValueKey>
{
public:
  ValueKey(DStruct* dstruct, DValue const& args);

  DValue  deserializeRaw(DValue const& stream);
  DValue  valueTypeName(void);
 
  //RealValue<DUInt8>            unknown;
  RealValue<DUInt16>           signature, nameLength, unknown1, valueType;//named, unkown2
  RealValue<DInt32>            size;
  RealValue<DUInt32>           dataOffset, dataLength, dataType;
  RealValue<DObject*>          name, data;
  RealValue<DFunctionObject*>  _valueTypeName;

  attributeCount(ValueKey, 11)
  attributeList(
                attribute(DInt32, size)
                attribute(DUInt16, signature)
                attribute(DUInt16, nameLength)
                attribute(DUInt32, dataLength)
                attribute(DUInt32, dataOffset)
                attribute(DUInt32, dataType)//XXX data in offset ? for 'big data'
                attribute(DUInt16, valueType)
                attribute(DUInt16, unknown1)
                attribute(DObject, name)
                attribute(DObject, data)
                function(DUnicodeString, valueTypeName, DNone)
                //function(DUInt8, deserializeRaw, DObject)
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
             member(ValueKey, data)
             method(ValueKey, valueTypeName)
             //method(ValueKey, deserializeRaw)
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
                                          "REG_QDWORD",
                                        }; 
    return registryType[dataType]; 
  }
};

class RegistryValueData : public DCppObject<RegistryValueData>
{
public:
          RegistryValueData(DStruct* dstruct, DValue const& args);
          ~RegistryValueData();

  DValue  deserializeRaw(DValue const& stream);


  attributeCount(RegistryValueData, 1)
  attributeList(
                function(DUInt8, deserializeRaw, DObject)
               )
  memberList(RegistryValueData, 
             method(RegistryValueData, deserializeRaw)
            )
private:
  RealValue<DFunctionObject*>        _deserializeRaw; 
};

#endif
