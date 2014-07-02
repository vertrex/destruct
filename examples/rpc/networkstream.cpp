#include<sys/socket.h>    //socket
#include<arpa/inet.h> //inet_addr

#include "destruct.hpp"
#include "dsimpleobject.hpp"

#include "networkstream.hpp"

namespace Destruct 
{

//NetworkStream::NetworkStream(int socket) : __socket(socket)
NetworkStream::NetworkStream(DStruct* dstruct, DValue const& args) : DStream(dstruct)
{
  this->__socket = args.get<DInt32>();
}

NetworkStream::~NetworkStream()
{
  this->__close();
}

int32_t NetworkStream::write(std::string const& str) const
{
  uint64_t size = str.size();
  uint32_t readed = this->write((void*)&size, sizeof(size));
  if (readed != sizeof(size))
    throw std::string("Can't write string size");
 
  readed = this->write((void*)str.c_str(), size);
  if (readed != size)
    throw std::string("Can't write string");
  return str.size();
}

int32_t NetworkStream::write(void* buff, int32_t size) const 
{
  return (send(this->__socket, buff, size, 0));
}

int32_t NetworkStream::read(std::string & readValue)
{
  uint64_t size = 0;
  if (this->read(&size, sizeof(size)) != sizeof(size))
    throw std::string("NetworkStream::read can't get size");
  uint8_t*  value = new uint8_t[size + 1];
  this->read(value, size); //test return value
  value[size] = 0;
  readValue = std::string((char*)value, size);
  delete value;

  return readValue.size();
}

int32_t NetworkStream::read(void* buff, int32_t size)
{
  return(recv(this->__socket, buff, size, 0));
}


void NetworkStream::__close(void)
{
}

/*
 *   DSerializeRPC 
 */
const std::string DSerializeRPC::name(void)
{
  return ("RPC");
}

DSerializeRPC*   DSerializeRPC::create(void)
{
  return (new DSerializeRPC);
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
    //return RPCObject(networkStream, "root", dstruct); //serialize/dserialize uri too //XXX fix me please
    return RealValue<DObject*>(dstruct->newObject()); //XXXX XXX RETURN RPCOBJECT AND C BON :) 
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
