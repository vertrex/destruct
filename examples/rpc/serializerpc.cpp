#include "serializerpc.hpp"
#include "networkstream.hpp"
#include "dsimpleobject.hpp"
#include "destruct.hpp"
#include "rpcobject.hpp"
/*
 *   DSerializeRPC 
 */

namespace Destruct
{

DSerializeRPC::DSerializeRPC(NetworkStream stream, ObjectManager & objects) : __networkStream(stream), __objects(objects)
{
}

const std::string DSerializeRPC::name(void)
{
  return ("RPC");
}

DSerializeRPC*   DSerializeRPC::create(void)
{
  return (new DSerializeRPC(this->__networkStream, this->__objects));
}

//throw rather than many check // ? use ./open read rather than slow stream?
bool DSerializeRPC::serialize(DStream& output, DStruct& dstruct)
{
        //std::cout << "SERIALIZE 1 " << std::endl;
  if (this->serialize(output, dstruct.name()) == false)
    return (false);

  size_t attributeCount = dstruct.attributeCount(); 
  output.write((char *)&attributeCount, sizeof(attributeCount));

  for (DStruct::DAttributeIterator i = dstruct.attributeBegin(); i != dstruct.attributeEnd(); ++i)
  {
    if (this->serialize(output, (*i).name()) == false)
      return (false);
   
//un peu bizarre avec les fonctions car comment on c combien on doit deserializer ...
 
    DType::Type_t type = (*i).type().getType();
    if (output.write((char*)&type, sizeof(type)).fail())
      return (false);
    if (type == DType::DMethodType)
    {
       DType::Type_t argumentType = (*i).type().getArgumentType();
       DType::Type_t returnType = (*i).type().getReturnType();
       if (output.write((char*)&argumentType, sizeof(type)).fail())
         return (false);
       if (output.write((char*)&returnType, sizeof(type)).fail())
         return (false);
    }
  } 

  return (true);
}

DStruct* DSerializeRPC::deserialize(DStream& input) //UNUSED
{ 
  //std::cout << "SERIALIZE 2 " << std::endl;
  //std::string name;
  //size_t attributeCount;
  //DStruct* dstruct = NULL; 

  //if (this->deserialize(input, name) == false)
    //return (NULL);
  //if (input.read((char*)&attributeCount, sizeof(size_t)).fail())
    //return (NULL);
 
  //if ((dstruct = new DStruct(0, name, DSimpleObject::newObject)) == NULL) //XXX parent etc !! SimpleObject ?
    //return (NULL);
  //for (size_t i = 0; i < attributeCount; i++) 
  //{
     //DType::Type_t type;
     //std::string   name;

     //if (this->deserialize(input, name) == false)
       //return (NULL);
     //if (input.read((char*)&type, sizeof(type)).fail())
       //return (NULL); //strange error checking !
     //if (type == DType::DMethodType)
     //{
       //DType::Type_t argumentType;
       //DType::Type_t returnType;
       //if (input.read((char*)&argumentType, sizeof(type)).fail())
         //return (NULL); //s
       //if (input.read((char*)&returnType, sizeof(type)).fail())
         //return (NULL); //s
       //dstruct->addAttribute(DAttribute(returnType, name, argumentType, type));
     //}
     //else
       //dstruct->addAttribute(DAttribute(type, name));
  //}
  //return (dstruct);
  return (NULL); 
}

bool DSerializeRPC::serialize(DStream& output, DValue value, DType::Type_t type)
{
        //std::cout << "SERIALIZE 3 " << std::endl;
  if (type == DType::DObjectType)
  {
    DObject* dobject = value.get<DObject*>();
    this->serialize(output, *dobject);
  }
  else
    output << value;

  return (true);
}

bool DSerializeRPC::serialize(DStream& output, DObject&  dobject)
{
        //std::cout << "SERIALIZE 4 " << std::endl;
  DValue v = RealValue<DUnicodeString>(dobject.instanceOf()->name());
  DUInt64 id = this->__objects.registerObject(&dobject);
  DValue rid = RealValue<DUInt64>(id);

  output << v;
  output << rid;
 
  return (true);
}

DValue DSerializeRPC::deserialize(DStream& input, DType::Type_t type)
{
        //std::cout << "SERIALIZE 5 " << std::endl;
  if (type == DType::DObjectType)
  {
    DValue rstring = RealValue<DUnicodeString>("");        
 
    DValue id = RealValue<DUInt64>();
    input >>  rstring; //input >> objectName; //not implemented for string !
    input >> id;
    std::string objectName = rstring.get<DUnicodeString>();

    DStruct* dstruct = Destruct::Destruct::instance().find(objectName);
    if (dstruct == NULL)
    {
      std::cout << "Can't deserialize object not find in base must get struct named :  " << objectName << std::endl;
      return RealValue<DObject*>(DNone);
    } 
    return RealValue<DObject*>(new RPCObject(this->__networkStream, id.get<DUInt64>(), dstruct, this->__objects));
  }
  DValue value(DType(type).newValue());
  input >> value;

  return (value);
}

bool DSerializeRPC::deserialize(DStream& input, DObject& dobject) //UNUSED //XXX must return a DOBject can't construct it before !
{
  //std::cout << "SERIALIZE 6 " << std::endl;
  return (false);
}


//kind of specialization for string type 
bool DSerializeRPC::serialize(DStream& output, const std::string& str)
{
        //std::cout << "SERIALIZE 7 " << std::endl;
 size_t ssize = str.size();
  
  if (output.write((char*)&ssize, sizeof(ssize)).fail())
    return (false);
  if (output.write(str.c_str(), ssize).fail())
    return (false);

  return (true);
}

bool DSerializeRPC::deserialize(DStream& input, std::string& str)//UNUSED
{
  //std::cout << "SERIALIZE 8 " << std::endl;
  //size_t ssize;
  //if (input.read((char*)&ssize, sizeof(size_t)).fail())
    //return (false);

  //char* buff = new char[ssize];

  //if (buff == NULL)
    //return (false);
  //if (input.read(buff, ssize).fail())
    //return (false);

  //str = std::string(buff, ssize);
  //delete[] buff;  

  return (true);
}

}
