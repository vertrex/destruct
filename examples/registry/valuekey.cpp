#include "valuekey.hpp"

using namespace Destruct;

/**
 * ValueKey
 */
ValueKey::ValueKey(DStruct* dstruct, DValue const& args) : DCppObject<ValueKey>(dstruct, args), __size(0)
{
  this->init();
  this->realDataSize = 0;
}


ValueKey::~ValueKey(void)
{
}

DObject*       ValueKey::deserializeRaw(DValue const& arg)
{
  DObject* deserializer = arg;
  this->_deserializer = arg;
  DObject* stream = deserializer->getValue("stream");

 
  this->size = deserializer->call("DInt32");
  this->signature =  deserializer->call("DUInt16");
  this->nameLength = deserializer->call("DUInt16");
  this->dataLength = deserializer->call("DUInt32");

  DUInt64 dataOffsetOffset = stream->call("tell");
  this->dataOffset = deserializer->call("DUInt32");
  this->dataType =   deserializer->call("DUInt32");
  this->valueType =  deserializer->call("DUInt16");
  this->unknown1 =   deserializer->call("DUInt16");

  if (this->nameLength)
  {
    DBuffer buffer = deserializer->getValue("stream").get<DObject*>()->call("read", RealValue<DInt64>((DInt64)this->nameLength));
    this->name = DUnicodeString(std::string((char*)buffer.data(), this->nameLength));
  }
  else
    this->name = "(default)";

  if (0x80000000 & this->dataLength)
  {
    this->realDataSize = this->dataLength & ~0x80000000;
    this->realDataOffset = dataOffsetOffset - 4; //because we skip DUInt32 when we read a cell hive 
  }
  else
  {
    this->realDataSize = this->dataLength;
    this->realDataOffset = this->dataOffset + 0x1000;
  }

  return (RealValue<DObject*>(this));
}

DUnicodeString  ValueKey::valueTypeName(void)
{
 return (ValueKey::registryType(this->dataType));
}

DObject*        ValueKey::data(void)
{
  DStruct* dataStruct = DStructs::instance().find(ValueKey::registryTypeStructName(this->dataType));
  if (dataStruct == NULL)
  {
    DObject* regnone = DStructs::instance().generate("RegistryDataNone");
    return (RealValue<DObject*>(regnone));    
  }
  DObject* dataObject = dataStruct->newObject(RealValue<DObject*>(this));
  return (dataObject);
}

/**
 * RegistryDataNone
 */

RegistryDataNone::RegistryDataNone(DStruct* dstruct, DValue const& args) : DCppObject<RegistryDataNone>(dstruct, args)
{
  this->init();
}

RegistryDataNone::~RegistryDataNone(void)
{
}

DObject*        RegistryDataNone::data(void)
{
  return (RealValue<DObject*>(DNone)); 
}

/**
 * RegistryDataSZ
 */
RegistryDataSZ::RegistryDataSZ(DStruct* dstruct, DValue const& args) : DCppObject<RegistryDataSZ>(dstruct, args)
{
  this->init();
  this->_parent = args;
}

RegistryDataSZ::~RegistryDataSZ(void)
{
}

DUnicodeString        RegistryDataSZ::data(void)
{
  try
  {
    ValueKey* parent = (ValueKey*)(DObject*)this->_parent;
    DObject* stream = ((DObject*)parent->_deserializer)->getValue("stream"); 
    stream->call("seek", RealValue<DUInt64>(parent->realDataOffset));

    ((DObject*)parent->_deserializer)->call("DUInt32"); //size ?
    DBuffer buffer = stream->call("read", RealValue<DInt64>(parent->realDataSize));

    return (DUnicodeString((char*)buffer.data(), parent->realDataSize, "UTF16-LE"));
  }
  catch (DException const &exception)
  {
    throw DException("RegistryDataSZ::data() Error:\n" + exception.error());
  }
}

/**
 * RegistryDataMultiSZ
 */
RegistryDataMultiSZ::RegistryDataMultiSZ(DStruct* dstruct, DValue const& args) : DCppObject<RegistryDataMultiSZ>(dstruct, args)
{
  this->init();
  this->_parent = args;
}

RegistryDataMultiSZ::~RegistryDataMultiSZ(void)
{
}

DObject*        RegistryDataMultiSZ::data(void)
{
  try
  {
    ValueKey* parent = (ValueKey*)(DObject*)this->_parent;
    DObject* stream = ((DObject*)parent->_deserializer)->getValue("stream"); 
    stream->call("seek", RealValue<DUInt64>(parent->realDataOffset));

    ((DObject*)parent->_deserializer)->call("DUInt32"); //skip cell id
    DBuffer buffer = stream->call("read", RealValue<DInt64>(parent->realDataSize));

    DObject* vector = DStructs::instance().generate("DVectorString");

    int32_t pos = 0;
    int32_t lastOffset = 0;
    uint16_t* data = (uint16_t*)buffer.data();
    for (; pos != buffer.size() - 1; pos++)
    {
      if (data[pos] == 0)
      {
        DUnicodeString str =  DUnicodeString((char*)((uint16_t*)data + lastOffset), (pos - lastOffset) * 2, "UTF16-LE");
        vector->call("push", RealValue<DUnicodeString>(str));
        lastOffset += pos + 1; 
      }
      if (data[pos] == 0 && data[pos + 1] ==0) //There is an other NULL uint16_t at end of list
        break;
    }
    return (vector); 
  }
  catch (DException const &exception)
  {
    throw DException("RegistryDataMultiSZ::data() Error:\n" + exception.error());
  } 
}


/**
 * RegistryDataDWord
 */
RegistryDataDWord::RegistryDataDWord(DStruct* dstruct, DValue const& args) : DCppObject<RegistryDataDWord>(dstruct, args)
{
  this->init();
  this->_parent = args;
}

RegistryDataDWord::~RegistryDataDWord(void)
{
}

DUInt32 RegistryDataDWord::data(void)
{
  try
  { 
    ValueKey* parent = (ValueKey*)(DObject*)this->_parent;
    DObject* stream = ((DObject*)parent->_deserializer)->getValue("stream"); 
    stream->call("seek", RealValue<DUInt64>(parent->realDataOffset));

    ((DObject*)parent->_deserializer)->call("DUInt32"); //size ?
    DUInt32 value = ((DObject*)parent->_deserializer)->call("DUInt32"); //size ?
    return value;
  }
  catch (DException const &exception)
  {
    throw DException("RegistryDataDWord::data() Error:\n" + exception.error());
  }  
}

/**
 * RegistryDataDWordBE
 */
RegistryDataDWordBE::RegistryDataDWordBE(DStruct* dstruct, DValue const& args) : DCppObject<RegistryDataDWordBE>(dstruct, args)
{
  this->init();
  this->_parent = args;
}

RegistryDataDWordBE::~RegistryDataDWordBE(void)
{
}

DInt32 RegistryDataDWordBE::data(void)
{
  try
  {
    ValueKey* parent = (ValueKey*)(DObject*)this->_parent;
    DObject* stream = ((DObject*)parent->_deserializer)->getValue("stream"); 
    stream->call("seek", RealValue<DUInt64>(parent->realDataOffset));

    ((DObject*)parent->_deserializer)->call("DInt32"); //size ?
    DInt32 value = ((DObject*)parent->_deserializer)->call("DInt32"); //size ?
    return value;
  }
  catch (DException const &exception)
  {
    throw DException("RegistryDataDWordBE::data() Error:\n" + exception.error());
  }  
}

/**
 * RegistryDataQWord
 */
RegistryDataQWord::RegistryDataQWord(DStruct* dstruct, DValue const& args) : DCppObject<RegistryDataQWord>(dstruct, args)
{
  this->init();
  this->_parent = args;
}

RegistryDataQWord::~RegistryDataQWord(void)
{
}

DUInt64 RegistryDataQWord::data(void)
{
  try
  {
    ValueKey* parent = (ValueKey*)(DObject*)this->_parent;
    DObject* stream = ((DObject*)parent->_deserializer)->getValue("stream"); 
    stream->call("seek", RealValue<DUInt64>(parent->realDataOffset));

    ((DObject*)parent->_deserializer)->call("DInt32"); //size ?
    DUInt64 value = ((DObject*)parent->_deserializer)->call("DUInt64");
    return value;
  }
  catch (DException const &exception)
  {
    throw DException("RegistryDataQWord::data() Error:\n" + exception.error());
  }  
}


/**
 * RegistryDataBinary
 */
RegistryDataBinary::RegistryDataBinary(DStruct* dstruct, DValue const& args) : DCppObject<RegistryDataBinary>(dstruct, args)
{
  this->init();
  this->_parent = args;
}

RegistryDataBinary::~RegistryDataBinary(void)
{
}

DBuffer RegistryDataBinary::data(void)
{
  try
  {
    ValueKey* parent = (ValueKey*)(DObject*)this->_parent;
    DObject* stream = ((DObject*)parent->_deserializer)->getValue("stream"); 
    stream->call("seek", RealValue<DUInt64>(parent->realDataOffset));

    ((DObject*)parent->_deserializer)->call("DInt32"); //size ?
    return (stream->call("read", RealValue<DInt64>(parent->realDataSize)));
  }
  catch (DException const &exception)
  {
    throw DException("RegistryDataBinary::data() Error:\n" + exception.error());
  } 
}
