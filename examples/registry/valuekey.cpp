#include "valuekey.hpp"

using namespace Destruct;

/**
 * ValueKey
 */
ValueKey::ValueKey(DStruct* dstruct, DValue const& args) : DCppObject<ValueKey>(dstruct, args), __size(0)
{
  this->init();
  this->realDataSize = 0;
  this->minor = 5;
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
    DBuffer buffer = stream->call("read", RealValue<DInt64>((DInt64)this->nameLength));
    if (this->valueType == 0x1)
      this->name = DUnicodeString(std::string((char*)buffer.data(), this->nameLength)); ///XXX encode ?
    else
      this->name = DUnicodeString((char*)buffer.data(), this->nameLength, "UTF16-LE"); ///XXX encode ?
        
  }
  else
    this->name = "(default)";

  //check if data in offset
  if (0x80000000 & this->dataLength)
  {
    this->realDataSize = (DInt32)(this->dataLength & ~0x80000000);
    this->realDataOffset = dataOffsetOffset;
  }
  else
  {
    this->realDataSize = (DInt32)this->dataLength;
    this->realDataOffset = this->dataOffset + 0x1000 + 4; //skip first byte
  }

  DUInt64 previousOffset = stream->call("tell");
  if (this->minor == 3 ||  this->realDataSize < 16344)
  {
    this->dataOffsets = DStructs::instance().generate("DVectorUInt32");
    ((DObject*)this->dataOffsets)->call("push", RealValue<DUInt32>(this->realDataOffset));
    ((DObject*)this->dataOffsets)->destroy();
  }
  else
  {
    stream->call("seek", RealValue<DUInt64>(this->realDataOffset));
    DObject* bigData = DStructs::instance().generate("RegistryBigData");
    deserializer->call("DObject", RealValue<DObject*>(bigData));
    this->dataOffsets = bigData->getValue("offsets");
    //((DObject*)this->dataOffsets)->addRef(); //check with lsan XXX
    bigData->destroy(); //or segfault  cause offsets is deleted
  }
  stream->call("seek", RealValue<DUInt64>(previousOffset)); //usefull ? c deja seek ds un parent a check XXX

  //deserializer->destroy(); //segfault
  //stream->destroy();
  
  return (this);
}

DUnicodeString  ValueKey::valueTypeName(void)
{
 return (ValueKey::registryType(this->dataType));
}

DObject*        ValueKey::data(void)
{
  DStruct* dataStruct = DStructs::instance().find(ValueKey::registryTypeStructName(this->dataType));
  if (dataStruct == NULL)
    return (DNone);
  DObject* dataObject = dataStruct->newObject(RealValue<DObject*>(this));
  return (dataObject);
}


/**
 *  RegistryBigData
 */
RegistryBigData::RegistryBigData(DStruct* dstruct, DValue const& args) : DCppObject<RegistryBigData>(dstruct, args)
{
  this->init();
}

RegistryBigData::~RegistryBigData()
{
}

DObject*       RegistryBigData::deserializeRaw(DValue const& args)
{
  DObject* serializer  = args;
  DObject* stream = serializer->getValue("stream");

  //DUInt32  binSize = serializer->call("DUInt32"); //already skipped
  DUInt16  signature = serializer->call("DUInt16");
  DUInt16  numberOfSegments = serializer->call("DUInt16");
  DUInt32  segmentListOffset = (DUInt32)serializer->call("DUInt32") + 0x1000;

  stream->call("seek", RealValue<DUInt64>(segmentListOffset));
  DObject* offsetsList = DStructs::instance().generate("DVectorUInt32");

  DUInt32 dataListSize = serializer->call("DUInt32"); 
  for (DUInt16 count = 0; count < numberOfSegments; ++count)
    offsetsList->call("push", RealValue<DUInt32>((DUInt32)serializer->call("DUInt32") + 4 + 0x1000));
  
  this->offsets = offsetsList;

  //stream->destroy();
  //serializer->destroy();
  return (this);
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
 *  RegistryData
 */
RegistryData::RegistryData()
{
}

RegistryData::~RegistryData()
{
}

DBuffer    RegistryData::read(DValue const& _parent)
{
  ValueKey* parent = (ValueKey*)_parent.get<DObject*>();
 
  DObject* deserializer = parent->_deserializer;
  DObject* stream = deserializer->getValue("stream"); 

  if (parent->minor == 3 || parent->realDataSize < 16344)
  {
    stream->call("seek", RealValue<DUInt64>(parent->realDataOffset));
    return stream->call("read", RealValue<DInt64>((DInt64)parent->realDataSize));
  }
  else
  {
    uint8_t*    buffer = new uint8_t[parent->realDataSize];
    DObject* offsets = parent->dataOffsets;
    DUInt64 offsetCount = offsets->call("size");
    DUInt64 sizeToRead = 0;
    DUInt64 sizeReaded = 0;

    for (DUInt64 index = 0; index < offsetCount; ++index)
    {
      if (parent->realDataSize - sizeReaded < 16344)
        sizeToRead = parent->realDataSize - sizeReaded;
      else
        sizeToRead = 16344;

      stream->call("seek", RealValue<DUInt64>((DUInt32)offsets->call("get", RealValue<DUInt64>(index))));

      DBuffer tmpBuffer = stream->call("read", RealValue<DInt64>(sizeToRead));
      memcpy(buffer + sizeReaded, tmpBuffer.data(), sizeToRead);
      sizeReaded += sizeToRead;
    }
    DBuffer dbuffer((uint8_t*)buffer, parent->realDataSize);
    delete buffer;
    return (dbuffer);
  }
  //deserializer->destroy();
  //stream->destroy();

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
    DBuffer buffer = this->read(this->_parent);
    return (DUnicodeString((char*)buffer.data(), buffer.size() - 2, "UTF16-LE")); //-1 pas de \\0 pour icu ?
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
    DBuffer buffer = this->read(this->_parent);
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
    return (this->read(this->_parent));
  }
  catch (DException const &exception)
  {
    throw DException("RegistryDataBinary::data() Error:\n" + exception.error());
  } 
}
