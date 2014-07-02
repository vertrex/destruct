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

DSerializeRPC::DSerializeRPC(NetworkStream stream) : __networkStream(stream)
{
}

const std::string DSerializeRPC::name(void)
{
  return ("RPC");
}

DSerializeRPC*   DSerializeRPC::create(void)
{
  return (new DSerializeRPC(this->__networkStream));
}

//throw rather than many check // ? use ./open read rather than slow stream?
bool DSerializeRPC::serialize(DStream& output, DStruct& dstruct)
{
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

DStruct* DSerializeRPC::deserialize(DStream& input)
{ 
  std::string name;
  size_t attributeCount;
  DStruct* dstruct = NULL; 

  if (this->deserialize(input, name) == false)
    return (NULL);
  if (input.read((char*)&attributeCount, sizeof(size_t)).fail())
    return (NULL);
 
  if ((dstruct = new DStruct(0, name, DSimpleObject::newObject)) == NULL) //XXX parent etc !! SimpleObject ?
    return (NULL);
  for (size_t i = 0; i < attributeCount; i++) 
  {
     DType::Type_t type;
     std::string   name;

     if (this->deserialize(input, name) == false)
       return (NULL);
     if (input.read((char*)&type, sizeof(type)).fail())
       return (NULL); //strange error checking !
     if (type == DType::DMethodType)
     {
       DType::Type_t argumentType;
       DType::Type_t returnType;
       if (input.read((char*)&argumentType, sizeof(type)).fail())
         return (NULL); //s
       if (input.read((char*)&returnType, sizeof(type)).fail())
         return (NULL); //s
       dstruct->addAttribute(DAttribute(returnType, name, argumentType, type));
     }
     else
       dstruct->addAttribute(DAttribute(type, name));
  }
  return (dstruct); 
}

bool DSerializeRPC::serialize(DStream& output, DValue value, DType type)
{
  if (type.getType() == DType::DObjectType)
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
  DValue v = RealValue<DUnicodeString>(dobject.instanceOf()->name());
//  objectManager->get(); ...

//XXX HAHAH c la merde :) 

//  DUInt64 id = dobject.getValue("id").get<DUInt64>(); //XXX XXX XXX XXX send it :) 
//  DUInt64 id = dobject.call("id").get<DUInt64>(); //XXX XXX XXX XXX send it :) 
//  this->server()->getObject(id); ...

  output << v;

  return (true);
}

DValue DSerializeRPC::deserialize(DStream& input, DType dtype)
{
  if (dtype.getType() == DType::DObjectType)
  {
    DValue rstring = RealValue<DUnicodeString>("");        
    input >>  rstring; //input >> objectName; //not implemented for string !
    std::string objectName = rstring.get<DUnicodeString>();


    DStruct* dstruct = Destruct::Destruct::instance().find(objectName);
    if (dstruct == NULL)
    {
      std::cout << "Can't deserialize object not find in base must get itstruct name :  " << objectName << std::endl;
      return RealValue<DObject*>(DNone);
    } 
    //XXX XXX XXX XXX READ ID HERE TO GET THE RIGHT OBJECT

    return RealValue<DObject*>(new RPCObject(this->__networkStream, 0, dstruct)); //serialize/dserialize uri too //XXX fix me please
  }
  DValue value(dtype.newValue());
  input >> value;

  return (value);
}

bool DSerializeRPC::deserialize(DStream& input, DObject& dobject) //XXX must return a DOBject can't construct it before !
{
  return (false);
}

//king of specialization for string type 

bool DSerializeRPC::serialize(DStream& output, const std::string& str)
{
 size_t ssize = str.size();
  
  if (output.write((char*)&ssize, sizeof(ssize)).fail())
    return (false);
  if (output.write(str.c_str(), ssize).fail())
    return (false);

  return (true);
}

bool DSerializeRPC::deserialize(DStream& input, std::string& str)
{
  size_t ssize;
  if (input.read((char*)&ssize, sizeof(size_t)).fail())
    return (false);

  char* buff = new char[ssize];

  if (buff == NULL)
    return (false);
  if (input.read(buff, ssize).fail())
    return (false);

  str = std::string(buff, ssize);
  delete[] buff;  

  return (true);
}

}
