#include "serializerpc.hpp"
#include "networkstream.hpp"
#include "clientobject.hpp"
#include "serverobject.hpp"
#include "clientfunctionobject.hpp"

#include "dsimpleobject.hpp"
/*
 *   DSerializeRPC 
 */
namespace Destruct
{

SerializeRPC::SerializeRPC(DStruct* dstruct, DValue const& args) : DCppObject<SerializeRPC>(dstruct, args), __stream(args.get<DObject*>())
{
  this->init(); 
}

SerializeRPC::SerializeRPC(SerializeRPC const& copy) : DCppObject<SerializeRPC>(copy), __stream(copy.__stream)
{
  this->init();
}

SerializeRPC::~SerializeRPC()
{
  ((DObject*)__stream)->destroy();
}

void    SerializeRPC::sDObject(DValue const& args)
{
  DObject* dobject = args.get<DObject*>();
  if (dobject == NULL) //XXX XXX ct fait avant mais ca faisait quoi ? :)
    return (this->sDNone());
  

  DStruct const* dstruct = dobject->instanceOf(); 
  if (dstruct == NULL)
    throw DException("SerializeRPCBinary::sDObject(DValue) object instance is NULL");
  this->sDUnicodeString(RealValue<DUnicodeString>(dstruct->name()));

  /*
   *  We follow special serialization and iterable method first 
   */
  int32_t index = dstruct->findAttribute("iterator");
  if (index != -1)
  {
    DObject* iterator = dobject->call("iterator").get<DObject*>();
    DType returnType = DType(iterator->instanceOf()->attribute("currentItem").type().getReturnType());

    DValue count = dobject->call("size");   
    this->sDUInt64(count);
 
    for ( ;iterator->call("isDone").get<DInt8>() != 1; iterator->call("nextItem"))
    {
      DValue value = iterator->call("currentItem");
      this->call(returnType.name(), value);
    }
    //iterator->destroy(); //destroy get<DObject*> instance
    //iterator->destroy(); //Destroy "call" instance
    //dobject->destroy();
    return ;
  }
  
  for (DStruct::DAttributeIterator attribute = dstruct->attributeBegin(); attribute != dstruct->attributeEnd(); ++attribute)
  {
    DType type = attribute->type();
    DType::Type_t type_t = type.getType();
    if (type_t == DType::DMethodType || type_t == DType::DNoneType || type_t == DType::DUnknownType)
      continue;
    else
    {
      DValue value = dobject->getValue((*attribute).name());
      this->call(type.name(), value);
    }
  }
  //dobject->destroy();
}


void    SerializeRPC::sDStruct(DValue const& args)
{
  DStruct* dstruct = args.get<DStruct*>();
//  if (dstruct == NULL)
//     throw DException

  this->sDUnicodeString(RealValue<DUnicodeString>(dstruct->name())); 

  size_t attributeCount = dstruct->attributeCount();
  this->sDUInt32(RealValue<DUInt32>(attributeCount)); //XXX size t

  for (DStruct::DAttributeIterator i = dstruct->attributeBegin(); i != dstruct->attributeEnd(); ++i)
  {
    this->sDUnicodeString(RealValue<DUnicodeString>((*i).name()));
    DType::Type_t type = (*i).type().getType();
    this->sDUInt8(RealValue<DUInt8>(type)); //DUInt8?
    if (type == DType::DMethodType)
    {
       DType::Type_t argumentType = (*i).type().getArgumentType();
       this->sDUInt8(RealValue<DUInt8>(argumentType));
       DType::Type_t returnType = (*i).type().getReturnType();
       this->sDUInt8(RealValue<DUInt8>(returnType));
    }
  } 
}

void    SerializeRPC::sDNone(void)
{
  this->sDObject(RealValue<DObject*>(DNone)); 
}

void    SerializeRPC::sDMethod(DValue const& args)
{
  //pass or throw ?
}

void    SerializeRPC::sDUnicodeString(DValue const& args)
{
  DObject* stream = this->__stream;

  DUnicodeString str = args.get<DUnicodeString>();
  RealValue<DInt64>  dsize((int64_t)str.size());

  stream->call("write", RealValue<DBuffer>(dsize.asDBuffer()));
  stream->call("write", RealValue<DBuffer>(args.asDBuffer()));
}

void    SerializeRPC::sDBuffer(DValue const& args)
{
  DObject* stream = this->__stream;
  //write size
  //write buffer data ? (for other only write data or do it for each so it self contained ??) 
  stream->call("write", args);
}

void    SerializeRPC::sDInt8(DValue const& args)
{
  DObject* stream = this->__stream;
  stream->call("write", RealValue<DBuffer>(args.asDBuffer()));
}

void    SerializeRPC::sDInt16(DValue const& args)
{
  DObject* stream = this->__stream;
  stream->call("write", RealValue<DBuffer>(args.asDBuffer()));
}

void    SerializeRPC::sDInt32(DValue const& args)
{
  DObject* stream = this->__stream;
  stream->call("write", RealValue<DBuffer>(args.asDBuffer()));
}

void    SerializeRPC::sDInt64(DValue const& args)
{
  DObject* stream = this->__stream;
  stream->call("write", RealValue<DBuffer>(args.asDBuffer()));
}

void    SerializeRPC::sDUInt8(DValue const& args)
{
  DObject* stream = this->__stream;
  stream->call("write", RealValue<DBuffer>(args.asDBuffer()));
}

void    SerializeRPC::sDUInt16(DValue const& args)
{
  DObject* stream = this->__stream;
  stream->call("write", RealValue<DBuffer>(args.asDBuffer()));
}

void    SerializeRPC::sDUInt32(DValue const& args)
{
  DObject* stream = this->__stream;
  stream->call("write", RealValue<DBuffer>(args.asDBuffer()));
}

void    SerializeRPC::sDUInt64(DValue const& args)
{
  DObject* stream = this->__stream;
  stream->call("write", RealValue<DBuffer>(args.asDBuffer()));
}

/**
 *  Deserialize RPC
 */

DeserializeRPC::DeserializeRPC(DStruct* dstruct, DValue const& args) : DCppObject<DeserializeRPC>(dstruct, args), __stream(args.get<DObject*>())
{
  this->init(); 
}

DeserializeRPC::DeserializeRPC(DeserializeRPC const& copy) : DCppObject<DeserializeRPC>(copy), __stream(copy.__stream)
{
  this->init();
}

DeserializeRPC::~DeserializeRPC()
{
  ((DObject*)__stream)->destroy();
}

DObject*        DeserializeRPC::dDObject(DValue const& value)
{
  DStruct* dstruct = NULL;
  DObject* dobject = value.get<DObject*>();
 
  if (dobject == DNone)
  {
    DStructs& dstructs = DStructs::instance();
    DUnicodeString structName = this->call("DUnicodeString").get<DUnicodeString>();

    if (structName == "None")
      return (RealValue<DObject*>(DNone));

    dstruct = dstructs.find(structName);
    if (dstruct == NULL)
      throw DException("Can't find struct : '" + structName + "' in destruct database.");
    dobject = dstruct->newObject();
  }
  else
    dstruct = dobject->instanceOf();
  
  
  int32_t index = dstruct->findAttribute("iterator");
  int32_t hasNewItem = dstruct->findAttribute("newItem");
  if (index != -1)
  {
    DType::Type_t   returnType = dstruct->attribute("get").type().getReturnType();
    DUInt64 count = this->call("DUInt64").get<DUInt64>();
   
    if (hasNewItem != -1)
    {
      for (DUInt64 index = 0; index < count; index++)
      {
         DUnicodeString structName = this->call("DUnicodeString").get<DUnicodeString>(); 
         DObject* item = dobject->call("newItem").get<DObject*>();
         this->call("DObject", RealValue<DObject*>(item));
         dobject->call("setItem", RealValue<DObject*>(item));
         //item->destroy();
         //item->destroy();
      }
    }
    else
    {
      for (DUInt64 index = 0; index < count; index++) 
      {
         
         DValue value = this->call(DType(returnType).name());
         dobject->call("push", value);
         //if (returnType == DType::DObjectType)
         //{
         //DObject* obj = value.get<DObject*>();
         //obj->destroy();
         //obj->destroy();
         //}
      }
    }
    return (dobject);
  }

  for (DStruct::DAttributeIterator attribute = dstruct->attributeBegin(); attribute != dstruct->attributeEnd(); ++attribute)
  {
    DType type = attribute->type();
    if (type.getType() == DType::DMethodType)
      continue;
    else if (type.getType() == DType::DUnknownType)
      continue;
    else
    {
      DValue value = this->call(type.name());
      dobject->setValue((*attribute).name(), value);
    }
    //if type un dobject
    //obj->destroy() * 2 ?
  }

  return (dobject);
}

DStruct*        DeserializeRPC::dDStruct(void)
{
  DStruct* dstruct = NULL; 

  DUnicodeString structName = this->call("DUnicodeString").get<DUnicodeString>();
  DUInt32 attributeCount = this->call("DUInt32").get<DUInt32>(); //32 ?

  if ((dstruct = new DStruct(0, structName, DSimpleObject::newObject)) == NULL)//inheritance ? 
    return (NULL);
  for (size_t i = 0; i < attributeCount; i++) 
  {
     DUnicodeString name = this->call("DUnicodeString").get<DUnicodeString>();
     
     DType::Type_t type = (DType::Type_t)this->call("DUInt8").get<DUInt8>(); //XXX type serialization ?
     if (type == DType::DMethodType)
     {
       DType::Type_t argumentType = (DType::Type_t)this->call("DUInt8").get<DUInt8>();
       DType::Type_t returnType = (DType::Type_t)this->call("DUInt8").get<DUInt8>();
       dstruct->addAttribute(DAttribute(returnType, name, argumentType, type));
     }
     else
       dstruct->addAttribute(DAttribute(type, name));
  }
  DStructs::instance().registerDStruct(dstruct); //needed for complex object that register struct and need it later to deserialize rest of file 
  return (dstruct); 
}

DObject*        DeserializeRPC::dDNone(void)
{
  return (DNone);
}

DFunctionObject* DeserializeRPC::dDMethod(void)
{
  return (NULL);
}

DUnicodeString  DeserializeRPC::dDUnicodeString(void)
{
  DObject* stream = this->__stream;

  DInt64  size = this->call("DInt64").get<DInt64>(); 
  DBuffer buffer = stream->call("read", RealValue<DInt64>(size)).get<DBuffer>();

  //char* data = new char[size + 1];
  //is.read(data, size);
  //data[size] = 0;
  //delete[] data;

  std::string str((const char*)buffer.data(), buffer.size()); //DUnicodeString constructor XXX
  //DUnicodeString string(dbuffer.data(), size);
  //delete DBuffer->data;

  return (str);
}

DBuffer         DeserializeRPC::dDBuffer(void)
{
  //DObject* stream = this->__stream;
  //stream->call("write", args);
  //read buffer size
  //read buffer data

  return DBuffer();
}

DInt8           DeserializeRPC::dDInt8(void)
{
  DObject* stream = this->__stream;
  DBuffer buffer = stream->call("read", RealValue<DInt64>(sizeof(DInt8))).get<DBuffer>();

  return (*((DInt8*)buffer.data()));
}

DInt16          DeserializeRPC::dDInt16(void)
{
  DObject* stream = this->__stream;
  DBuffer buffer = stream->call("read", RealValue<DInt64>(sizeof(DInt16))).get<DBuffer>();

  return (*((DInt16*)buffer.data()));
}

DInt32          DeserializeRPC::dDInt32(void)
{
  DObject* stream = this->__stream;
  DBuffer buffer = stream->call("read", RealValue<DInt64>(sizeof(DInt32))).get<DBuffer>();

  return (*((DInt32*)buffer.data()));
}

DInt64          DeserializeRPC::dDInt64(void)
{
  DObject* stream = this->__stream;
  DBuffer buffer = stream->call("read", RealValue<DInt64>(sizeof(DInt64))).get<DBuffer>();

  return (*((DInt64*)buffer.data()));
}

DUInt8          DeserializeRPC::dDUInt8(void)
{
  DObject* stream = this->__stream;
  DBuffer buffer = stream->call("read", RealValue<DInt64>(sizeof(DUInt8))).get<DBuffer>();

  return (*((DUInt8*)buffer.data()));
}

DUInt16         DeserializeRPC::dDUInt16(void)
{
  DObject* stream = this->__stream;
  DBuffer buffer = stream->call("read", RealValue<DInt64>(sizeof(DUInt16))).get<DBuffer>();

  return (*((DUInt16*)buffer.data()));
}

DUInt32         DeserializeRPC::dDUInt32(void)
{
  DObject* stream = this->__stream;
  DBuffer buffer = stream->call("read", RealValue<DInt64>(sizeof(DUInt32))).get<DBuffer>();

  return (*((DUInt32*)buffer.data()));
}

DUInt64         DeserializeRPC::dDUInt64(void)
{
  DObject* stream = this->__stream;
  DBuffer buffer = stream->call("read", RealValue<DInt64>(sizeof(DUInt64))).get<DBuffer>();
  
  return (*((DUInt64*)buffer.data()));
}



/*


DSerializeRPC::DSerializeRPC(NetworkStream& stream, ObjectManager<DObject*>& objects, ObjectManager<ServerFunctionObject*>& functionObjects) : __networkStream(stream), __objects(objects), __functionObjects(functionObjects), __streamString(DStreamString(DStructs::instance().find("DStreamString"), RealValue<DObject*>(DNone)))
{
}

DSerializeRPC*   DSerializeRPC::create(void)
{
  return (new DSerializeRPC(this->__networkStream, this->__objects, this->__functionObjects));
}

const DUnicodeString DSerializeRPC::name(void)
{
  return ("RPC");
}


//   Serialization

 
bool DSerializeRPC::serialize(DStream& networkStream, DStruct& dstruct)
{
//  DSerialize* binarySerializer = DSerializers::to("Binary");
//  binarySerializer->serialize(this->__streamString, dstruct);
  DObject* serializer = DStructs::instance().generate("SerializeBinary", this->__streamString);
  serializer->call("DStruct", RealValue<DStruct*>(dstruct)); //& to * ?

  this->__networkStream << this->__streamString;

  return (true);
}

bool DSerializeRPC::serialize(DStream& networkStream, DValue value, DType::Type_t type)
{
  if (type == DType::DNoneType)
    return (true);
  else if (type == DType::DObjectType)
  {
    DObject* dobject = value.get<DObject*>();
    this->serialize(this->__networkStream, dobject);
  }
  else if (type == DType::DMethodType)
  {
    DException("DFunctionType serialization not implemented");
  }
  else
  {
    this->__streamString.clear();
    this->__streamString.write(value); //XXX new serialiatiuon
    //this->__streamString << value;
    this->__networkStream << this->__streamString;
  } 
  this->__streamString.clear();

  return (true);
}

bool DSerializeRPC::serialize(DStream& networkStream, DFunctionObject* dfunctionObject, DType::Type_t argumentType, DType::Type_t returnType)
{
  ServerFunctionObject* serverFunctionObject = new ServerFunctionObject(dfunctionObject, argumentType, returnType);
  RealValue<DUInt64> id(this->__functionObjects.registerObject(serverFunctionObject));
  
  //id.serialize(this->__streamString); //new serialization
  this->__streamString.write(id);

  this->__networkStream << this->__streamString;

  return (true);
}

bool DSerializeRPC::serialize(DStream& networkStream, DObject*  dobject) 
{
  RealValue<DUnicodeString> objectName(dobject->instanceOf()->name());
  RealValue<DUInt64> id(this->__objects.registerObject(dobject));

  //objectName.serialize(this->__streamString); //XXX new serialization
  //id.serialize(this->__streamString); //XXX
  this->__streamString.write(objectName);
  this->__streamString.write(id);

  this->__networkStream << this->__streamString;

  return (true);
}


 //  Deserialization
 
DStruct* DSerializeRPC::deserialize(DStream& input)
{ 
  this->__networkStream >> this->__streamString;
  DSerialize* binarySerializer = DSerializers::to("Binary");
  DStruct* dstruct = binarySerializer->deserialize(this->__streamString);

  return (dstruct);
}

DValue DSerializeRPC::deserialize(DStream& networkStream, DType::Type_t type)
{
  if (type == DType::DNoneType)
    return (RealValue<DObject*>(DNone));

  this->__networkStream >> this->__streamString;
  if (type == DType::DObjectType)
  {
    RealValue<DUnicodeString> objectName("");
    RealValue<DUInt64> id;
  
    //objectName.unserialize(this->__streamString);
    //id.unserialize(this->__streamString);
    this->__streamString.read(objectName);//.get<DUnicodeString>(); //new serialization
    this->__streamString.read(id); //.get<DUInt64>();

    DStruct* dstruct = Destruct::DStructs::instance().find(objectName);
    if (dstruct == NULL)
    {
      std::cout << "DSerializeRPC Can't deserialize object not find in base must get struct named :  " << objectName << std::endl;
      return RealValue<DObject*>(DNone);
    }

    return RealValue<DObject*>(new ClientObject(this->__networkStream, this, id, dstruct));
  }

  DValue value(DType(type).newValue());
  //this->__streamString >> value;
  this->__streamString.read(value); //XXX XXX new serializaiton streamString -> value this->__streamString.read?
  this->__streamString.clear();
  return (value);
}

DValue DSerializeRPC::deserialize(DStream& input, DType::Type_t argumentType, DType::Type_t returnType)
{
  this->__networkStream >> this->__streamString;

  RealValue<DUInt64> id;
  //id.unserialize(this->__streamString);
  this->__streamString.read(id); 
 
  return (RealValue<DFunctionObject*>(new ClientFunctionObject(this->__networkStream, this, id, argumentType, returnType)));
}

bool DSerializeRPC::deserialize(DStream& input, DObject* dobject) //UNUSED //XXX must return a DObject can't construct it before !
{
 return (false);
}
*/
}
